/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf91_lwm2m_fota.h
 * @defgroup nrf91_lwm2m_fota nRF91 LwM2M FOTA
 * @ingroup nrf91_lwm2m_app
 * @{
 * @brief API for the LwM2M based FOTA
 */

#ifndef NRF91_LWM2M_FOTA_H__
#define NRF91_LWM2M_FOTA_H__

/**
 * @brief Update counter
 */
struct update_counter {
	int current; /**< Active image's state */
	int update; /**< Update image's state */
};

/**
 * @brief Counter type
 */
enum counter_type {
	COUNTER_CURRENT = 0, /**< Active image's counter */
	COUNTER_UPDATE /**< Update image's counter */
};

/**
 * @brief Read the update counter
 */
int fota_update_counter_read(struct update_counter *update_counter);

/**
 * @brief Update the update counter
 */
int fota_update_counter_update(enum counter_type type, uint32_t new_value);

/**
 * @brief Initialize FOTA settings
 */
int fota_settings_init(void);

#endif /* NRF91_LWM2M_FOTA_H__ */

/**@} */
