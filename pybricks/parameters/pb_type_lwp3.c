// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2020 The Pybricks Authors

#include "py/mpconfig.h"

#if PYBRICKS_PY_PARAMETERS

#include <lego_lwp3.h>

#include <pybricks/parameters.h>

#include <pybricks/util_mp/pb_type_enum.h>

const pb_obj_enum_member_t pb_LWP3_DUPLO_TRAIN_obj = {
    {&pb_enum_type_LWP3},
    .name = MP_QSTR_DUPLO_TRAIN,
    .value = LWP3_HUB_KIND_DUPLO_TRAIN
};

const pb_obj_enum_member_t pb_LWP3_MOVE_HUB_obj = {
    {&pb_enum_type_LWP3},
    .name = MP_QSTR_MOVE_HUB,
    .value = LWP3_HUB_KIND_BOOST
};

const pb_obj_enum_member_t pb_LWP3_CITY_HUB_obj = {
    {&pb_enum_type_LWP3},
    .name = MP_QSTR_CITY_HUB,
    .value = LWP3_HUB_KIND_SYSTEM_2IO
};

const pb_obj_enum_member_t pb_LWP3_REMOTE_obj = {
    {&pb_enum_type_LWP3},
    .name = MP_QSTR_REMOTE,
    .value = LWP3_HUB_KIND_HANDSET
};

const pb_obj_enum_member_t pb_LWP3_MARIO_obj = {
    {&pb_enum_type_LWP3},
    .name = MP_QSTR_MARIO,
    .value = LWP3_HUB_KIND_MARIO
};

const pb_obj_enum_member_t pb_LWP3_TECHNIC_HUB_obj = {
    {&pb_enum_type_LWP3},
    .name = MP_QSTR_TECHNIC_HUB,
    .value = LWP3_HUB_KIND_TECHNIC_MEDIUM
};

STATIC const mp_rom_map_elem_t pb_enum_LWP3_table[] = {
    { MP_ROM_QSTR(MP_QSTR_DUPLO_TRAIN),   MP_ROM_PTR(&pb_LWP3_DUPLO_TRAIN_obj)  },
    { MP_ROM_QSTR(MP_QSTR_MOVE_HUB),      MP_ROM_PTR(&pb_LWP3_MOVE_HUB_obj)     },
    { MP_ROM_QSTR(MP_QSTR_CITY_HUB),      MP_ROM_PTR(&pb_LWP3_CITY_HUB_obj)     },
    { MP_ROM_QSTR(MP_QSTR_REMOTE),        MP_ROM_PTR(&pb_LWP3_REMOTE_obj)       },
    { MP_ROM_QSTR(MP_QSTR_MARIO),         MP_ROM_PTR(&pb_LWP3_MARIO_obj)        },
    { MP_ROM_QSTR(MP_QSTR_TECHNIC_HUB),   MP_ROM_PTR(&pb_LWP3_TECHNIC_HUB_obj)  },
};
STATIC MP_DEFINE_CONST_DICT(pb_enum_type_LWP3_locals_dict, pb_enum_LWP3_table);

const mp_obj_type_t pb_enum_type_LWP3 = {
    { &mp_type_type },
    .name = MP_QSTR_LWP3,
    .print = pb_type_enum_print,
    .unary_op = mp_generic_unary_op,
    .locals_dict = (mp_obj_dict_t *)&(pb_enum_type_LWP3_locals_dict),
};

#endif // PYBRICKS_PY_PARAMETERS
