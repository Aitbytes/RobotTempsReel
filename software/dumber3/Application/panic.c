/*
 * panic.c
 *
 *  Created on: Oct 11, 2023
 *      Author: dimercur
 */

#include "application.h"
#include "timers.h"

#include "panic.h"
#include "leds.h"

void PANIC_StopTasksAndWait(void);
void MOTORS_PowerOff(void);

extern TaskHandle_t xHandleLedsHandler;
extern TaskHandle_t xHandleLedsAction;
extern TaskHandle_t xHandleBattery;
extern TimerHandle_t xHandleTimerButton;
extern TaskHandle_t xHandleApplicationMain;
extern TimerHandle_t xHandleTimerTimeout;
extern TaskHandle_t xHandleMotors;
extern TaskHandle_t xHandleMotorsControl;
extern TaskHandle_t xHandleXbeeTXHandler;
extern TaskHandle_t xHandleXbeeRX;

void PANIC_Raise(PANIC_Typedef panicId) {
	switch (panicId) {
	case panic_adc_err:
		LEDS_Set(leds_error_1);
		break;
	case panic_charger_err:
		LEDS_Set(leds_error_2);
		break;
	case panic_malloc:
		LEDS_Set(leds_error_3);
		break;
	default:
		LEDS_Set(leds_error_5);
		break;
	}

	PANIC_StopTasksAndWait();
}

void PANIC_StopTasksAndWait(void){
	TaskHandle_t currentTask;
	currentTask = xTaskGetCurrentTaskHandle();

	/* Arret des timers */
	//xTimerStop(xHandleTimerButton,0);
	//xTimerStop(xHandleTimerTimeout,0);

	/* Arret des taches
	 * On n'arrete toute les taches sauf celle en cours !
	 *
	 * a la fin, on arrete la tache en cours
	 *
	 * on n'arrete jamais les leds, sinon plus d'animation !!
	 */

	if (currentTask != xHandleXbeeRX)
		vTaskSuspend(xHandleXbeeRX);

	if (currentTask != xHandleXbeeTXHandler)
			vTaskSuspend(xHandleXbeeTXHandler);

	if (currentTask != xHandleMotors)
			vTaskSuspend(xHandleMotors);

	if (currentTask != xHandleMotorsControl)
			vTaskSuspend(xHandleMotorsControl);

	if (currentTask != xHandleBattery)
			vTaskSuspend(xHandleBattery);

	if (currentTask != xHandleApplicationMain)
			vTaskSuspend(xHandleApplicationMain);


	/* Stop des alim moteurs */
	MOTORS_PowerOff();

	/* disable XBEE */
	HAL_GPIO_WritePin(XBEE_RESET_GPIO_Port, XBEE_RESET_Pin, GPIO_PIN_RESET);

	/* Stop la tache courante */
	vTaskSuspend(currentTask);

	while (1) {
		__WFE(); /* Attente infinie */
	}
}