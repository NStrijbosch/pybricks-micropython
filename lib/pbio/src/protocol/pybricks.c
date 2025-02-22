// SPDX-License-Identifier: MIT
// Copyright (c) 2021 The Pybricks Authors

// Pybricks communication protocol

#include <stdint.h>

#include <pbio/protocol.h>
#include <pbio/util.h>

_Static_assert(NUM_PBIO_PYBRICKS_STATUS <= sizeof(uint32_t) * 8,
    "oh no, we added too many status flags");

/**
 * Writes Pybricks status report command to @p buf
 *
 * @param [in]  buf     The buffer to hold the binary data.
 * @param [in]  flags   The status flags.
 * @return              The number of bytes written to @p buf.
 */
uint32_t pbio_pybricks_event_status_report(uint8_t *buf, uint32_t flags) {
    buf[0] = PBIO_PYBRICKS_EVENT_STATUS_REPORT;
    pbio_set_uint32_le(&buf[1], flags);
    return 5;
}

/**
 * Pybricks Service UUID.
 *
 * C5F50001-8280-46DA-89F4-6D8051E4AEEF
 */
const uint8_t pbio_pybricks_service_uuid[] = {
    0xC5, 0xF5, 0x00, 0x01, 0x82, 0x80, 0x46, 0xDA,
    0x89, 0xF4, 0x6D, 0x80, 0x51, 0xE4, 0xAE, 0xEF,
};

/**
 * Pybricks Characteristic UUID.
 *
 * C5F50002-8280-46DA-89F4-6D8051E4AEEF
 */
const uint8_t pbio_pybricks_control_char_uuid[] = {
    0xC5, 0xF5, 0x00, 0x02, 0x82, 0x80, 0x46, 0xDA,
    0x89, 0xF4, 0x6D, 0x80, 0x51, 0xE4, 0xAE, 0xEF,
};

// Standard BLE UUIDs used as part of Pybricks "protocol".

/** Bluetooth Device Information Service UUID. */
const uint16_t pbio_gatt_device_info_service_uuid = 0x180A;

/** Bluetooth Firmware Version Characteristic UUID. */
const uint16_t pbio_gatt_firmware_version_char_uuid = 0x2A26;

/** Bluetooth Software Version Characteristic UUID (Pybricks protocol version). */
const uint16_t pbio_gatt_software_version_char_uuid = 0x2A28;

/** Bluetooth PnP ID Characteristic UUID. */
const uint16_t pbio_gatt_pnp_id_char_uuid = 0x2A50;
