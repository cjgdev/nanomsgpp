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

#ifndef NANOMSGPP_DEVICE_HPP_INCLUDED
#define NANOMSGPP_DEVICE_HPP_INCLUDED

#ifndef NANOMSGPP_SOCKET_HPP_INCLUDED
#	include "socket.hpp"
#endif

namespace nanomsgpp {

	// A device is used to forward messages between two sockets. If only one socket is valid, the
	// device works in a loopback mode, where any messages received from the socket are sent back
	// to itself
	class device {
	public:
		// construct a device from socket a and socket b
		device(socket& a, socket& b);

		// destructor
		~device() {};

	private:
		// NOT IMPLEMENTED
		device() = delete;
		device(device&& other) = delete;
		device(const device& other) = delete;
		device& operator=(const device& other) = delete;
		device& operator=(device&& other) = delete;
	};

}

#endif
