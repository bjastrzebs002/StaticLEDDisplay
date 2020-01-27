/*#include "i2c.h"
#include "extra.h"
#include "mma8451.h"

void i2cInitialize(void){

	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    
	PORTB->PCR[3] |= PORT_PCR_MUX(2); // I2C0 SCL
	PORTB->PCR[4] |= PORT_PCR_MUX(2); // I2C0 SDA

	I2C0->F = I2C_F_ICR(14);					// baud rate settings
	
	I2C0->C1 |= I2C_C1_IICEN_MASK;		// Enable I2C
	I2C0->C1 |= I2C_C1_IICIE_MASK;		// Set to master mode
	I2C0->C2 |= I2C_C2_HDRS_MASK;			// High drive mode
	
	i2cSendSingleByte(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG, 1);
	delay(100);
	i2cSendSingleByte(MMA8451_I2C_ADDRESS, CTRL_REG1_REG, 1);
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
*/
#include "MKL05Z4.h"                    	/* Device header */
#include "i2c.h"													/* Leds functions for FRDM46-KL46 */
#include <stdio.h>
#include <string.h>
#include "mma8451.h"

#define I2C_M_START I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_TRAN I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC I2C0->C1 &= ~I2C_C1_TX_MASK
#define BUSY_ACK while(I2C0->S & 0x01)
#define TRANS_COMP while(!(I2C0->S & 0x80))
#define I2C_WAIT i2c_wait();

#define NACK I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK I2C0->C1 &= ~I2C_C1_TXAK_MASK
void i2c_start()
{
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
}
 void i2cInitialize()
{
	uint8_t temp;
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
  SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK);

	PORTB->PCR[3] = PORT_PCR_MUX(2);  
	PORTB->PCR[4] = PORT_PCR_MUX(2);  
	
	//	I2C baud rate = bus speed (Hz)/(mul × SCL divider)
	//	bus speed (Hz) = 21000000000
	//	ICR = 14 => SCL = 56		mul = 0
	//	set to 375k baud

 	I2C0->F = (I2C_F_ICR(14));
		
	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);
	I2C0->C1 |= (I2C_C1_IICIE_MASK);
	
	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
	//i2c_write_byte(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG, 1);
	
	i2c_write_byte(MMA8451_I2C_ADDRESS, CTRL_REG1_REG, 1);
	delay(5);

}  

void i2c_read_setup(uint8_t device, uint8_t address){
	I2C_TRAN;
	I2C_M_START;
	I2C0->D = device;
	I2C_WAIT;
	I2C0->D = address;
	I2C_WAIT;
	I2C_M_RSTART;
	I2C0->D =(device|0x1);
	I2C_WAIT;
	I2C_REC;
}

void read_full_xyz(uint8_t *p_x, uint8_t *p_y, uint8_t *p_z)
{
	uint8_t i;
	uint8_t data[6];
	
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start*/
	
	i2c_read_setup(MMA8451_I2C_ADDRESS, OUT_X_MSB_REG);
	
// Read five bytes in repeated read mode to data array "data[6]"
	for(i = 0; i<5; i++){
		data[i] = i2c_read_mult_bytes(0);
	}
// Read last byte ending repeated read_mode
	data[i] = i2c_read_mult_bytes(1);
	
	*p_x = data[0];
	*p_y = data[2];
	*p_z = data[4];
}

uint8_t i2c_read_mult_bytes(uint8_t isLastRead){
	uint8_t data;
	if(isLastRead){       /*If this is the last READ (isLastRead); if no, send NACK; if yes, send ACK*/
			NACK; 				    /*set NACK after read	*/
	}
	else{
			ACK;					    /*ACK after read	*/
	}
	data = I2C0->D;				/*Add a dummy read*/	
	I2C_WAIT;							/*wait for completion*/            			    
	if(isLastRead)	{     //if this is the last READ (isLastRead), if yes, send STOP
			I2C_M_STOP;       /*send stop	*/
	}
	data = I2C0->D; 			/*read data	*/
	return data;					/*return data*/				
}

void i2c_wait(void) {
	while((I2C0->S & I2C_S_IICIF_MASK)==0) {
	} 
  I2C0->S |=I2C_S_IICIF_MASK;
}


//send device and register addresses
//read a byte and ack/nack as appropriate
// #pragma no_inline 

//////////funcs for reading and writing a single byte
//using 7bit addressing reads a byte from dev:address
// #pragma no_inline 
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;
	i2c_start();							/*set to transmit mode */
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT							/*wait for completion */
	
	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT							/*wait for completion */
		
	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT							 /*wait for completion */
	
	I2C_REC;						   /*set to recieve mode */
	NACK;									 /*set NACK after read	*/
	
	data = I2C0->D;					/*dummy read	*/
	I2C_WAIT								/*wait for completion */
	
	I2C_M_STOP;							/*send stop	*/
	data = I2C0->D;					/*read data	*/

	return data;
}



//using 7bit addressing writes a byte data to dev:address
#pragma no_inline 
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{
	
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT						  /*wait for ack */
	
	I2C0->D =  address;		/*send write address	*/
	I2C_WAIT
		
	I2C0->D = data;				/*send data	*/
	I2C_WAIT
	I2C_M_STOP;
}