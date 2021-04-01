#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT fota/settings.c)

zephyr_library_sources_ifdef(CONFIG_LWM2M_LOCATION_OBJ_SUPPORT lwm2m/lwm2m_location.c)
zephyr_library_sources_ifdef(CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT lwm2m/lwm2m_firmware.c)
zephyr_library_sources_ifdef(CONFIG_LWM2M_CONN_MON_OBJ_SUPPORT lwm2m/lwm2m_connmon.c)
zephyr_library_sources(lwm2m/lwm2m_device.c)
zephyr_library_sources(lwm2m/lwm2m_security.c)
