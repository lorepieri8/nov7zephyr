/*
 * Copyright (c) 2018 Linaro Limited.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_MODULE_NAME net_socket_offload
#define NET_LOG_LEVEL CONFIG_NET_SOCKETS_LOG_LEVEL

#include <net/socket_offload.h>

/* Only one provider may register socket operations upon boot. */
const struct socket_offload *socket_ops;

void socket_offload_register(const struct socket_offload *ops)
{
	__ASSERT_NO_MSG(ops);
	__ASSERT_NO_MSG(socket_ops == NULL);

	socket_ops = ops;
}
