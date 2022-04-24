
#include "board.h"

#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "app_button.h"
#include "nrf_drv_saadc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"



volatile Flag sys={0,0,0};

void in_pin_handler(uint8_t pin_no, uint8_t button_action);
static const app_button_cfg_t app_buttons[5] =
{
    {PIN_LEFT     , true , NRF_GPIO_PIN_NOPULL, in_pin_handler},
    {PIN_RIGHT    , false, NRF_GPIO_PIN_PULLUP, in_pin_handler},
    {PIN_MIDDLE   , false, NRF_GPIO_PIN_PULLUP, in_pin_handler},
    {PIN_FORWARD  , false, NRF_GPIO_PIN_PULLUP, in_pin_handler},
    {PIN_BACKWARD , false, NRF_GPIO_PIN_PULLUP, in_pin_handler},
};

void gpio_init(void);
void button_init(void);
void saadc_init(void);

void bsp_test()
{
    for(uint8_t i=1; i<=31; i++ )
    {
        if(i==9||i==10||i==21)
            continue;
        nrf_gpio_cfg_output( i );
    }

    while(1)
    {
        for(uint8_t i=1; i<=31; i++ )
        {
            if(i==9||i==10||i==21)
                continue;
            nrf_gpio_pin_toggle( i );
        }
        nrf_delay_ms(500);
    }
}

int8_t board_init(void)
{
    memset((void *)&sys, 0, sizeof(sys));

    //bsp_test();
    gpio_init();
    button_init();
    saadc_init();
    return 0;
}

// gpio init
void gpio_init(void)
{
    nrf_gpio_cfg_output( PIN_LED );
    nrf_gpio_pin_clear( PIN_LED );
}

// gpiote init
void in_pin_handler(uint8_t pin_no, uint8_t button_action)
{
    uint8_t buffer = 0;

    switch( pin_no )
    {
      case PIN_LEFT :
        NRF_LOG_INFO("gpiote pin LEFT event.%d",button_action);
        mouse_motion_send( button_action<<0 & 0x01 ) ;

      break;

      case PIN_RIGHT :
        NRF_LOG_INFO("gpiote pin RIGHT event.%d",button_action);


      break;

      case PIN_MIDDLE :
        NRF_LOG_INFO("gpiote pin MIDDLE event.%d",button_action);


      break;

      case PIN_FORWARD :
        NRF_LOG_INFO("gpiote pin FORWARD event.%d",button_action);


      break;

      case PIN_BACKWARD :
        NRF_LOG_INFO("gpiote pin BACKWARD event.");


      break;

      /*
        add event on gpiote inetrrupt
      */

      default:
      break;
    }
}

void button_init(void)
{
    uint32_t err_code = NRF_SUCCESS;

    err_code = app_button_init((app_button_cfg_t *)app_buttons,
                                  5,
                                  APP_TIMER_TICKS(10));
    APP_ERROR_CHECK(err_code);

    err_code = app_button_enable();
    APP_ERROR_CHECK(err_code);
}

void saadc_init(void)
{
    // initial saadc_init
    ret_code_t err_code = nrf_drv_saadc_init(NULL, NULL);
    APP_ERROR_CHECK(err_code);
    
    nrf_saadc_channel_config_t battery =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ADC_BATTERY);
    
    err_code = nrf_drv_saadc_channel_init(ADC_BATTERY_CH, &battery);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t joystick_x =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ADC_JOYSTICK_X);
    
    err_code = nrf_drv_saadc_channel_init(ADC_JOYSTICK_X_CH, &joystick_x);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t joystick_y =
    NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ADC_JOYSTICK_Y);
    
    err_code = nrf_drv_saadc_channel_init(ADC_JOYSTICK_Y_CH, &joystick_y);
    APP_ERROR_CHECK(err_code);

//    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
//    APP_ERROR_CHECK(err_code);
//
//    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
//    APP_ERROR_CHECK(err_code);
}


void check_bond(bool *erase_bonds)
{
    // check button
    uint8_t pin_stat = nrf_gpio_pin_read(PIN_LEFT);

    // set erase_bonds
    *erase_bonds = pin_stat;

#if _DEBUG_LOG
    // aleays clear bond
    *erase_bonds = true;
#endif
}

int16_t conver_to_XY( int16_t move )
{
    int16_t scale = 0 , sign = 0 ; 
    move += 50 ;
    move >>= 2 ;
    move -= 128 ;
    sign = (move>=0)? 1:-1 ;

    scale = abs( move );
    
    if( scale <= 20 )         return  0*sign ;
    else if( scale <= 40 )    return  2*sign ;
    else if( scale <= 60 )    return  4*sign ;
    else if( scale <= 80 )    return  6*sign ;
    else if( scale <= 100 )   return  8*sign ;
    else                      return 10*sign ;
}

int8_t sys_task(void)
{
    return 0;
}
