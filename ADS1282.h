#include "apm32f10x_spi.h"
#include <stdint.h>

/* --------------------------------*/
/* -----Register ADS1282-----------*/
#define ADS1282_REG_ID              0x00 // ID read register
#define ADS1282_REG_CONFIG0         0x01
#define ADS1282_REG_CONFIG1         0x02
#define ADS1282_REG_HPF0            0x03
#define ADS1282_REG_HPF1            0x04
#define ADS1282_REG_OFC0            0x05
#define ADS1282_REG_OFC1            0x06
#define ADS1282_REG_OFC2            0x07
#define ADS1282_REG_FSC0            0x08
#define ADS1282_REG_FSC1            0x09
#define ADS1282_REG_FSC2            0x0A

/* --------------------------------*/
/* -------Comand ADS1282-----------*/
#define WAKEUP                      0x00
#define STANDBY                     0x02
#define SYNC                        0x04
#define RESET                       0x06
#define RDATAC                      0x10
#define SDATAC                      0x11
#define RDATA                       0x12
#define RREG                        0x20
#define WREG                        0x40
#define OFSCAL                      0x60
#define GANCAL                      0x61

/* --------------------------------*/
/* ------Register Config0----------*/
#define ADS1282_CONFIG0_bypass          0x00
#define ADS1282_CONFIG0_Sinc            0x01
#define ADS1282_CONFIG0_Sinc_LPF        0x02
#define ADS1282_CONFIG0_Sinc_LPF_HPF    0x03
#define ADS1282_CONFIG0_Line_phase      0x00
#define ADS1282_CONFIG0_Min_phase       0x04
#define ADS1282_CONFIG0_250sps          0x00
#define ADS1282_CONFIG0_500sps          0x08
#define ADS1282_CONFIG0_1000sps         0x10
#define ADS1282_CONFIG0_2000sps         0x18
#define ADS1282_CONFIG0_4000sps         0x20
#define ADS1282_CONFIG0_Pulse_SYNC_mode 0x00
#define ADS1282_CONFIG0_Contin_SYNC_mod 0x80

/* --------------------------------*/
/* ------Register Config1----------*/
#define ADS1282_CONFIG0_PGA_G1          0x00
#define ADS1282_CONFIG0_PGA_G2          0x01
#define ADS1282_CONFIG0_PGA_G4          0x02
#define ADS1282_CONFIG0_PGA_G8          0x03
#define ADS1282_CONFIG0_PGA_G16         0x04
#define ADS1282_CONFIG0_PGA_G32         0x05
#define ADS1282_CONFIG0_PGA_G64         0x06
#define ADS1282_CONFIG0_PGA_dis         0x00
#define ADS1282_CONFIG0_PGA_en          0x08
#define ADS1282_CONFIG1_MUX_AINP1_AINN1       0x00
#define ADS1282_CONFIG1_MUX_AINP2_AINN2       0x10
#define ADS1282_CONFIG1_MUX_INT_SHORT_400     0x20
#define ADS1282_CONFIG1_MUX_AINP12_AINN12     0x30
#define ADS1282_CONFIG1_MUX_EXT_SHORT_AINN2   0x40

void spi_init();
void ADS1282_init (void);
void spi_tx (SPI_T* spi, uint8_t data);
void config_ads (uint8_t buff[]);
