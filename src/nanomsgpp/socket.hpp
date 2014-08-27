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

#ifndef NANOMSGPP_SOCKET_TYPE_HPP_INCLUDED
#	include "socket_type.hpp"
#endif
#ifndef NANOMSGPP_SOCKET_OPTION_HPP_INCLUDED
#	include "socket_option.hpp"
#endif
#ifndef NANOMSGPP_MESSAGE_HPP_INCLUDED
#	include "message.hpp"
#endif

#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace nanomsgpp {

	class socket {
		int                        d_socket;
		std::map<std::string, int> d_endpoints;

	public:
		socket(socket &&other) = default;

		explicit socket(int socket);

		explicit socket(socket_domain domain, socket_type type);

		~socket();

		socket& operator=(socket &&other) = default;

		int get_fd() const { return d_socket; }

		void send(message &msg, bool dont_wait = true);

		int send_raw(const void *buf, size_t len, int flags);

		void operator<<(message &msg);

		message receive(bool dont_wait = true);

		int receive_raw(void *buf, size_t len, int flags);

		socket& operator>>(std::unique_ptr<message> &m);

		void set_option(int level, socket_option opt, int val);

		void set_option(int level, socket_option opt, const std::string &val);

		void set_option_raw(int level, int option, const void *val, size_t len);

		template<typename T>
		T get_option(int level, socket_option opt);

		void get_option_raw(int level, int option, void *val, size_t *len);

		void bind(const std::string &addr);

		void connect(const std::string &addr);

		void shutdown(const std::string &addr);

	private:
		socket() = delete;

		socket(const socket &other) = delete;

		socket& operator=(const socket &other) = delete;
	};

	// INLINE FUNCTION DEFINITIONS

	template<>
	int socket::get_option(int level, socket_option opt);

	template<>
	std::string socket::get_option(int level, socket_option opt);

}

#endif
