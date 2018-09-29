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
#include <openthread/cli.h>
#include <openthread/instance.h>
#include <openthread/platform/openthread-system.h>

#include "py/mphal.h"
#include "py/obj.h"
#include "py/runtime.h"

static otInstance *mp_ot_instance;

STATIC mp_obj_t mesh_init(void) {
    otSysInit(0, NULL);
    mp_ot_instance = otInstanceInitSingle();
    // TODO check for NULL

    otCliUartInit(mp_ot_instance);
    return MP_OBJ_NEW_SMALL_INT(0);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mesh_init_obj, mesh_init);

STATIC const mp_map_elem_t mesh_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_mesh) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_init), (mp_obj_t)&mesh_init_obj },
};

STATIC MP_DEFINE_CONST_DICT (
    mp_module_mesh_globals,
    mesh_globals_table
);

const mp_obj_module_t mp_module_mesh = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_mesh_globals,
};

// Junk - just to make it compile...

void SEGGER_RTT_ConfigUpBuffer() {}
void SEGGER_RTT_WriteNoLock() {}
int __start_ot_flash_data = 0;
int __stop_ot_flash_data = 0;
