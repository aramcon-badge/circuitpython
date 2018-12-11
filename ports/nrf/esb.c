/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Uri Shaked
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <string.h>
#include "py/mphal.h"
#include "py/obj.h"
#include "py/runtime.h"

#include "nrf_esb.h"
#include "nrf_error.h"

static nrf_esb_payload_t rx_payload;
static nrf_esb_payload_t tx_payload = NRF_ESB_CREATE_PAYLOAD(0);
static bool data_available = false;

void nrf_esb_event_handler(nrf_esb_evt_t const * p_event)
{
    switch (p_event->evt_id)
    {
        case NRF_ESB_EVENT_TX_SUCCESS:
            printf("TX SUCCESS EVENT\n");
            break;
        case NRF_ESB_EVENT_TX_FAILED:
            printf("TX FAILED EVENT\n");
            break;
        case NRF_ESB_EVENT_RX_RECEIVED:
            printf("RX RECEIVED EVENT\n");
            data_available = true;
            break;
    }
}

STATIC mp_obj_t esb_init_rx( void )
{
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };
    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
    nrf_esb_config.payload_length           = 8;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
    nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
    nrf_esb_config.selective_auto_ack       = false;

    printf("Calling nrf_esb_init for RX!\n");

    err_code = nrf_esb_init(&nrf_esb_config);
    printf("Result: %lu\n", err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    printf("Result: %lu\n", err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    printf("Result: %lu\n", err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
    printf("Result: %lu\n", err_code);

	printf("Start rx: %lu\n", nrf_esb_start_rx());

    return mp_const_none;
}


STATIC mp_obj_t esb_init_tx( void )
{
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.retransmit_delay         = 600;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
    nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;
    nrf_esb_config.selective_auto_ack       = false;

    printf("Calling nrf_esb_init for TX!\n");

    err_code = nrf_esb_init(&nrf_esb_config);
    printf("Result: %lu\n", err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    printf("Result: %lu\n", err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    printf("Result: %lu\n", err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, NRF_ESB_PIPE_COUNT);
    printf("Result: %lu\n", err_code);

    return mp_const_none;
}

STATIC mp_obj_t esb_send(mp_obj_t tx_buf) {
    mp_buffer_info_t src;
    mp_get_buffer_raise(tx_buf, &src, MP_BUFFER_READ);
    if (src.len == 0 || src.len > NRF_ESB_MAX_PAYLOAD_LENGTH) {
        return mp_const_none;
    }
    tx_payload.length = src.len;
    memcpy(tx_payload.data, src.buf, src.len);
    printf("Sending %d bytes\n", src.len);
    printf("Result: %lu\n", nrf_esb_write_payload(&tx_payload));
    return mp_const_none;
}

STATIC mp_obj_t esb_recv(void) {
    if (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS) {
        return mp_obj_new_bytes(rx_payload.data, rx_payload.length);
    }

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(esb_init_rx_obj, esb_init_rx);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(esb_init_tx_obj, esb_init_tx);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(esb_send_obj, esb_send);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(esb_recv_obj, esb_recv);

STATIC const mp_map_elem_t esb_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_esb) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_init_rx), (mp_obj_t)&esb_init_rx_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_init_tx), (mp_obj_t)&esb_init_tx_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send), (mp_obj_t)&esb_send_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv), (mp_obj_t)&esb_recv_obj },
};

STATIC MP_DEFINE_CONST_DICT (
    mp_module_esb_globals,
    esb_globals_table
);

const mp_obj_module_t mp_module_esb = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_esb_globals,
};
