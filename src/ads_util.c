#include "ads_util.h"


#define _RESET_ASD() TM_SPI_Send(SPI1, 0x06)


// PA2 for DRDY
void InitDRDY(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
   
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    //TODO:Replace with actual speed
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    SYSCFG_EXTILineConfig(EXTI_Port_SourceGPIOA, EXTI_PinSource2);
    EXTI_InitStruct.EXTI_Line = EXTI_Line2;    
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;

    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);

}

void Init_Comm(void) {
    // reset and delay = 50 ms
    // configure CPOL = 0, CPHA = 1  
    /* | MOSI MISO SCK    |
     PA7  PA6  PA5
    */
    TM_SPI_Init(SPI1, TM_SPI_PinsPack_1);  
    //TM_SPI_Send(SPI1, );     
    
    

    // Init drdy interrupts 
    InitDRDY();
    // Delay for a minimum td
    //For now i'll try without delays
    // Send Reset (06h) (0000 011x)
    _RESET_ASD();
      
    // Delay for a minimum of 50 ns + 32*tclk


    // Write coniguration command (43h, 08h, 04h, 10h and 00h)
    

    // Read back all configurations with RREG 23h command

    // Send start/SYNC (08h) to start conversation in contionuous conversion mode

    // Delay for a minimum of td
    
    // READING
    //----LOOP----
    // Wait for DRDY transition low

    // Delay for a minimum of td

    // Send 23 SCLK rising edges to read out conversion data on DOUT/DRDY

    //Delay for a minimum td
    //---ENDLOOP

}

void Init_ADC(void) {


}


void Init_Regs(void){
    // WREG from 0 register 3 + 1 bits
    TM_SPI_Send(SPI1, 0x43);
    // 0 refister PGA disabled and bypassed
    TM_SPI_Send(SPI1, 0x01);
    // 1 register Set Continous mode 
    TM_SPI_Send(SPI1, 0x04);
    // 2 register 50-Hz rejection only
    TM_SPI_Send(SPI1, 0x20);
    // 3 register MOST USELESS REGISTER
    TM_SPI_Send(SPI1, 0x00);
}


void Deinit_ADC(void) {
    //Send the powerdown comman (02h) to stop conversions and put the device in power-down mode
    //Maybe deinit spi1
    TM_SPI_Send(SPI1, 0x02);
}
