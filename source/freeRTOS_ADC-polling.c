/**
 * @autor   Jesus Rojas
 * @file    Practica2_ADC.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_adc16.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
int signal_1[] = {0, 0, 0, 0, 0};
int signal_2[] = {0, 0, 0, 0, 0};
int signal_3[] = {0, 0, 0, 0, 0};

int index_1,
	index_2 = 0,
	index_3 = 0;

int prom1;
int prom2;

adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct_2;
adc16_channel_config_t adc16ChannelConfigStruct_3;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* TODO: insert other definitions and declarations here. */
void Task_ADC_polling (void *arg){
	index_1 = 0;
	prom1 = 0;

    while(1){

    	   ADC16_SetChannelConfig(ADC0, 0U, &adc16ChannelConfigStruct);
           while (0U == (kADC16_ChannelConversionDoneFlag &
                         ADC16_GetChannelStatusFlags(ADC0, 0U)))
           {
           }

           signal_1[index_1] = ADC16_GetChannelConversionValue(ADC0, 0U);

           //PRINTF("%d\t%d\r\n",index_1, signal_1[index_1]);

           if (index_1 == 4){
        	   for (index_1 = 0; index_1 < 4; ++index_1) {
        		   prom1 = (signal_1[index_1] + prom1);
        		   //PRINTF("%d\t%d\r\n",index_1, signal_1[index_1]);
        	   }
        	   //PRINTF("1: %d\r\n", index_1);
        	   PRINTF("1: %d\r\n", prom1/5);
           }
           prom1 = 0;
           index_1 = (index_1+1)%5;
        vTaskDelay(30);
	}
}

void Task_ADC_polling_2 (void *arg){

    while(1){
        ADC16_SetChannelConfig(ADC0, 0U, &adc16ChannelConfigStruct_2);
           while (0U == (kADC16_ChannelConversionDoneFlag &
                         ADC16_GetChannelStatusFlags(ADC0, 0U)))
           {
           }
           PRINTF("2: %d\r\n", ADC16_GetChannelConversionValue(ADC0, 0U));
           vTaskDelay(60);
	}
}

void Task_ADC_polling_3 (void *arg){

	while(1){
        ADC16_SetChannelConfig(ADC0, 0U, &adc16ChannelConfigStruct_3);
           while (0U == (kADC16_ChannelConversionDoneFlag &
                         ADC16_GetChannelStatusFlags(ADC0, 0U)))
           {
           }

           signal_2[index_2] = ADC16_GetChannelConversionValue(ADC0, 0U);

        	   for (index_3 = 0; index_3 < 5; ++index_3) {
               prom2 = (signal_2[index_3] + prom2);
               }

        	   PRINTF("3: %d\r\n", prom2/5);

           prom2 = 0;
           index_2 = (index_2+1)%5;
           vTaskDelay(30);
	}
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC0, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC0, false); /* Make sure the software trigger is used. */
    ADC16_DoAutoCalibration(ADC0);

    adc16ChannelConfigStruct.channelNumber = 0U;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;

    adc16ChannelConfigStruct_2.channelNumber = 8U;
    adc16ChannelConfigStruct_2.enableInterruptOnConversionCompleted = false;

    adc16ChannelConfigStruct_3.channelNumber = 9U;
    adc16ChannelConfigStruct_3.enableInterruptOnConversionCompleted = false;

    #if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
        adc16ChannelConfigStruct.enableDifferentialConversion = false;
        adc16ChannelConfigStruct_2.enableDifferentialConversion = false;
        adc16ChannelConfigStruct_3.enableDifferentialConversion = false;
    #endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */


    PRINTF("Hello World\r\n");
    //xTaskCreate(Task_ADC_polling, "my_task", configMINIMAL_STACK_SIZE + 10, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(Task_ADC_polling_2, "my_task_2", configMINIMAL_STACK_SIZE + 10, NULL, configMAX_PRIORITIES - 1, NULL);
    //xTaskCreate(Task_ADC_polling_3, "my_task_3", configMINIMAL_STACK_SIZE + 10, NULL, configMAX_PRIORITIES - 1, NULL);

    /* Force the counter to be placed into memory. */
    vTaskStartScheduler();

    return 0 ;
}
