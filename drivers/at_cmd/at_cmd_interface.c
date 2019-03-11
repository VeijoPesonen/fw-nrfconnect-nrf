/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(at_cmd_interface);

#include <zephyr.h>
#include <stdio.h>
#include <net/socket.h>
#include <init.h>
#include <bsd_limits.h>

#include <at_cmd_interface.h>

#define RESPONSE_MAX_LEN	CONFIG_AT_CMD_RESPONSE_MAX_LEN
#define THREAD_PRIORITY		K_PRIO_PREEMPT(CONFIG_AT_CMD_THREAD_PRIO)

#define AT_CMD_OK_STR    "OK"
#define AT_CMD_ERROR_STR "ERROR"
#define AT_CMD_CMS_STR   "+CMS:"
#define AT_CMD_CME_STR   "+CME:"

static int common_socket_fd;
static struct k_thread socket_thread;
static K_THREAD_STACK_DEFINE(socket_thread_stack, 2048);

K_MSGQ_DEFINE(return_code_msq, sizeof(enum at_cmd_return_code), 2, 4);
static K_SEM_DEFINE(cmd_pending, 1, 1);

static at_cmd_interface_handler_t received_data_handler;

static int  last_error;

static enum at_cmd_return_code get_return_code(char *buf)
{
	enum at_cmd_return_code return_code = AT_CMD_NOT_RETURN_CODE;
	char *tmpstr = NULL;

	do {
		tmpstr = strstr(buf, AT_CMD_OK_STR);
		if (tmpstr) {
			return_code = AT_CMD_OK;
			break;
		}

		tmpstr = strstr(buf, AT_CMD_ERROR_STR);
		if (tmpstr) {
			return_code = AT_CMD_ERROR;
			break;
		}

		tmpstr = strstr(buf, AT_CMD_CMS_STR);
		if (tmpstr) {
			return_code = AT_CMD_CMS;
			break;
		}

		tmpstr = strstr(buf, AT_CMD_CME_STR);
		if (tmpstr) {
			return_code = AT_CMD_CME;
			break;
		}
	} while (0);

	if ((return_code == AT_CMD_CMS) || (return_code == AT_CMD_CME)) {
		char tmpstr[5];

		memcpy(tmpstr, &buf[strlen(AT_CMD_CMS_STR)], 3);
		last_error = atoi(tmpstr);
	} else {
		last_error = 0;
	}

	if (tmpstr) {
		buf[tmpstr - buf] = '\0';
	}

	return return_code;
}

static void socket_thread_fn(void *arg1, void *arg2, void *arg3)
{
	int bytes_read;
	char buf[RESPONSE_MAX_LEN];
	enum at_cmd_return_code return_code;

	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	LOG_DBG("Socket thread started");

	for (;;) {
		bytes_read = recv(common_socket_fd, buf, sizeof(buf), MSG_DONTWAIT);

		if (bytes_read <= 0) {
			k_sleep(K_MSEC(10));
			continue;
		}

		return_code = get_return_code(buf);

		if (return_code != AT_CMD_NOT_RETURN_CODE) {
			k_msgq_put(&return_code_msq, &return_code, K_FOREVER);
		}

		if (strlen(buf) == 0) {
			continue;
		}

		if (received_data_handler) {
			received_data_handler(buf, strlen(buf));
		} else {
			LOG_DBG("No handler registerd for received data");
		}

	}
}

enum at_cmd_return_code at_cmd_interface_write(const char *const cmd)
{
	int bytes_sent;
	enum at_cmd_return_code return_code;

	k_sem_take(&cmd_pending, K_FOREVER);

	LOG_DBG("Sending command %s", cmd);
	bytes_sent = send(common_socket_fd, cmd, strlen(cmd), 0);

	k_msgq_get(&return_code_msq, &return_code, K_FOREVER);

	LOG_DBG("Bytes sent: %d", bytes_sent);

	k_sem_give(&cmd_pending);

	return return_code;
}

void at_cmd_interface_set_handler(at_cmd_interface_handler_t handler)
{
	received_data_handler = handler;
}

int at_cmd_interface_get_last_error(void)
{
	return last_error;
}

static int at_cmd_interface_init(struct device *dev)
{
	ARG_UNUSED(dev);

	common_socket_fd = socket(AF_LTE, 0, NPROTO_AT);

	if (common_socket_fd == -1) {
		LOG_ERR("Socket could not be established.");
		return -EFAULT;
	}

	k_thread_create(&socket_thread, socket_thread_stack,
			K_THREAD_STACK_SIZEOF(socket_thread_stack),
			socket_thread_fn,
			NULL, NULL, NULL,
			THREAD_PRIORITY, 0, K_NO_WAIT);

	return 0;
}

SYS_INIT(at_cmd_interface_init, APPLICATION, \
		CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);

