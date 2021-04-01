/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF91_LWM2M_APP_H__
#define NRF91_LWM2M_APP_H__

#include <zephyr.h>
#include <net/lwm2m.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Security */
int lwm2m_init_security(struct lwm2m_ctx *ctx, char *endpoint);

/* Device */
int lwm2m_init_device(void);

/* Location */
#if defined CONFIG_LWM2M_LOCATION_OBJ_SUPPORT
int lwm2m_init_location(void);
#endif

#if defined(CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT)
void *firmware_read_cb(uint16_t obj_inst_id, size_t *data_len);
int lwm2m_init_firmware(void);
int lwm2m_init_image(void);
void lwm2m_verify_modem_fw_update(void);
#endif

#if defined(CONFIG_LWM2M_CONN_MON_OBJ_SUPPORT)
int lwm2m_init_connmon(void);
int lwm2m_update_connmon(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NRF91_LWM2M_APP_H__ */
