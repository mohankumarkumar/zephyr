/* Copyright (c) 2022 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_CAVS_TEST_H
#define ZEPHYR_INCLUDE_CAVS_TEST_H

/* Helper to escape from infinite polling loops with a test failure
 * instead of a hang.  Spins with a relaxation loop so that it works
 * in interrupt context and doesn't stress shared resources like SRAM
 */
#define WAIT_FOR(expr) do {						\
		int i;							\
		for (i = 0; !(expr) && i < 10000; i++) {		\
			for (volatile int j = 0; j < 1000; j++) {	\
			}						\
		}							\
		zassert_true(i < 10000, "timeout waiting for %s", #expr); \
	} while (0)

/* The cavstool.py script that launched us listens for a very simple
 * set of IPC commands to help test.  Pass one of the following values
 * as the "data" argument to cavs_ipc_send_message(CAVS_HOST_DEV, ...):
 */
enum cavstool_cmd {
	/* The host takes no action, but signals DONE to complete the message */
	IPCCMD_SIGNAL_DONE,

	/* The host returns done after a short timeout */
	IPCCMD_ASYNC_DONE_DELAY,

	/* The host issues a new message with the ext_data value as its "data" */
	IPCCMD_RETURN_MSG,

	/* The host writes the given value to ADSPCS */
	IPCCMD_ADSPCS,

	/* The host emits a (real/host time) timestamp into the log stream */
	IPCCMD_TIMESTAMP,

	/* The host copies OUTBOX[ext_data >> 16] to INBOX[ext_data & 0xffff] */
	IPCCMD_WINCOPY,

	/* The host clears the run bit and resets the HDA stream */
	IPCCMD_HDA_RESET,

	/* The host configures an HDA stream (with provided buffer size and stream id) */
	IPCCMD_HDA_CONFIG,

	/* The host runs (sets the SDxCTL.RUN bit) for a given HDA stream */
	IPCCMD_HDA_START,

	/* The host stops (sets the SDxCTL.RUN bit) for a given HDA stream */
	IPCCMD_HDA_STOP,

	/* The host validates an HDA byte stream contains an 8bit counter and received a given
	 * number of bytes
	 */
	IPCCMD_HDA_VALIDATE,

	/* Host sends some data */
	IPCCMD_HDA_SEND
};

#endif /* ZEPHYR_INCLUDE_CAVS_TEST_H */
