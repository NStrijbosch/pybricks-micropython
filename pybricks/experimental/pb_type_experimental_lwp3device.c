// SPDX-License-Identifier: MIT
// Copyright (c) 2021 The Pybricks Authors

#include "py/mpconfig.h"

#if PYBRICKS_PY_EXPERIMENTAL

#include <stdint.h>

#include <lego_lwp3.h>

#include <pbdrv/bluetooth.h>
#include <pbio/button.h>
#include <pbio/color.h>
#include <pbio/error.h>
#include <pbio/task.h>

#include <pybricks/common.h>
#include <pybricks/parameters.h>
#include <pybricks/util_mp/pb_kwarg_helper.h>
#include <pybricks/util_mp/pb_obj_helper.h>
#include <pybricks/util_pb/pb_error.h>
#include <pybricks/util_pb/pb_task.h>

#include "py/mphal.h"
#include "py/runtime.h"
#include "py/obj.h"
#include "py/mperrno.h"

typedef struct {
    pbio_task_t task;
    pbdrv_bluetooth_scan_and_connect_context_t context;
} pb_lwp3device_t;

STATIC pb_lwp3device_t pb_lwp3device_singleton;

// Handles LEGO Wireless protocol messages from the LWP3 Device
STATIC void handle_notification(pbdrv_bluetooth_connection_t connection, const uint8_t *value, uint8_t size) {
    // TODO: include notification handler: store all incoming messages in finite lenght buffer
}

STATIC void lwp3device_connect(const lwp3_hub_kind_t hub_kind, const char *name, mp_int_t timeout) {
    pb_lwp3device_t *lwp3device = &pb_lwp3device_singleton;

    // REVISIT: for now, we only allow a single connection to a LWP3 device.
    if (pbdrv_bluetooth_is_connected(PBDRV_BLUETOOTH_CONNECTION_PERIPHERAL_LWP3)) {
        pb_assert(PBIO_ERROR_BUSY);
    }

    // needed to ensure that no buttons are "pressed" after reconnecting since
    // we are using static memory
    memset(lwp3device, 0, sizeof(*lwp3device));

    lwp3device->context.hub_kind = hub_kind;

    if (name) {
        strncpy(lwp3device->context.name, name, sizeof(lwp3device->context.name));
    }

    pbdrv_bluetooth_set_notification_handler(handle_notification);
    pbdrv_bluetooth_scan_and_connect(&lwp3device->task, &lwp3device->context);
    pb_wait_task(&lwp3device->task, timeout);
}

void pb_type_LWP3Device_cleanup(void) {
    pbdrv_bluetooth_disconnect_remote();
}

STATIC void lwp3device_assert_connected(void) {
    if (!pbdrv_bluetooth_is_connected(PBDRV_BLUETOOTH_CONNECTION_PERIPHERAL_LWP3)) {
        mp_raise_OSError(MP_ENODEV);
    }
}

typedef struct _pb_type_experimental_LWP3Device_obj_t {
    mp_obj_base_t base;
} pb_type_experimental_LWP3Device_obj_t;

STATIC mp_obj_t pb_type_experimental_LWP3Device_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    PB_PARSE_ARGS_CLASS(n_args, n_kw, args,
        PB_ARG_REQUIRED(hub_kind),
        PB_ARG_DEFAULT_NONE(name),
        PB_ARG_DEFAULT_INT(timeout, 10000));

    pb_type_experimental_LWP3Device_obj_t *self = m_new_obj(pb_type_experimental_LWP3Device_obj_t);
    self->base.type = (mp_obj_type_t *)type;

    pbio_port_id_t hub_kind = pb_type_enum_get_value(hub_kind_in, &pb_enum_type_LWP3);

    const char *name = name_in == mp_const_none ? NULL : mp_obj_str_get_str(name_in);
    mp_int_t timeout = timeout_in == mp_const_none ? -1 : pb_obj_get_positive_int(timeout_in);
    lwp3device_connect(hub_kind, name, timeout);

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t lwp3device_name(size_t n_args, const mp_obj_t *args) {
    pb_lwp3device_t *lwp3device = &pb_lwp3device_singleton;

    lwp3device_assert_connected();

    if (n_args == 2) {
        size_t len;
        const char *name = mp_obj_str_get_data(args[1], &len);

        if (len == 0 || len > LWP3_MAX_HUB_PROPERTY_NAME_SIZE) {
            mp_raise_ValueError(MP_ERROR_TEXT("bad name length"));
        }

        struct {
            pbdrv_bluetooth_value_t value;
            uint8_t length;
            uint8_t hub;
            uint8_t type;
            uint8_t property;
            uint8_t operation;
            char payload[LWP3_MAX_HUB_PROPERTY_NAME_SIZE];
        } __attribute__((packed)) msg;

        msg.value.size = msg.length = len + 5;
        msg.hub = 0;
        msg.type = LWP3_MSG_TYPE_HUB_PROPERTIES;
        msg.property = LWP3_HUB_PROPERTY_NAME;
        msg.operation = LWP3_HUB_PROPERTY_OP_SET;
        memcpy(msg.payload, name, len);

        // NB: operation is not cancelable, so timeout is not used
        pbdrv_bluetooth_write_remote(&lwp3device->task, &msg.value);
        pb_wait_task(&lwp3device->task, -1);

        // assuming write was successful instead of reading back from the handset
        memcpy(lwp3device->context.name, name, len);
        lwp3device->context.name[len] = 0;

        return mp_const_none;
    }

    return mp_obj_new_str(lwp3device->context.name, strlen(lwp3device->context.name));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lwp3device_name_obj, 1, 2, lwp3device_name);

STATIC const mp_rom_map_elem_t pb_type_experimental_LWP3Device_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_name), MP_ROM_PTR(&lwp3device_name_obj) },
};
STATIC MP_DEFINE_CONST_DICT(pb_type_experimental_LWP3Device_locals_dict, pb_type_experimental_LWP3Device_locals_dict_table);

const mp_obj_type_t pb_type_experimental_LWP3Device = {
    { &mp_type_type },
    .name = PYBRICKS_HUB_CLASS_NAME,
    .make_new = pb_type_experimental_LWP3Device_make_new,
    .locals_dict = (mp_obj_dict_t *)&pb_type_experimental_LWP3Device_locals_dict,
};

#endif // PYBRICKS_PY_EXPERIMENTAL