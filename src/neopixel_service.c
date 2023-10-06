#include "neopixel_service.h"

#include <string.h>

#include "sdk_common.h"
#include "ble_srv_common.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"

#include "neopixel_driver.h"

static neopixel_t neopixel;

static uint32_t custom_value_char_add(
    ble_neopixel_t* service, const ble_neopixel_init_t* service_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md = {0};
    ble_gatts_attr_md_t attr_md = {0};
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_t    attr_char_value = {0};

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    attr_md.read_perm  = service_init->custom_value_char_attr_md.read_perm;
    attr_md.write_perm = service_init->custom_value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = service->uuid_type;
    ble_uuid.uuid = NEOPIXEL_VALUE_CHAR_UUID;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 3;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 3;

    err_code = sd_ble_gatts_characteristic_add(
        service->service_handle, &char_md, &attr_char_value,
        &service->custom_value_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

uint32_t ble_neopixel_init(
    ble_neopixel_t* service, const ble_neopixel_init_t* service_init)
{
    if (service == NULL || service_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    service->conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add Custom Service UUID
    ble_uuid128_t base_uuid = { NEOPIXEL_SERVICE_UUID_BASE };
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = service->uuid_type;
    ble_uuid.uuid = NEOPIXEL_SERVICE_UUID;

    // Add the Custom Service
    err_code = sd_ble_gatts_service_add(
        BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &service->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Custom Value characteristic
    err_code = custom_value_char_add(service, service_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    neopixel_conf_t neopixel_conf = {
        .num_leds     = 64,
        .output_pin   = 6,
        .pwm_instance = NRF_DRV_PWM_INSTANCE(0)
    };
    err_code = neopixel_init(&neopixel, &neopixel_conf);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    //neopixel.color = 0xff0000;
    //err_code = neopixel_show(&neopixel);
    //if (err_code != NRF_SUCCESS)
    //{
    //    return err_code;
    //}

    return NRF_SUCCESS;
}

static void on_write(ble_neopixel_t* service, ble_evt_t const* ble_evt)
{
    const ble_gatts_evt_write_t* evt_write = &ble_evt->evt.gatts_evt.params.write;

    // Check if the handle passed with the event matches the Custom Value Characteristic handle.
    if (
        evt_write->handle == service->custom_value_handles.value_handle &&
        evt_write->len == 3
    )
    {
        neopixel.color =
            (evt_write->data[2] <<  0) |
            (evt_write->data[1] <<  8) |
            (evt_write->data[0] << 16);
        neopixel_show(&neopixel);
    }
}

void ble_neopixel_on_ble_evt(ble_evt_t const* ble_evt, void* context)
{
    ble_neopixel_t* service = (ble_neopixel_t *) context;

    if (service == NULL || ble_evt == NULL)
    {
        return;
    }

    switch (ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            //neopixel.color = 0x00ff00;
            //neopixel_show(&neopixel);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            //neopixel.color = 0xff0000;
            //neopixel_show(&neopixel);
            break;

        case BLE_GATTS_EVT_WRITE:
           on_write(service, ble_evt);
           break;

        default:
            // No implementation needed.
            break;
    }
}
