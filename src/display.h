#include "MKL05Z4.h"

/*DEFINITION OF REGISTER MASKS*/
	#define Y_MASK (1u<<6)
	#define X_MASK (1u<<7)
	#define H_MASK 1u
	#define V_MASK (1u<<1)
	#define FUNCTION_SET_MASK (1u<<5)
	#define DISPLAY_CONTROL_MASK (1u<<3)
	#define D_MASK (1u<<2)
	#define E_MASK 1u
	#define VOP_MASK (1u<<7)
	#define BIAS_MASK (1u<<4)


const static uint32_t dc_mask = (1UL << 7);
const static uint32_t rst_mask = (1UL << 6);

void displayInitialize(void);
void move_XY(uint8_t X,uint8_t Y);
void print_character(char x);
void print_character_2(uint8_t x);

void sendCommand(uint8_t cmd);
void sendData(uint8_t data);
void print_vertical_line(void);
void clear_all_columns(void);