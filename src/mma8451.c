#include "mma8451.h"


void accelInitialize(void){
	
	i2cInitialize();
	i2cSendSingleByte(MMA8451_I2C_ADDRESS, CTRL_REG1_REG, 1); // Active mode
}

uint8_t accelReadX(void){
	
	uint8_t result_x;
	result_x = i2cReadSingleByte(MMA8451_I2C_ADDRESS,OUT_X_MSB_REG);
	return result_x;
}

uint8_t accelReadY(void){
	
	uint8_t result_y;
	result_y   = i2cReadSingleByte(MMA8451_I2C_ADDRESS, OUT_Y_MSB_REG);
	return result_y;
}

uint8_t accelReadZ(void){
	
	uint8_t result_z;
	result_z   = i2cReadSingleByte(MMA8451_I2C_ADDRESS, OUT_Z_MSB_REG);
	return result_z;
}


