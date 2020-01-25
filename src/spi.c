#include "spi.h"
#include "MKL05Z4.h"
void spiInitialize(void)
  {
         SIM->SCGC5|=SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK; // Enable Clock
         PORTA->PCR[5] |= PORT_PCR_MUX(1); //ss
         PORTB->PCR[0] |= PORT_PCR_MUX(3); //clk
         PORTA->PCR[7] |= PORT_PCR_MUX(3); //mosi
         PORTA->PCR[6] |= PORT_PCR_MUX(3); //miso

         PTA->PDDR |= (1<<5); //initialize GPIO as OUTPUT
         PTA->PSOR |= (1<<5); //SS set to high;

         SIM->SCGC4|=SIM_SCGC4_SPI0_MASK; // enable Clock for SPI0 module
         SPI0->C1 = 0;
         SPI0->C1 |= SPI_C1_SPE_MASK| SPI_C1_MSTR_MASK  ; // ENABLE SPI | SET AS MASTER
         SPI0->C1 &= ~(SPI_C1_CPOL_MASK|SPI_C1_SPIE_MASK|SPI_C1_CPHA_MASK);// INTERRUPT DISABLE- USING POLLING
         SPI0->C2 &= ~(SPI_C2_SPMIE_MASK); // 8 BIT | USING POLLING

         SPI0->BR = 35; // SET BOUNDRATE


  }

/*----------------------------------------------------------------------------
   Function that send a N byte frame 8 bit data each

*----------------------------------------------------------------------------*/
  void spiWrite1(uint8_t dataWrite)
  {

         PTA->PCOR |= (1<<5); // SS set to low ;

                 while((SPI0->S & SPI_S_SPTEF_MASK)!=SPI_S_SPTEF_MASK); // wait to spi buffer is empty

                 SPI0->D = dataWrite; // send data

                 while((SPI0->S & SPI_S_SPRF_MASK)!=SPI_S_SPRF_MASK);

                 SPI0->D;       // dummy read
         PTA->PSOR |= (1<<5); //SS set to high;
  }