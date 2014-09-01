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
#include <nanomsg/nn.h>
#include <cstring>

using namespace nanomsgpp;

socket::socket(socket &&other) {
	*this = std::move(other);
}

socket::socket(int socket)
	: d_socket(socket)
	, d_endpoints()
{}

socket::socket(socket_domain domain, socket_type type)
	: d_socket(nn_socket(static_cast<int>(domain), static_cast<int>(type)))
	, d_endpoints()
{}

socket::~socket() {
	close();
}

socket&
socket::operator=(socket &&other) {
	d_socket = other.d_socket;
	other.d_socket = -1;
	d_endpoints = std::move(other.d_endpoints);
	return (*this);
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

void
socket::close() {
	if (d_socket >= 0) {
		int result = nn_close(d_socket);
		d_socket = -1;
		if (-1 == result) {
			throw internal_exception();
		}
	}
}

int
socket::sendmsg(message&& msg, bool dont_wait) {
	int nb = nn_sendmsg(d_socket, msg.gen_nn_msghdr().get(), (dont_wait) ? NN_DONTWAIT : 0);
	msg.release();
	if (-1 == nb) {
		throw internal_exception();
	}
	return nb;
}

socket&
socket::operator<<(message&& msg) {
	sendmsg(std::move(msg));
	return (*this);
}

int
socket::send_raw(const void *buf, size_t len, int flags) {
	int nb = nn_send(d_socket, buf, len, flags);
	if (-1 == nb) {
		throw internal_exception();
	}
	return nb;
}

std::unique_ptr<message>
socket::recvmsg(size_t n_parts, bool dont_wait) {
	struct nn_msghdr hdr;
	struct nn_iovec iov[n_parts];
	void* buf[n_parts];

	size_t buf_size = (1 == n_parts) ? NN_MSG : 2048;

	for (int i = 0; i < n_parts; ++i) {
		if (buf_size != NN_MSG) {
			buf[i] = malloc(buf_size);
		}
		iov[i].iov_len  = buf_size;
		iov[i].iov_base = &buf[i];
	}

	std::memset(&hdr, 0, sizeof(hdr));
	hdr.msg_iov = iov;
	hdr.msg_iovlen = n_parts;

	int nb = nn_recvmsg(d_socket, &hdr, (dont_wait) ? NN_DONTWAIT : 0);
	if (-1 == nb) {
		throw internal_exception();
	}

	parts msgparts;
	for (int i = 0; i < n_parts; ++i) {
		if (1 == n_parts) {
			msgparts.push_back(part(*(void**)buf[i], nb, true));
			int rc = nn_freemsg(buf[i]); // TODO: free me in part destructor?
			if (-1 == rc) {
				throw internal_exception();
			}
		} else {
			msgparts.push_back(part(*(void**)buf[i], buf_size, true)); // TODO: size should be NN_MSG?
		}
	}

	return std::unique_ptr<message>
		(new message(std::move(msgparts)));
}

int
socket::recv_raw(void *buf, size_t len, int flags) {
	int nb = nn_recv(d_socket, buf, len, flags);
	if (-1 == nb) {
		throw internal_exception();
	}
	return nb;
}
