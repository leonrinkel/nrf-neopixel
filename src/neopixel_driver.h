#ifndef _NEOPIXEL_DRIVER_H
#define _NEOPIXEL_DRIVER_H

#include <stdint.h>

#include "sdk_common.h"
#include "nrf_drv_pwm.h"

typedef struct
{
    uint8_t       num_leds;
    nrf_drv_pwm_t pwm_instance;
    uint16_t*     pwm_sequence_values;
    uint32_t      color;
} neopixel_t;

typedef struct
{
    uint8_t       num_leds;
    uint8_t       output_pin;
    nrf_drv_pwm_t pwm_instance;
} neopixel_conf_t;

ret_code_t neopixel_init
(
    neopixel_t*      neopixel,
    neopixel_conf_t* neopixel_conf
);

ret_code_t neopixel_show(neopixel_t* neopixel);

#endif /* _NEOPIXEL_DRIVER_H */
