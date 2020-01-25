#include "spi.h"
#include "display.h"
#include "MKL05Z4.h"
#include "i2c.h"
uint8_t data = 10;

#include "extra.h"
#include "mma8451.h"


uint8_t accel_values[3] = {0, 0, 0};
int main(void){
	uint32_t waittime = 0;
	
	//spiInitialize();
	displayInitialize();
	
	i2cInitialize();
	i2cSendSingleByte(MMA8451_I2C_ADDRESS, CTRL_REG1_REG, 1); // Active mode
	
	// ACCELEROMETER
	accelInitialize();
	
	while(1){
		// get values from accelerometer to array 
		accel_values[0] = accelReadX();
		accel_values[1] = accelReadY();
		accel_values[2] = accelReadZ();
		
		// set cursor on column which should be turned on
		move_XY(get_array_index(accel_values[0]), 0x00);
		
		// print vertical line in defined column
		print_vertical_line();
		
		// delay 
		delay(1000);
		
		// clear all columns
		clear_all_columns();
	}
}