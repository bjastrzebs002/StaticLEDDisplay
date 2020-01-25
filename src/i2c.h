#ifndef i2c_h
#define i2c_h

#include "MKL05Z4.h"

#define I2C_M_START I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_TRAN I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC I2C0->C1 &= ~I2C_C1_TX_MASK
#define BUSY_ACK while(I2C0->S & 0x01)
#define TRANS_COMP while(!(I2C0->S & 0x80))
#define WAIT i2cWait();
#define NACK I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK I2C0->C1 &= ~I2C_C1_TXAK_MASK

void i2cInitialize(void);
void i2cWait(void);
void i2cSendSingleByte(uint8_t dev, uint8_t address, uint8_t data);
uint8_t i2cReadSingleByte(uint8_t dev, uint8_t address);

#endif