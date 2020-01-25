#include "i2c.h"

void i2cInitialize(void){

	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    
	PORTB->PCR[3] |= PORT_PCR_MUX(2); // I2C0 SCL
	PORTB->PCR[4] |= PORT_PCR_MUX(2); // I2C0 SDA

	I2C0->F = I2C_F_ICR(14);					// baud rate settings
	
	I2C0->C1 |= I2C_C1_IICEN_MASK;		// Enable I2C
	I2C0->C1 |= I2C_C1_IICIE_MASK;		// Set to master mode
	I2C0->C2 |= I2C_C2_HDRS_MASK;			// High drive mode
}

void i2cWait(void){
	
	while((I2C0->S & I2C_S_IICIF_MASK) == 0){
	} 
	I2C0->S |= I2C_S_IICIF_MASK;
}

void i2cSendSingleByte(uint8_t dev, uint8_t address, uint8_t data){
	
	I2C_TRAN; 					//set to transmit mode 
	I2C_M_START; 				//send start 
	I2C0->D = dev; 			//send dev address 
	WAIT; 							//wait for ack
	I2C0->D = address; 	//send write address 
	WAIT;
	I2C0->D = data;			//send data 
	WAIT;
	I2C_M_STOP;					//send stop 
}

uint8_t i2cReadSingleByte(uint8_t dev, uint8_t address){

	uint8_t data;
	
	I2C_TRAN; 						//set to transmit mode 
	I2C_M_START; 					//send start
	I2C0->D = dev; 				//send dev address 
	WAIT;									//wait for completion
	I2C0->D = address; 		//send read address 
	WAIT;									//wait for completion 
	I2C_M_RSTART;					//repeated start 
	I2C0->D = (dev|0x1);	//send dev address (read)
	WAIT;			 						//wait for completion 
	I2C_REC; 							//set to recieve mode 
	NACK; 								//set NACK after read 
	data = I2C0->D; 			//dummy read 
	WAIT;			 						//wait for completion 
	I2C_M_STOP; 					//send stop 
	data = I2C0->D; 			//read data
	
	return data;
}