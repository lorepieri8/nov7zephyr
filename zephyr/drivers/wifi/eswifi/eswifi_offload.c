/**
 * Copyright (c) 2018 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#define LOG_LEVEL CONFIG_WIFI_LOG_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(wifi_eswifi_offload);

#include <zephyr.h>
#include <kernel.h>
#include <device.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <net/net_pkt.h>
#include <net/net_if.h>

#include "eswifi.h"

#define ESWIFI_OFFLOAD_THREAD_STACK_SIZE 1024
K_THREAD_STACK_MEMBER(eswifi_data_read_stack, ESWIFI_OFFLOAD_THREAD_STACK_SIZE);
struct k_thread data_read_thread;

struct eswifi_offload {
	struct eswifi_off_socket socket[ESWIFI_OFFLOAD_MAX_SOCKETS];
};

static int __select_socket(struct eswifi_dev *eswifi, u8_t idx)
{
	int err;

	snprintf(eswifi->buf, sizeof(eswifi->buf), "P0=%d\r", idx);

	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		return -EIO;
	}

	return 0;
}

static int __read_data(struct eswifi_dev *eswifi, size_t len)
{
	char cmd[] = "R0\r";
	char size[] = "R1=9999\r";
	char timeout[] = "R2=30000\r";
	int err, i, read = 0;

	/* Set max read size */
	snprintf(size, sizeof(eswifi->buf), "R1=%u\r", len);
	err = eswifi_request(eswifi, size, strlen(size),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to set read size");
		return -EIO;
	}

	/* Set timeout */
	snprintf(timeout, sizeof(eswifi->buf), "R2=%u\r", 30); /* 30 ms */
	err = eswifi_request(eswifi, timeout, strlen(timeout),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to set timeout");
		return -EIO;
	}

	err = eswifi_request(eswifi, cmd, strlen(cmd), eswifi->buf,
			     sizeof(eswifi->buf));
	if (err) {
		return -EIO;
	}

	/* find payload size */
	/* '\r\n''paylod'\r\n''OK''\r\n''> ' */
	for (i = 0; i < sizeof(eswifi->buf); i++) {
		if (!strncmp(&eswifi->buf[i], AT_OK_STR, AT_OK_STR_LEN)) {
			read = i - AT_RSP_DELIMITER_LEN;
			break;
		}
	}

	return read;
}

static int eswifi_off_get(sa_family_t family,
			  enum net_sock_type type,
			  enum net_ip_protocol ip_proto,
			  struct net_context **context)
{
	struct eswifi_dev *eswifi = eswifi_by_iface_idx((*context)->iface);
	struct eswifi_off_socket *socket = NULL;
	int err, i;

	LOG_DBG("");

	if (family != AF_INET) {
		LOG_ERR("Only AF_INET is supported!");
		return -EPFNOSUPPORT;
	}

	if (ip_proto != IPPROTO_TCP) {
		/* TODO: add UDP */
		LOG_ERR("Only TCP supported");
		return -EPROTONOSUPPORT;
	}

	eswifi_lock(eswifi);

	/* pickup available socket */
	for (i = 0; i < ESWIFI_OFFLOAD_MAX_SOCKETS; i++) {
		if (!eswifi->socket[i].context) {
			socket = &eswifi->socket[i];
			socket->index = i;
			socket->context = *context;
			(*context)->offload_context = socket;
			break;
		}
	}

	if (!socket) {
		LOG_ERR("No socket resource available");
		eswifi_unlock(eswifi);
		return -ENOMEM;
	}

	err = __select_socket(eswifi, socket->index);
	if (err) {
		LOG_ERR("Unable to select socket %u", socket->index);
		eswifi_unlock(eswifi);
		return -EIO;
	}

	/* Set Transport Protocol */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "P1=%d\r",
		ESWIFI_TRANSPORT_TCP);
	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to set transport protocol");
		eswifi_unlock(eswifi);
		return -EIO;
	}

	eswifi_unlock(eswifi);

	return 0;
}

static int eswifi_off_bind(struct net_context *context,
			   const struct sockaddr *addr,
			   socklen_t addrlen)
{
	struct eswifi_off_socket *socket = context->offload_context;
	struct eswifi_dev *eswifi = eswifi_by_iface_idx(context->iface);
	int err;

	if (addr->sa_family != AF_INET) {
		LOG_ERR("Only AF_INET is supported!");
		return -EPFNOSUPPORT;
	}

	LOG_DBG("");

	eswifi_lock(eswifi);

	__select_socket(eswifi, socket->index);

	/* Set Local Port */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "P2=%d\r",
		 (u16_t)sys_be16_to_cpu(net_sin(addr)->sin_port));
	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to set local port");
		eswifi_unlock(eswifi);
		return -EIO;
	}

	eswifi_unlock(eswifi);

	return 0;
}

static int eswifi_off_listen(struct net_context *context, int backlog)
{
	/* TODO */
	LOG_ERR("");
	return -ENOTSUP;
}

static int eswifi_off_connect(struct net_context *context,
			      const struct sockaddr *addr,
			      socklen_t addrlen,
			      net_context_connect_cb_t cb,
			      s32_t timeout,
			      void *user_data)
{
	struct eswifi_off_socket *socket = context->offload_context;
	struct eswifi_dev *eswifi = eswifi_by_iface_idx(context->iface);
	struct in_addr *sin_addr = &net_sin(addr)->sin_addr;
	int err;

	if (addr->sa_family != AF_INET) {
		LOG_ERR("Only AF_INET is supported!");
		return -EPFNOSUPPORT;
	}

	eswifi_lock(eswifi);

	/* Set Remote IP */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "P3=%u.%u.%u.%u\r",
		 sin_addr->s4_addr[0], sin_addr->s4_addr[1],
		 sin_addr->s4_addr[2], sin_addr->s4_addr[3]);

	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to set remote ip");
		eswifi_unlock(eswifi);
		return -EIO;
	}

	/* Set Remote Port */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "P4=%d\r",
		(u16_t)sys_be16_to_cpu(net_sin(addr)->sin_port));
	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to set remote port");
		eswifi_unlock(eswifi);
		return -EIO;
	}

	/* Start TCP client */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "P6=1\r");
	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to connect");
		eswifi_unlock(eswifi);
		return -EIO;
	}

	socket->state = ESWIFI_SOCKET_STATE_CONNECTED;

	eswifi_unlock(eswifi);

	return 0;
}

static int eswifi_off_accept(struct net_context *context,
		     net_tcp_accept_cb_t cb,
		     s32_t timeout,
		     void *user_data)
{
	/* TODO */
	LOG_DBG("");
	return -ENOTSUP;
}

static int eswifi_off_send(struct net_pkt *pkt,
			   net_context_send_cb_t cb,
			   s32_t timeout,
			   void *token,
			   void *user_data)
{
	struct eswifi_off_socket *socket = pkt->context->offload_context;
	struct eswifi_dev *eswifi = eswifi_by_iface_idx(socket->context->iface);
	unsigned int bytes;
	struct net_buf *frag;
	int err, offset;

	bytes = net_pkt_get_len(pkt);

	LOG_DBG("%u bytes", bytes);

	eswifi_lock(eswifi);

	__select_socket(eswifi, socket->index);

	/* header */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "S3=%u\r", bytes);
	offset = strlen(eswifi->buf);

	/* copy payload */
	for (frag = pkt->frags; frag; frag = frag->frags) {
		memcpy(&eswifi->buf[offset], frag->data, frag->len);
		offset += frag->len;
	}

	err = eswifi_request(eswifi, eswifi->buf, offset + 1,
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to send data");
		eswifi_unlock(eswifi);
		return -EIO;
	}

	net_pkt_unref(pkt);
	eswifi_unlock(eswifi);

	return 0;
}

static int eswifi_off_sendto(struct net_pkt *pkt,
			     const struct sockaddr *dst_addr,
			     socklen_t addrlen,
			     net_context_send_cb_t cb,
			     s32_t timeout,
			     void *token,
			     void *user_data)
{
	/* TODO */
	LOG_DBG("");
	return -ENOTSUP;
}

static int eswifi_off_recv(struct net_context *context,
			   net_context_recv_cb_t cb,
			   s32_t timeout,
			   void *user_data)
{
	struct eswifi_off_socket *socket = context->offload_context;
	struct eswifi_dev *eswifi = eswifi_by_iface_idx(context->iface);

	/* TODO */
	LOG_DBG("");

	eswifi_lock(eswifi);
	socket->recv_cb = cb;
	socket->user_data = user_data;
	eswifi_unlock(eswifi);

	return 0;
}

static int eswifi_off_put(struct net_context *context)
{
	struct eswifi_off_socket *socket = context->offload_context;
	struct eswifi_dev *eswifi = eswifi_by_iface_idx(context->iface);
	int err;

	LOG_DBG("");

	eswifi_lock(eswifi);

	__select_socket(eswifi, socket->index);

	socket->context = NULL;
	socket->state = ESWIFI_SOCKET_STATE_NONE;

	/* Stop TCP client */
	snprintf(eswifi->buf, sizeof(eswifi->buf), "P6=0\r");
	err = eswifi_request(eswifi, eswifi->buf, strlen(eswifi->buf),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		LOG_ERR("Unable to connect");
		return -EIO;
	}

	eswifi_unlock(eswifi);

	return 0;
}

static struct net_offload eswifi_offload = {
	.get	       = eswifi_off_get,
	.bind	       = eswifi_off_bind,
	.listen	       = eswifi_off_listen,
	.connect       = eswifi_off_connect,
	.accept	       = eswifi_off_accept,
	.send	       = eswifi_off_send,
	.sendto	       = eswifi_off_sendto,
	.recv	       = eswifi_off_recv,
	.put	       = eswifi_off_put,
};

static void eswifi_off_read_data(struct eswifi_dev *eswifi)
{
	int i;

	LOG_DBG("");

	eswifi_lock(eswifi);

	if (!eswifi->iface) { /* not yet initialized */
		eswifi_unlock(eswifi);
		return;
	}

	for (i = 0; i < ESWIFI_OFFLOAD_MAX_SOCKETS; i++) {
		struct eswifi_off_socket *socket = &eswifi->socket[i];
		struct net_pkt *pkt;
		int err, len;

		if (socket->state != ESWIFI_SOCKET_STATE_CONNECTED) {
			continue;
		}

		err = __select_socket(eswifi, i);
		if (err) {
			continue;
		}

		len = __read_data(eswifi, 1460); /* 1460 is max size */
		if (len <= 0 /*|| !socket->recv_cb*/) {
			continue;
		}

		LOG_DBG("payload sz = %d", len);

		pkt = net_pkt_get_reserve_rx(0, K_NO_WAIT);
		if (!pkt) {
			LOG_ERR("Cannot allocate rx packet");
			continue;
		}

		if (!net_pkt_append_all(pkt, len,
					eswifi->buf + AT_RSP_DELIMITER_LEN,
					K_NO_WAIT)) {
			LOG_WRN("Incomplete buffer copy");
		}

		if (!socket->recv_cb) {
			net_pkt_unref(pkt);
			continue;
		}
		socket->recv_cb(socket->context, pkt, 0, socket->user_data);
	}

	eswifi_unlock(eswifi);
}

static void eswifi_off_data_read_thread(void *p1)
{
	struct eswifi_dev *eswifi = p1;

	while (1) {
		k_sleep(K_MSEC(500));
		eswifi_off_read_data(eswifi);
	}
}

static int eswifi_off_enable_dhcp(struct eswifi_dev *eswifi)
{
	char cmd[] = "C4=1\r";
	int err;

	LOG_DBG("");

	eswifi_lock(eswifi);

	err = eswifi_request(eswifi, cmd, strlen(cmd),
			     eswifi->buf, sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		eswifi_unlock(eswifi);
		return -EIO;
	}

	eswifi_unlock(eswifi);

	return 0;
}

static int eswifi_off_disable_bypass(struct eswifi_dev *eswifi)
{
	char cmd[] = "PR=0\r";
	int err;

	LOG_DBG("");

	eswifi_lock(eswifi);

	err = eswifi_request(eswifi, cmd, strlen(cmd), eswifi->buf,
			     sizeof(eswifi->buf));
	if (err || !eswifi_is_buf_at_ok(eswifi->buf)) {
		eswifi_unlock(eswifi);
		return -EIO;
	}

	eswifi_unlock(eswifi);

	return 0;
}

int eswifi_offload_init(struct eswifi_dev *eswifi)
{
	eswifi->iface->if_dev->offload = &eswifi_offload;
	int err;

	k_thread_create(&data_read_thread, eswifi_data_read_stack,
			ESWIFI_OFFLOAD_THREAD_STACK_SIZE,
			(k_thread_entry_t)&eswifi_off_data_read_thread, eswifi,
			NULL, NULL, K_PRIO_COOP(CONFIG_WIFI_ESWIFI_THREAD_PRIO),
			0, K_NO_WAIT);

	err = eswifi_off_enable_dhcp(eswifi);
	if (err) {
		LOG_ERR("Unable to configure dhcp");
		return err;
	}

	err = eswifi_off_disable_bypass(eswifi);
	if (err) {
		LOG_ERR("Unable to disable bypass mode");
		return err;
	}

	return 0;
}
