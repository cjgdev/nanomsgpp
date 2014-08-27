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

#include "nanomsgpp/socket.hpp"
#include "nanomsgpp/exception.hpp"

using namespace nanomsgpp;

socket::socket(int socket)
	: d_socket(socket)
	, d_endpoints()
{}

socket::socket(socket_domain domain, socket_type type)
	: d_socket(nn_socket(static_cast<int>(domain), static_cast<int>(type)))
	, d_endpoints()
{}

socket::~socket() {
	if (d_socket >= 0) {
		int result = nn_close(d_socket);
		d_socket = -1;
		if (result != 0) {
			throw internal_exception();
		}
	}
}

void
socket::set_option(int level, socket_option opt, int val) {
	set_option_raw(level, static_cast<int>(opt), &val, sizeof(val));
}

void
socket::set_option(int level, socket_option opt, const std::string &val) {
	set_option_raw(level, static_cast<int>(opt), val.c_str(), val.size());
}

void
socket::set_option_raw(int level, int option, const void *val, size_t len) {
	int result = nn_setsockopt(d_socket, level, option, val, len);
	if (result != 0) {
		throw internal_exception();
	}
}

template<>
int socket::get_option(int level, socket_option opt) {
	int val; size_t len = sizeof(val);
	get_option_raw(level, static_cast<int>(opt), &val, &len);
	return val;
}

template<>
std::string socket::get_option(int level, socket_option opt) {
	char val[1024]; size_t len = sizeof(val);
	get_option_raw(level, static_cast<int>(opt), val, &len);
	return std::string(val);
}

void
socket::get_option_raw(int level, int option, void *val, size_t *len) {
	int result = nn_getsockopt(d_socket, level, option, val, len);
	if (result != 0) {
		throw internal_exception();
	}
}

void
socket::bind(const std::string &addr) {
	int id = nn_bind(d_socket, addr.c_str());
	if (-1 == id) {
		throw internal_exception();
	} else {
		d_endpoints[addr] = id;
	}
}

void
socket::connect(const std::string &addr) {
	int id = nn_connect(d_socket, addr.c_str());
	if (-1 == id) {
		throw internal_exception();
	} else {
		d_endpoints[addr] = id;
	}
}

void
socket::shutdown(const std::string &addr) {
	int id = -1;
	auto found = d_endpoints.find(addr);
	if (found != d_endpoints.end()) {
		id = found->second;
	}
	int result = nn_shutdown(d_socket, id);
	if (-1 == result) {
		throw internal_exception();
	}
}

int
socket::send_raw(const void *buf, size_t len, int flags) {
	int nb = nn_send(d_socket, buf, len, flags);
	if (-1 == len) {
		throw internal_exception();
	}
	return nb;
}

int
socket::receive_raw(void *buf, size_t len, int flags) {
	int nb = nn_recv(d_socket, buf, len, flags);
	if (-1 == len) {
		throw internal_exception();
	}
	return nb;
}
