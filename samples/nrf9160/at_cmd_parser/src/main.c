/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>

#include <at_cmd_interface.h>
#include <at_cmd_parser/at_cmd_decoder.h>

/* This callback will be called when ever a message from the modem is completely
 * parsed. The model parameter will contain an identifier for the content
 * of the message, and the model_ptr parameter will contain a pointer to a
 * model of the data.
 */
void process_model(enum at_cmd_models model, void *model_ptr)
{
	static u32_t count;

	printk("------Message %04d-------\n", count++);

	switch (model) {
	case AT_CMD_CEREG_MODEL:
		printk("CEREG Stat: %d\n",
			((struct at_cmd_model_cereg *)model_ptr)->stat);
		printk("CEREG TAC:  %s\n",
			((struct at_cmd_model_cereg *)model_ptr)->tac);
		printk("CEREG CI:   %s\n",
			((struct at_cmd_model_cereg *)model_ptr)->ci);
		printk("CEREG Act:  %d\n",
			((struct at_cmd_model_cereg *)model_ptr)->act);
		break;
	case AT_CMD_CESQ_MODEL:
		printk("CESQ Rxlev: %d\n",
			((struct at_cmd_model_cesq *)model_ptr)->rxlev);
		printk("CESQ ber:   %d\n",
			((struct at_cmd_model_cesq *)model_ptr)->ber);
		printk("CESQ rscp:  %d\n",
			((struct at_cmd_model_cesq *)model_ptr)->rscp);
		printk("CESQ ecno:  %d\n",
			((struct at_cmd_model_cesq *)model_ptr)->ecno);
		printk("CESQ rsrq:  %d\n",
			((struct at_cmd_model_cesq *)model_ptr)->rsrq);
		printk("CESQ rsrp:  %d\n",
			((struct at_cmd_model_cesq *)model_ptr)->rsrp);
		break;
	case AT_NOT_CESQ_MODEL:
		printk("Notification CESQ rsrp: %d\n",
			((struct at_not_model_cesq *)model_ptr)->rsrp);
		printk("Notification CESQ threshold index: %d\n",
			((struct at_not_model_cesq *)
				model_ptr)->threshold_index);
		break;
	default:
		break;
	}

	printk("-------------------------\n");
}

void received_data_handler(char *response, size_t response_len)
{
	if (at_cmd_decode(response) != 0) {
		printk("Could not decode AT command response %s\n", response);
	}
}

/* This sample will enable unsolicited (AT+CEREG=2),
 * then enable signal strength notifications (AT%CESQ=1),
 * then finally poll for extended signal strengt information
 * every 5 seconds.
 */
void main(void)
{
	char buf[32];

	printk("The AT command parser sample started\n");

	at_cmd_set_handler(received_data_handler);
	at_cmd_decoder_init(NULL);
	at_cmd_decoder_set_handler(process_model);

	if (at_cmd_write("AT+CEREG=2") != AT_CMD_OK) {
		printk("Failed to send AT+CEREG=2 command\n");
	} else {
		printk("AT+CEREG=2 command returned OK\n");
	}

	if (at_cmd_write("AT%CESQ=1") != AT_CMD_OK) {
		printk("Failed to send AT%%CESQ=1 command\n");
	} else {
		printk("AT%CESQ=1 command returned OK\n");
	}

	while (1) {
		if (at_cmd_write_with_response("AT+CESQ", buf, 32)
						!= AT_CMD_OK) {
			printk("Failed to send AT+CESQ command");
		} else {
			printk("AT+CESQ command response: %s\n", buf);
		}
		k_sleep(K_MSEC(5000));
	}
}
