#include "extra.h"
#include "mma8451.h"
#include <math.h>
#include <stdio.h>
void delay(uint32_t value){
	
	uint32_t delay_val, x;
	
	for(x=0; x < value; x++){
		for(delay_val=0; delay_val < 10000; delay_val++){}; 
	}
}


uint8_t get_array_index(uint8_t accel_read){ // function to calculate which column shoub be turned on
	float_t pixel_div = 0.66;
	uint8_t index;
	if(accel_read <= 0x40 && accel_read >= 0x00){
		index = 42 - (accel_read * pixel_div);
	} else {
		accel_read = accel_read - 192;
		index = 84 - (accel_read * pixel_div);
	}
	
	return index;
}

