/*
 * Copyright (c) the libnanomsgpp contributors. All rights reserved.
 *
 * This file is a part of libnanomsgpp, distributed under the MIT
 * license. For full terms see the included COPYING file.
 */

#ifndef NANOMSGPP_SOCKET_HPP_INCLUDED
#define NANOMSGPP_SOCKET_HPP_INCLUDED

#include <iostream>
#include <string>

namespace nanomsgpp {

class socket {
	int                        d_socket;
	socket_type                d_type;
	std::map<std::string, int> d_endpoints;

public:
	socket() = delete;

	socket(const socket &other) = delete;

	socket(socket &&other) = default;

	socket(socket_type type);

	~socket() = default;

	socket& operator=(const socket &other) = delete;

	socket& operator=(socket &&other) = default;

	void send(message &msg, bool dont_wait = true);

	void send_raw(const void *buf, size_t len, int flags);

	void operator<<(message &msg);

	message receive(bool dont_wait = true);

	int receive_raw(void *buf, size_t len, int flags);

	message operator>>();

	void set_option(socket_option opt, int val);

	void set_option(socket_option opt, const std::string &val);

	void set_option_raw(int level, int option, const void *val, size_t len);

	void bind(const std::string &addr);

	void connect(const std::string &addr);

	void shutdown(const std::string &addr);
};

}

#endif