
#ifndef RPI2_AUX_H
#define RPI2_AUX_H


#include "RPI_BASE.h"


#define AUX_IRQ         (PERIPHERAL_BASE + 0x00215000)
#define AUX_ENABLES     (PERIPHERAL_BASE + 0x00215004)



//------------------------------------------------------------------------------
// Mini UART GPIO (first group omn ALT5 - others on 30-33 and 40-43)
#define MU_TXD   GPIO14
#define MU_RXD   GPIO15
#define MU_CTS   GPIO16
#define MU_RTS   GPIO17

//------------------------------------------------------------------------------
// Mini UART registers
#define AUX_MU_IO_REG   (PERIPHERAL_BASE + 0x00215040)
#define AUX_MU_IER_REG  (PERIPHERAL_BASE + 0x00215044)
#define AUX_MU_IIR_REG  (PERIPHERAL_BASE + 0x00215048)
#define AUX_MU_LCR_REG  (PERIPHERAL_BASE + 0x0021504C)
#define AUX_MU_MCR_REG  (PERIPHERAL_BASE + 0x00215050)
#define AUX_MU_LSR_REG  (PERIPHERAL_BASE + 0x00215054)
#define AUX_MU_MSR_REG  (PERIPHERAL_BASE + 0x00215058)
#define AUX_MU_SCRATCH  (PERIPHERAL_BASE + 0x0021505C)
#define AUX_MU_CNTL_REG (PERIPHERAL_BASE + 0x00215060)
#define AUX_MU_STAT_REG (PERIPHERAL_BASE + 0x00215064)
#define AUX_MU_BAUD_REG (PERIPHERAL_BASE + 0x00215068)

// SPI0 registers
#define AUX_SPI0_CNTL0_REG (PERIPHERAL_BASE + 0x00215080)
#define AUX_SPI0_CNTL1_REG (PERIPHERAL_BASE + 0x00215084)
#define AUX_SPI0_STAT_REG  (PERIPHERAL_BASE + 0x00215088)
#define AUX_SPI0_IO_REG    (PERIPHERAL_BASE + 0x00215090)
#define AUX_SPI0_PEEK_REG  (PERIPHERAL_BASE + 0x00215094)

// SPI1 registers
#define AUX_SPI1_CNTL0_REG (PERIPHERAL_BASE + 0x002150C0)
#define AUX_SPI1_CNTL1_REG (PERIPHERAL_BASE + 0x002150C4)
#define AUX_SPI1_STAT_REG  (PERIPHERAL_BASE + 0x002150C8)
#define AUX_SPI1_IO_REG    (PERIPHERAL_BASE + 0x002150D0)
#define AUX_SPI1_PEEK_REG  (PERIPHERAL_BASE + 0x002150D4)


// Constants...
// For AUX_IRQ
#define AUX_IRQ_SPI2 (1 << 2)
#define AUX_IRQ_SPI1 (1 << 1)
#define AUX_IRQ_MU   (1 << 0)

// For AUX_ENABLES
#define AUX_ENB_SPI2   (1 << 2)
#define AUX_ENB_SPI1   (1 << 1)
#define AUX_ENB_MU     (1 << 0)

// For MU_IO
#define AUX_MU_IO_MASK   (0x000000FF)

// For MU_IER
#define AUX_MU_INT_RX_MASK   (1 << 1)
#define AUX_MU_INT_TX_MASK   (1 << 0)

// for MU_IIR
#define AUX_MU_IID_FIFOCLR   (3 << 1)
#define AUX_MU_INT_PENDING   (1 << 0)

// For MU_LCR
#define AUX_MU_LCR_DLAB       (1 << 7)
#define AUX_MU_LCR_BREAK      (1 << 6)
#define AUX_MU_LCR_DATASIZE   ( 1 )

// For MU_MCR
#define AUX_MU_MCR_RTS   (1 << 1)

// For MU_LSR
#define AUX_MU_LSR_TXIDLE    (1 << 6)
#define AUX_MU_LSR_TXEMPTY   (1 << 5)
#define AUX_MU_LSR_RXOVRR    (1 << 1)
#define AUX_MU_LSR_DATARDY   (1 << 0)

// For MU_MSR
#define AUX_MU_LSR_CTS   (1 << 5)

// For SCRTACH
#define AUX_MU_SCRATCH_MASK   (0x000000FF)

// For MU_CNTL
#define AUX_MU_CNTL_CTS_LVL        (1 << 7)
#define AUX_MU_CNTL_RTS_LVL        (1 << 6)
#define AUX_MU_CNTL_RTS_FLW_3      (0b00 << 4)
#define AUX_MU_CNTL_RTS_FLW_2      (0b01 << 4)
#define AUX_MU_CNTL_RTS_FLW_1      (0b10 << 4)
#define AUX_MU_CNTL_RTS_FLW_0      (0b11 << 4)
#define AUX_MU_CNTL_EN_TX_AFCCTS   (1 << 3)
#define AUX_MU_CNTL_EN_RX_AFCRTS   (1 << 2)
#define AUX_MU_CNTL_TX_EN          (1 << 1)
#define AUX_MU_CNTL_RX_EN          (1 << 0)

// For MU_STAT
#define AUX_MU_STAT_TXFIFO_LVL     (0xF << 24)
#define AUX_MU_STAT_RXFIFO_LVL     (0xF << 16)
#define AUX_MU_STAT_TX_DONE        (1 << 9)
#define AUX_MU_STAT_TX_FIFO_EMPTY  (1 << 8)
#define AUX_MU_STAT_CTS_LINE       (1 << 7)
#define AUX_MU_STAT_RTS_STATUS     (1 << 6)
#define AUX_MU_STAT_TX_FIFO_FULL   (1 << 5)
#define AUX_MU_STAT_RX_OVRRN       (1 << 4)
#define AUX_MU_STAT_TX_IDLE        (1 << 3)
#define AUX_MU_STAT_RX_IDLE        (1 << 2)
#define AUX_MU_STAT_SPC_AVAIL      (1 << 1)
#define AUX_MU_STAT_SBL_AVAIL      (1 << 0)

// For MU_BAUD
#define AUX_MU_BAUD_MASK   (0x0000FFFF)


// Services

extern void muInit (void);

extern void muLogInt2Hex (unsigned int d);
extern void muLogInt2Bin (unsigned int d);
extern void muLogChar (char c, char md);
extern void muLogString (char * str);
extern void muLogString2 (char * str);
extern void muLogStrInt (char * str, uint32_t d);

extern char muRxFifoNotEmpty (void);
extern unsigned int muGetRxByte (void);
extern unsigned int muWaitReceiveByte (void);
extern void muSendByte (unsigned int d);



#endif /* RPI2_AUX_H */
