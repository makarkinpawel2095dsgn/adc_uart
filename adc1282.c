#include "apm32f10x.h"
#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_spi.h"
#include "ADS1282.h"


void init_GPIO()
{
    // RCM APB2 clock periph
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_GPIOD); 
    GPIO_Config_T GPIO_Config_portD;
    // output pin 
    GPIO_Config_portD.mode  = GPIO_MODE_OUT_PP;
    GPIO_Config_portD.pin   = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_Config_portD.speed = GPIO_SPEED_10MHz;
    GPIO_Config(GPIOD, &GPIO_Config_portD);
    // input pin
    GPIO_Config_portD.mode  = GPIO_MODE_IN_PD;
    GPIO_Config_portD.pin   = GPIO_PIN_12 | GPIO_PIN_13; 
    GPIO_Config_portD.speed = GPIO_SPEED_10MHz;
    GPIO_Config(GPIOD, &GPIO_Config_portD);
}

void nPWDN_hith()
{
    // power down input low active
    GPIO_SetBit(GPIOD, GPIO_PIN_8);
}

void nPWDN_low()
{
    // power down input hith active
    GPIO_ResetBit(GPIOD, GPIO_PIN_8);
}

void nReset_low()
{
    // reset low
    GPIO_SetBit(GPIOD, GPIO_PIN_9);
}
void nReset_hith()
{
    // reset low
    GPIO_ResetBit(GPIOD, GPIO_PIN_9);
}


void CLK_low()
{
    // reset low
    GPIO_SetBit(GPIOD, GPIO_PIN_11);
}
void CLK_hith()
{
    // reset low
    GPIO_ResetBit(GPIOD, GPIO_PIN_11);
}

//  void spi init 
void spi_init()
{
    // init GPIO SPI
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_GPIOA );
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_SPI1 );
    GPIO_Config_T GPIO_Config_portA;
    // output sclk mosi
    GPIO_Config_portA.mode = GPIO_MODE_AF_PP;
    GPIO_Config_portA.pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_Config_portA.speed = GPIO_SPEED_10MHz;
    GPIO_Config(GPIOA, &GPIO_Config_portA);
    // input miso
    GPIO_Config_portA.mode = GPIO_MODE_IN_FLOATING;
    GPIO_Config_portA.pin = GPIO_PIN_6;
    GPIO_Config_portA.speed = GPIO_SPEED_10MHz;
    GPIO_Config(GPIOA, &GPIO_Config_portA);
    // init spi config
    SPI_Config_T SPI_ADC;
    SPI_ADC.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    SPI_ADC.mode = SPI_MODE_MASTER;
    SPI_ADC.length = SPI_DATA_LENGTH_8B;
    SPI_ADC.polarity = SPI_CLKPOL_LOW;
    SPI_ADC.phase = SPI_CLKPHA_1EDGE;
    SPI_ADC.nss = SPI_NSS_SOFT;
    SPI_ADC.baudrateDiv = SPI_BAUDRATE_DIV_256;
    SPI_ADC.firstBit = SPI_FIRSTBIT_MSB;
    SPI_ADC.crcPolynomial = 7;
    SPI_Config(SPI1, &SPI_ADC);
    SPI_Enable(SPI1);
}

void ADS1282_init (void)
{
    nPWDN_hith();
    nReset_low();
    CLK_hith();
    Delay(50);
    nReset_hith();
    Delay(500);
}

void spi_tx (SPI_T* spi, uint8_t data)
{
    while(!(spi -> STS & SPI_FLAG_TXBE ));
    spi -> DATA = data;
}

// write params
void config_ads (uint8_t* buff)
{
    // transmit SPI Stop data adc
    spi_tx(SPI1, SDATAC);
    Delay(100);
    //  write rigester 0x01h
    spi_tx(SPI1, WREG + 0x01); 
    Delay(100); 
    spi_tx(SPI1, 0x01); 
    Delay(100);     
    spi_tx(SPI1, buff[0]);
    Delay(100);
    //  write rigester 0x02h
    spi_tx(SPI1, WREG + 0x02); 
    Delay(100); 
    spi_tx(SPI1, 0x01); 
    Delay(100); 
    spi_tx(SPI1, buff[1]);
    Delay(100);
    // write rigester 0x03h
    spi_tx(SPI1, WREG + 0x03); 
    Delay(100);
    spi_tx(SPI1, 0x01); 
    Delay(100); 
    spi_tx(SPI1, buff[2]);
    Delay(100);
    // write rigester 0x04h
    spi_tx(SPI1, WREG + 0x04); 
    Delay(100);
    spi_tx(SPI1, 0x01); 
    Delay(100); 
    spi_tx(SPI1, buff[3]);
    Delay(100);
    //
    spi_tx(SPI1, RDATAC); 
}

