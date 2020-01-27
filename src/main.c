#include "spi.h"
#include "display.h"
#include "MKL05Z4.h"
#include "i2c.h"

#include "extra.h"
#include "mma8451.h"


uint8_t accel_values[3] = {0, 0, 0};
int main(void){

	//spiInitialize();
	displayInitialize();
	
	accelInitialize();
	
	clear_all_columns();
	while(1){
		read_full_xyz(accel_values, accel_values +1, accel_values + 2);
		// set cursor on column which should be turned on
		uint8_t i;
		for(i = 0; i< 6; i++){
			move_XY(get_array_index(accel_values[0]), i);
			// print vertical line in defined column
			print_vertical_line();
		}
		
		
		delay(100);
		
		// clear all columns
		clear_all_columns();

	}
}