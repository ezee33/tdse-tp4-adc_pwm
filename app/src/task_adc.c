/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_adc.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"

/********************** macros and definitions *******************************/


/********************** external data declaration ****************************/
extern ADC_HandleTypeDef hadc1;

/********************** internal data declaration ****************************/
static uint32_t tickstart;
static bool b_trig_new_conversion;
adc_valor_t adc_value;

/********************** internal functions declaration ***********************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void renew_and_wait(void);

/********************** internal data definition *****************************/
const char *p_task_adc 		= "Task ADC";

/********************** external functions definition ************************/
void task_adc_init(void) {
	// Configuración de interrupciones del ADC
	HAL_NVIC_SetPriority(ADC1_2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

	adc_value.value = 0;

    	b_trig_new_conversion = true; // Inicializa la primera conversión

    	tickstart = HAL_GetTick();
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_adc_init), p_task_adc);
}

void task_adc_update() {
    renew_and_wait();
}

void renew_and_wait() {
    // Inicia la conversión si se permite
    if (b_trig_new_conversion) {
       	b_trig_new_conversion = false;
        HAL_ADC_Start_IT(&hadc1);
    }
    HAL_Delay(5);
    //LOGGER_LOG("Valor de la muestra: %u\n", adc_value.value);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    uint16_t hold_value = HAL_ADC_GetValue(&hadc1);
	adc_value.value = hold_value;
	b_trig_new_conversion = true;
}

/********************** end of file ******************************************/
