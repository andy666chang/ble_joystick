#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "app_timer.h"
#include "nrf_drv_saadc.h"

#define _DEBUG_LOG 0

typedef struct Flag
{
    uint8_t ble_conn    :1 ;
    uint8_t ble_disconn :1 ;
    uint8_t joystick_xy :1 ;   
    uint8_t reserve     :5 ;
}Flag;

extern volatile Flag sys ;

// adc init
// ADC
#define ADC_VDD NRF_SAADC_INPUT_VDD     // VDD
#define ADC_0   NRF_SAADC_INPUT_AIN0    // P0.02
#define ADC_1   NRF_SAADC_INPUT_AIN1    // P0.03
#define ADC_2   NRF_SAADC_INPUT_AIN2    // P0.04
#define ADC_3   NRF_SAADC_INPUT_AIN3    // P0.05
#define ADC_4   NRF_SAADC_INPUT_AIN4    // P0.28
#define ADC_5   NRF_SAADC_INPUT_AIN5    // P0.29
#define ADC_6   NRF_SAADC_INPUT_AIN6    // P0.30
#define ADC_7   NRF_SAADC_INPUT_AIN7    // P0.31

#define ADC_BATTERY     ADC_2
#define ADC_JOYSTICK_X  ADC_0
#define ADC_JOYSTICK_Y  ADC_1

#define ADC_BATTERY_CH     0
#define ADC_JOYSTICK_X_CH  1
#define ADC_JOYSTICK_Y_CH  2

#define PIN_LEFT      31
#define PIN_RIGHT     30
#define PIN_MIDDLE    29
#define PIN_FORWARD   28
#define PIN_BACKWARD  27

#define PIN_LED 17

APP_TIMER_DEF(m_joystick_timer_id);                                                  /**< joystick timer. */
#define JOYSTICK_MEAS_INTERVAL          APP_TIMER_TICKS(20)                       /**< JOYSTICK measurement interval (ticks). */

static void (*mouse_movement_send_xy)(int16_t x_delta, int16_t y_delta);
static void (*mouse_motion_send_button)( uint8_t button );

int8_t board_init(void);
void check_bond(bool *erase_bonds);

int8_t sys_task(void);

int16_t conver_to_XY( int16_t move );

#endif