// SPDX-License-Identifier: MIT
// Copyright (c) 2018 Laurens Valk

#include <pbio/servo.h>

#include "py/obj.h"

// Class structure for Motors
typedef struct _motor_Motor_obj_t {
    mp_obj_base_t base;
    pbio_servo_t *mtr;
} motor_Motor_obj_t;

const mp_obj_type_t motor_Motor_type;
