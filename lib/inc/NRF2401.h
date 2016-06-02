#ifndef _NRF2401_H_
#define _NRF2401_H_
#include<Config.h>
#include<delay.h>

#define  GND  Pin1
#define  VCC  Pin2
#define  CE	  Pin3	   // Chip Enable pin signal (output)
#define  CSN  Pin4	   // Slave Select pin
#define  SCK  Pin5
#define  MOSI Pin6
#define  MISO Pin7
#define  IRQ  Pin8	   // Interrupt signal,
sbit Pin3=P0^0;
sbit Pin4=P0^1;
sbit Pin5=P0^2;
sbit Pin6=P0^3;
sbit Pin7=P0^4;
sbit Pin8=P0^5;
//****************************************************************//
// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

#define TX_ADR_WIDTH    5   // 5 bytes TX address width
#define RX_ADR_WIDTH    5   // 5 bytes RX address width
#define TX_PLOAD_WIDTH  20  // 20 bytes TX payload
#define RX_PLOAD_WIDTH  20  // 20 bytes TX payload

uchar code RX_ADDRESS1[RX_ADR_WIDTH]= {0xb0,0x43,0x10,0x10,0x01};	//接收地址1
uchar code RX_ADDRESS2[1]= {0xb1};	//接收地址2
uchar code RX_ADDRESS3[1]= {0xb2};	//接收地址3
uchar code RX_ADDRESS4[1]= {0xb3};	//接收地址4
uchar code RX_ADDRESS5[1]= {0xb4};	//接收地址5

extern uchar const TX_ADDRESS[TX_ADR_WIDTH];//TX address
extern uchar const RX_ADDRESS[RX_ADR_WIDTH];//;RX address
uchar rx_buf[TX_PLOAD_WIDTH];
uchar tx_buf[TX_PLOAD_WIDTH];

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//***************************************************************//
//                   FUNCTION's PROTOTYPES  //
/****************************************************************/
 void SPI_Init(BYTE Mode);     // Init HW or SW SPI
 BYTE SPI_RW(BYTE byte);                                // Single SPI read/write
 BYTE SPI_Read(BYTE reg);                               // Read one byte from nRF24L01
 BYTE SPI_RW_Reg(BYTE reg, BYTE byte);                  // Write one byte to register 'reg'
 BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);  // Writes multiply bytes to one register
 BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);   // Read multiply bytes from one register
//*****************************************************************/

void init_io(void) ;

void SetRX_Mode(void);
uchar nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);

