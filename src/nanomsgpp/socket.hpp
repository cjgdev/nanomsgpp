/*
 * Copyright (C) 2014 Christopher Gilbert <christopher.john.gilbert@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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