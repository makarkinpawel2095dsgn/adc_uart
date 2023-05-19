//include file
#include "apm32f10x.h"
#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_usart.h"
#include "ADS1282.h"
#include "tim_adc.h"

#define Rx_size 16
#define Tx_size 4



// couter delay 
void Delay(uint32_t delays)
{
    for (uint32_t i = 0; i<delays;i++)
	{
		i++;
	}
}

void init_LED(void)
{
    // enable port PE
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_GPIOE); 
    // config port PE
    GPIO_Config_T PORTE;
    PORTE.pin  = GPIO_PIN_5 | GPIO_PIN_6;
    PORTE.speed = GPIO_SPEED_10MHz;
    PORTE.mode = GPIO_MODE_OUT_PP;
    GPIO_Config(GPIOE, &PORTE);
}

void init_UART(void)
{
    // enable port PA
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_GPIOA );
    // USART PA
    RCM_EnableAPB2PeriphClock( RCM_APB2_PERIPH_USART1 );
    
    //
    GPIO_Config_T PORTA;
    GPIO_ConfigPinRemap(GPIO_REMAP_USART2);
    
    // port transmit 
    PORTA.pin = GPIO_PIN_9;
    PORTA.speed = GPIO_SPEED_10MHz;
    PORTA.mode = GPIO_MODE_AF_PP;
    GPIO_Config(GPIOA, &PORTA);
    
    // port recervie
    PORTA.pin = GPIO_PIN_10;
    PORTA.mode = GPIO_MODE_IN_FLOATING;
    //
    GPIO_Config(GPIOA, &PORTA);
}
void UART_config(void)
{
    // config uart
    USART_Config_T usartConfig;
    usartConfig.baudRate = 9600;
    usartConfig.wordLength = USART_WORD_LEN_8B;
    usartConfig.stopBits = USART_STOP_BIT_1;
    usartConfig.parity = USART_PARITY_NONE ;
    usartConfig.mode = USART_MODE_TX_RX;
    //USART_ConfigStructInit(&usartConfig);
    USART_Config(USART1, &usartConfig);
    USART_Enable(USART1);
    USART_EnableTx(USART1);
    USART_EnableRx(USART1);
}


// функция передачи данных по uart
void uart_tx (USART_T* usart, uint8_t data)
{
    while (!( usart -> STS & USART_FLAG_TXC));
    usart -> DATA = data;
}


//Функция отправляет строку в UART, по сути пересылая по байту в uart_tx
void send_str(USART_T* usart, char * string)
{
    uint8_t i=0;
    while(string[i]) 
    {
        uart_tx(usart, (uint8_t) string[i]);
        i++;
    }
}



int main(void)
{
    
    unsigned char Buff_rx[Rx_size-1];
    unsigned char Buff_tx[Tx_size-1];
    uint8_t buff_txx[3];
    uint8_t buff_rxx[3];
    uint8_t rx_byte;
    uint8_t rxx_byte;
    
    Buff_rx[1] = 0x00;
    Buff_rx[2] = 0x00;
    Buff_rx[3] = 0x00;
    Buff_rx[4] = 0x00;
    
    init_LED();
    init_UART();
    UART_config();
    spi_init();
    ADS1282_init();
    //config_ads(Buff_rx);
    send_str(USART1, "Hello amp32f103");
    buff_txx[0] = 0x52;
    buff_txx[1] = 0x08;
    buff_txx[2] = 0x32;
    buff_txx[3] = 0x03;
    config_ads (buff_txx);
    send_str(USART1, "init ADC1282");
    while(1)
    {
        if (SPI1 -> STS & SPI_FLAG_RXBNE)
        {
            buff_rxx[rxx_byte] = SPI1 ->  DATA;
            rxx_byte++;
            if (rxx_byte == 3)
            {
                rxx_byte = 0;
            }
        }
              
        send_str(USART1, buff_rxx);
               
         /*
        spi_tx(SPI1, 0x55);
        Delay(50000);
        spi_tx(SPI1, 0xAA);
        Delay(50000);
        */
        
        
        /*
        if (USART1 -> STS & USART_FLAG_RXBNE)
        {
            Buff_rx[rx_byte] = USART1 -> DATA; 
            rx_byte++;
        }
        if (rx_byte == Rx_size-1)
        {
            rx_byte = 0;
            send_str(USART1, Buff_rx);
        }
        /*-----------------------------------*//*
        Buff_rx[0] = 0x55;
        buff_rx[1] = ID -> Read register
        buff_rx[2] = CONFIG0;
        buff_rx[3] = CONFIG1;
        buff_rx[4] = HPF0;
        buff_rx[5] = HPF1;
        buff_rx[6] = OFC0;
        buff_rx[7] = OFC1;
        buff_rx[8] = OFC2;
        buff_rx[9] = FSC0;
        buff_rx[10] = FSC1;
        buff_rx[11] = FSC2;
        buff_rx[12] = NULL;
        buff_rx[13, 14] = CRC
        Buff_rx[15] = 0xAA;
        *//*-----------------------------------*/
        /*
        if (Buff_rx[0] == 0x55 & Buff_rx[15] == 0xAA)
        {
            spi_tx(SPI1, 0x55);
        }
        //
        /*
        // rx_buff -> tx_biff
        if (rx_byte == Rx_size)
        {
            send_str(USART1, Buff_rx);
            rx_byte = 0;
        }
        */
        
        /*
     
        
        GPIO_SetBit(GPIOE, GPIO_PIN_5);
        GPIO_ResetBit(GPIOE, GPIO_PIN_6);
        
        Delay(5000000);
        
        GPIO_SetBit(GPIOE, GPIO_PIN_6);
        GPIO_ResetBit(GPIOE, GPIO_PIN_5);
        Delay(5000000);
        */
    }
}