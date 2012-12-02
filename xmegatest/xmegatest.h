#ifndef _XMEGATEST_H_
#define _XMEGATEST_H_

////////////////////////////////////////////////////////
//                   Include Files                    //
////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

////////////////////////////////////////////////////////
//                  Hardware Defines                  //
////////////////////////////////////////////////////////
//PORTA
#define TEST2                                PIN0_bm
#define TEST3                                PIN1_bm
#define TEST6                                PIN2_bm
#define TEST5                                PIN3_bm
#define TEST4                                PIN4_bm
#define TEST7                                PIN5_bm

//PORTB
#define TEST12                               PIN2_bm

//PORTC
#define USB_UART_TXD                         PIN2_bm  // Input
#define USB_UART_RXD                         PIN3_bm  // Output
#define MB_MOSI                              PIN5_bm
#define MB_MISO                              PIN6_bm
#define MB_SCK                               PIN7_bm

//PORTD
#define FLASH_CS                             PIN0_bm
#define TEST13                               PIN1_bm
#define LED0                                 PIN2_bm
#define LED1                                 PIN3_bm
#define TEST11                               PIN5_bm
#define TEST10                               PIN6_bm
#define TEST9                                PIN7_bm

//PORTE
#define TEST8                                PIN0_bm

//PORTR
#define TEST0                                PIN0_bm
#define TEST1                                PIN1_bm

////////////////////////////////////////////////////////
//                  Software Defines                  //
////////////////////////////////////////////////////////
#define BSELH                                0x2E
#define BSELL                                0x08
#define BSCALE                               0b10010000

////////////////////////////////////////////////////////
//                    Flash Defines                   //
////////////////////////////////////////////////////////
#define AT45OP_MAINMEMPAGE_READ              0xD2
#define AT45OP_CONTARRAY_READ                0x03
#define AT45OP_BUFFER0_READ                  0xD1
#define AT45OP_BUFFER1_READ                  0xD3
#define AT45OP_BUFFER0_WRITE                 0x84
#define AT45OP_BUFFER1_WRITE                 0x87
#define AT45OP_BUFFER0PROG_ER                0x83
#define AT45OP_BUFFER1PROG_ER                0x86
#define AT45OP_BUFFER0PROG_NOER              0x88
#define AT45OP_BUFFER1PROG_NOER              0x89
#define AT45OP_MEMPAGEPROG_BUF0              0x82
#define AT45OP_MEMPAGEPROG_BUF1              0x85
#define AT45OP_ERASEPAGE                     0x81
#define AT45OP_ERASEBLOCK                    0x50
#define AT45OP_ERASESECTOR                   0x7C
#define AT45OP_ERASECHIP0                    0xC7
#define AT45OP_ERASECHIP1                    0x94
#define AT45OP_ERASECHIP2                    0x80
#define AT45OP_ERASECHIP3                    0x9A
#define AT45OP_MEMPAGE_TO_BUF0               0x53
#define AT45OP_MEMPAGE_TO_BUF1               0x55
#define AT45OP_MEMPAGE_CMP_BUF0              0x60
#define AT45OP_MEMPAGE_CMP_BUF1              0x61
#define AT45OP_DEEPPOWERDOWN                 0xB9
#define AT45OP_RESUME_DEEPPWR                0xAB
#define AT45OP_STATUSREG_READ                0xD7
#define AT45OP_DEVICE_ID                     0x9F

#define AT45_STATUSREG_RDY                   0x80
#define AT45_STATUSREG_CMP                   0x40

#define AT45_PAGE_SIZE                       528
#define AT45_PAGE_COUNT                      4096
#define AT45_BLOCK_COUNT                     512

////////////////////////////////////////////////////////
//                Function Prototypes                 //
////////////////////////////////////////////////////////
void         MCU_Init( void );
int          USB_Putchar (char c, FILE *stream);
void         CCP_CLK( uint8_t );
void         CCP_RST( void );

////////////////////////////////////////////////////////
//                      Globals                       //
////////////////////////////////////////////////////////
volatile uint16_t sine_frequency;
volatile uint8_t  newrx;

#endif