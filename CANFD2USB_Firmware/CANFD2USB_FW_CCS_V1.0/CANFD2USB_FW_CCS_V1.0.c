#include "ti_msp_dl_config.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// ── CANtrace wire protocol ────────────────────────────────────────────────────
#define SOF        0xAA
#define EOF_BYTE   0x55
#define FRAME_SIZE 73

#define FLAG_ERR   0x20   // error frame — avoids clash with ESI (0x10)
#define FLAG_ACK   0x40   // TX success confirmation back to PC

// ── CPU / CAN timing ──────────────────────────────────────────────────────────
#define CPU_FREQ_HZ            32000000UL    // 80 MHz (MSPM0G3507)
#define CAN_FRAME_BITS         130UL       // worst-case classic CAN frame bits
#define CAN_FD_BITS            200UL       // worst-case CAN FD frame bits
#define ACK_TIMEOUT_MULTIPLIER 3UL         // 3× safety margin (automotive standard)

// Set these to match your SYSCFG_DL_MCAN0_init() configuration
#define MCAN_NOMINAL_BITRATE_BPS   500000UL
#define MCAN_DATA_BITRATE_BPS      2000000UL

// ── Compute ACK timeout cycles based on bitrate ───────────────────────────────
// Formula: multiplier × bits × (CPU_freq / bitrate)
static uint32_t can_ack_timeout_cycles(uint32_t bitrate_bps, bool is_fd)
{
    uint32_t bits = is_fd ? CAN_FD_BITS : CAN_FRAME_BITS;
    // Avoid overflow: compute (CPU_FREQ / bitrate) first, then multiply
    // At 32MHz / 500kbps = 64 cycles per bit
    uint32_t cycles_per_bit = CPU_FREQ_HZ / bitrate_bps;
    return ACK_TIMEOUT_MULTIPLIER * bits * cycles_per_bit;
}

// ── UART TX ───────────────────────────────────────────────────────────────────
static void uart_write_byte(uint8_t b)
{
    while (DL_UART_Main_isBusy(UART_0_INST));
    DL_UART_Main_transmitData(UART_0_INST, b);
}

// ── DLC to actual byte count ──────────────────────────────────────────────────
static uint8_t dlc_to_len(uint8_t dlc)
{
    static const uint8_t table[] =
        {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};
    return (dlc < 16) ? table[dlc] : 64;
}

// ── Send normal CAN frame to CANtrace (RX path) ───────────────────────────────
static void send_cantrace_frame(uint32_t id, uint8_t dlc,
                                uint8_t *data, uint8_t dataLen,
                                bool ext, bool fd, bool brs)
{
    uint8_t buf[FRAME_SIZE];
    memset(buf, 0x00, FRAME_SIZE);

    buf[0] = SOF;
    buf[1] = (id)       & 0xFF;
    buf[2] = (id >>  8) & 0xFF;
    buf[3] = (id >> 16) & 0xFF;
    buf[4] = (id >> 24) & 0xFF;
    buf[5] = (ext ? 0x01 : 0x00)
           | (fd  ? 0x04 : 0x00)
           | (brs ? 0x08 : 0x00);
    buf[6] = dlc;
    buf[7] = dataLen;

    uint8_t len = (dataLen > 64) ? 64 : dataLen;
    for (uint8_t i = 0; i < len; i++)
        buf[8 + i] = data[i];

    buf[FRAME_SIZE - 1] = EOF_BYTE;

    for (int i = 0; i < FRAME_SIZE; i++)
        uart_write_byte(buf[i]);
}

// ── Send TX-ACK frame to CANtrace (TX success) ────────────────────────────────
static void send_cantrace_ack_frame(uint32_t id, uint8_t dlc,
                                    uint8_t *data, uint8_t dataLen,
                                    bool ext, bool fd, bool brs)
{
    uint8_t buf[FRAME_SIZE];
    memset(buf, 0x00, FRAME_SIZE);

    buf[0] = SOF;
    buf[1] = (id)       & 0xFF;
    buf[2] = (id >>  8) & 0xFF;
    buf[3] = (id >> 16) & 0xFF;
    buf[4] = (id >> 24) & 0xFF;
    buf[5] = FLAG_ACK
           | (ext ? 0x01 : 0x00)
           | (fd  ? 0x04 : 0x00)
           | (brs ? 0x08 : 0x00);
    buf[6] = dlc;
    buf[7] = dataLen;

    uint8_t len = (dataLen > 64) ? 64 : dataLen;
    for (uint8_t i = 0; i < len; i++)
        buf[8 + i] = data[i];

    buf[FRAME_SIZE - 1] = EOF_BYTE;

    for (int i = 0; i < FRAME_SIZE; i++)
        uart_write_byte(buf[i]);
}

// ── Send error frame to CANtrace (TX failure) ─────────────────────────────────
// errType (MCAN PSR LEC):
//   1 = Stuff Error
//   2 = Form Error
//   3 = ACK Error
//   4 = Bit1 Error
//   5 = Bit0 Error
//   6 = CRC Error
static void send_cantrace_error_frame(uint8_t errType)
{
    uint8_t buf[FRAME_SIZE];
    memset(buf, 0x00, FRAME_SIZE);
    buf[0] = SOF;
    buf[5] = FLAG_ERR;
    buf[6] = errType;
    buf[FRAME_SIZE - 1] = EOF_BYTE;
    for (int i = 0; i < FRAME_SIZE; i++)
        uart_write_byte(buf[i]);
}

// ── RX state machine ──────────────────────────────────────────────────────────
static uint8_t  s_txBuf[FRAME_SIZE];
static uint8_t  s_txIdx   = 0;
static bool     s_inFrame = false;

static void process_uart_rx(void)
{
    uint8_t b;

    while (!DL_UART_Main_isRXFIFOEmpty(UART_0_INST))
    {
        b = DL_UART_Main_receiveData(UART_0_INST);

        if (!s_inFrame) {
            if (b == SOF) {
                s_txBuf[0] = b;
                s_txIdx    = 1;
                s_inFrame  = true;
            }
        } else {
            s_txBuf[s_txIdx++] = b;

            if (s_txIdx >= FRAME_SIZE) {
                s_inFrame = false;
                s_txIdx   = 0;

                // Validate EOF
                if (s_txBuf[FRAME_SIZE - 1] != EOF_BYTE)
                    return;

                // Parse fields
                uint32_t id = ((uint32_t)s_txBuf[1])
                            | ((uint32_t)s_txBuf[2] <<  8)
                            | ((uint32_t)s_txBuf[3] << 16)
                            | ((uint32_t)s_txBuf[4] << 24);

                uint8_t flags = s_txBuf[5];
                bool ext      = (flags & 0x01) != 0;
                bool fd       = (flags & 0x04) != 0;
                bool brs      = (flags & 0x08) != 0;
                uint8_t dlc   = s_txBuf[6] & 0x0F;
                uint8_t dlen  = s_txBuf[7];

                // Build MCAN TX element
                DL_MCAN_TxBufElement tx;
                memset(&tx, 0, sizeof(tx));
                tx.id  = ext ? id : (id << 18U);
                tx.xtd = ext ? 1U : 0U;
                tx.rtr = 0U;
                tx.fdf = fd  ? 1U : 0U;
                tx.brs = brs ? 1U : 0U;
                tx.dlc = dlc;
                tx.esi = 0U;
                tx.efc = 0U;
                tx.mm  = 0U;

                uint8_t len = (dlen > 64) ? 64 : dlen;
                for (uint8_t i = 0; i < len; i++)
                    tx.data[i] = s_txBuf[8 + i];

                // Transmit
                DL_MCAN_writeMsgRam(MCAN0_INST,
                                    DL_MCAN_MEM_TYPE_BUF, 0, &tx);
                DL_MCAN_TXBufAddReq(MCAN0_INST, 0U);

                // ── Automotive-grade ACK timeout ──────────────────────────────
                // 3× worst-case frame time at nominal bitrate
                // Classic CAN: 130 bits / bitrate × 3
                // CAN FD:      200 bits / bitrate × 3
                uint32_t timeout = can_ack_timeout_cycles(
                                       MCAN_NOMINAL_BITRATE_BPS, fd);

                while (timeout--) {
                    if (!(DL_MCAN_getTxBufReqPend(MCAN0_INST) & 0x1))
                        break;
                }

                // ── Sample result ONCE — one status per message ───────────────
                DL_MCAN_ProtocolStatus ps;
                DL_MCAN_getProtocolStatus(MCAN0_INST, &ps);

                if (ps.lastErrCode != 0 && ps.lastErrCode != 7) {
                    // TX failed — report error
                    send_cantrace_error_frame((uint8_t)ps.lastErrCode);
                } else {
                    // TX succeeded — confirm to PC
                    send_cantrace_ack_frame(id, dlc,
                                            s_txBuf + 8, dlen,
                                            ext, fd, brs);
                }
            }
        }
    }
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main(void)
{
    SYSCFG_DL_init();

    // Startup delay
    volatile uint32_t d = 200000;
    while (d--);

    // Force MCAN clean init
    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_SW_INIT);
    while (DL_MCAN_OPERATION_MODE_SW_INIT != DL_MCAN_getOpMode(MCAN0_INST));

    SYSCFG_DL_MCAN0_init();

    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_NORMAL);
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST));

    while (1)
    {
        // ── Process incoming UART → CAN TX requests from PC ───────────────────
        process_uart_rx();

        // ── Forward received CAN frames to CANtrace ───────────────────────────
        DL_MCAN_RxFIFOStatus status;
        status.num = DL_MCAN_RX_FIFO_NUM_0;
        DL_MCAN_getRxFIFOStatus(MCAN0_INST, &status);

        while (status.fillLvl > 0)
        {
            DL_MCAN_RxBufElement rx;
            memset(&rx, 0, sizeof(rx));

            uint32_t getIdx = status.getIdx;

            DL_MCAN_readMsgRam(MCAN0_INST,
                               DL_MCAN_MEM_TYPE_FIFO,
                               0,
                               DL_MCAN_RX_FIFO_NUM_0,
                               &rx);

            DL_MCAN_writeRxFIFOAck(MCAN0_INST,
                                   DL_MCAN_RX_FIFO_NUM_0,
                                   getIdx);

            bool     ext     = (rx.xtd == 1U);
            bool     fd      = (rx.fdf == 1U);
            bool     brs     = (rx.brs == 1U);
            uint8_t  dlc     = (uint8_t)rx.dlc;
            uint8_t  dataLen = dlc_to_len(dlc);
            uint32_t id      = ext ? rx.id : ((rx.id >> 18U) & 0x7FFU);

            send_cantrace_frame(id, dlc,
                                (uint8_t *)rx.data, dataLen,
                                ext, fd, brs);

            status.num = DL_MCAN_RX_FIFO_NUM_0;
            DL_MCAN_getRxFIFOStatus(MCAN0_INST, &status);
        }
    }
}