/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file nrf91_lwm2m_app.h
 *
 * @defgroup nrf91_lwm2m_app nRF91 LwM2M application
 * @{
 * @brief LwM2M Client for nRF91 based devices
 *
 * @details The client provides APIs for:
 *  - connecting to a remote server
 *  - setting up default resources
 *    * Firmware
 *    * Connection monitoring
 *    * Device
 *    * Location
 *    * Security
 */

#ifndef NRF91_LWM2M_APP_H__
#define NRF91_LWM2M_APP_H__

#include <zephyr.h>
#include <net/lwm2m.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_NRF91_LWM2M_APP_SECURITY_OBJ_SUPPORT)
/**
 * @brief Initialize Security object
 */
int lwm2m_init_security(struct lwm2m_ctx *ctx, char *endpoint);
#endif

#if defined(CONFIG_NRF91_LWM2M_APP_DEVICE_OBJ_SUPPORT)
/**
 * @brief Initialize Device object
 */
int lwm2m_init_device(void);
#endif

#if defined(CONFIG_NRF91_LWM2M_APP_LOCATION_OBJ_SUPPORT)
/**
 * @brief Initialize Location object
 */
int lwm2m_init_location(void);
#endif

#if defined(CONFIG_NRF91_LWM2M_APP_FIRMWARE_UPDATE_OBJ_SUPPORT)
/**
 * @brief Firmware read callback
 */
void *firmware_read_cb(uint16_t obj_inst_id, size_t *data_len);
/**
 * @brief Verify active firmware image
 */
int lwm2m_init_firmware(void);

/**
 * @brief Initialize Image Update object
 */
int lwm2m_init_image(void);

/**
 * @brief Verifies modem firmware update
 */
void lwm2m_verify_modem_fw_update(void);
#endif

#if defined(CONFIG_NRF91_LWM2M_APP_CONN_MON_OBJ_SUPPORT)
/**
 * @brief Initialize Connection Monitor object
 */
int lwm2m_init_connmon(void);

/**
 * @brief Update Connection Monitor object
 */
int lwm2m_update_connmon(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NRF91_LWM2M_APP_H__ */

/**@} */
