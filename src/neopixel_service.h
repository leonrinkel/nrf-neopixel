#ifndef _NEOPIXEL_SERVICE_H
#define _NEOPIXEL_SERVICE_H

#include <stdint.h>

#include "ble.h"
#include "ble_srv_common.h"

#define NEOPIXEL_SERVICE_UUID_BASE \
    { \
        0x0c, 0x89, 0x93, 0xb9, 0x4c, 0x87, 0x1c, 0xad, \
        0x2e, 0xfb, 0xb6, 0x94, 0x1e, 0xad, 0x59, 0x61 \
    }

#define NEOPIXEL_SERVICE_UUID    0x4200
#define NEOPIXEL_VALUE_CHAR_UUID 0x4201

#define BLE_NEOPIXEL_DEF(_name) \
    static ble_neopixel_t _name; \
    NRF_SDH_BLE_OBSERVER(_name ## _obs, BLE_HRS_BLE_OBSERVER_PRIO, \
        ble_neopixel_on_ble_evt, &_name)

typedef struct
{
    uint8_t                      initial_custom_value;      /**< Initial custom value */
    ble_srv_cccd_security_mode_t custom_value_char_attr_md; /**< Initial security level for Custom characteristics attribute */
} ble_neopixel_init_t;

struct ble_neopixel_s
{
    uint16_t                 service_handle;       /**< Handle of Custom Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t custom_value_handles; /**< Handles related to the Custom Value characteristic. */
    uint16_t                 conn_handle;          /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t                  uuid_type;
};

typedef struct ble_neopixel_s ble_neopixel_t;

uint32_t ble_neopixel_init(ble_neopixel_t* service, const ble_neopixel_init_t* service_init);

void ble_neopixel_on_ble_evt(ble_evt_t const* ble_evt, void* context);

#endif /* _NEOPIXEL_SERVICE_H */
