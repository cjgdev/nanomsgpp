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

#ifndef NANOMSGPP_POLLER_HPP_INCLUDED
#define NANOMSGPP_POLLER_HPP_INCLUDED

#ifndef NANOMSGPP_SOCKET_HPP_INCLUDED
#	include "socket.hpp"
#endif

namespace nanomsgpp {

	enum class poll_event {
		// Check whether at least one message can be received from the fd socket without blocking. 
		in     = NN_POLLIN,

		// Check whether at least one message can be sent to the fd socket without blocking. 
		out    = NN_POLLOUT,

		// Combination of NN_POLLIN and NN_POLLOUT.
		in_out = NN_POLLIN | NN_POLLOUT,
	};

	// The poller checks a set of SP sockets and reports whether it's possible to send a message to
	// the socket and / or receive a message from each socket.
	class poller {
		std::vector<nn_pollfd> d_pollfds;

	public:
		// Default constructor.
		poller();

		// Destructor.
		~poller() {};

		// MANIPULATORS

		// Add a socket with the given event to poll for.
		void add_socket(socket& s, poll_event e);

		// Remove a socket from the internal list of file descriptors.
		void remove_socket(socket& s);

		// Poll for events.
		bool poll(int timeout = -1);

		// Check whether a given event exists.
		bool has_event(socket& s, poll_event e);
	};

}

#endif
