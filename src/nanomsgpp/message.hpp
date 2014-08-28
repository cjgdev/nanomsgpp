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

#ifndef NANOMSGPP_MESSAGE_HPP_INCLUDED
#define NANOMSGPP_MESSAGE_HPP_INCLUDED

#include <nanomsg/nn.h>
#include <iostream>
#include <memory>
#include <vector>

namespace nanomsgpp {

	struct msghdr_free {
		void operator()(void* x) {
			std::free(static_cast<nn_msghdr*>(x)->msg_iov);
			std::free(x);
		}
	};

	typedef std::unique_ptr<nn_msghdr, msghdr_free> msghdr_unique_ptr;

	class part {
		void*  d_msg;
		size_t d_size;
	public:
		// move constructor
		part(part&& other);

		// construct from pointer and size
		part(void* ptr, size_t size);

		// construct from size and type
		part(size_t size, int type);

		// destructor
		~part();

		// move assignment operator
		part& operator=(part &&other);

		// cast to void* operator
		operator void*() { return &d_msg; }

		// get size of memory pointed to d_msg
		size_t size() const { return d_size; }

		// transfer ownership of d_msg
		void* release();

	private:
		// not implemented
		part(const part& other) = delete;
		part& operator=(const part &other) = delete;
	};

	typedef std::vector<part> parts;

	class message {
		parts                      d_parts;
	public:
		message();

		message(const message &other) = default;

		message(message &&other) = default;

		// construct from nn_msghdr pointer
		message(msghdr_unique_ptr hdr);

		// destructor
		~message();

		message& operator=(const message &other) = default;

		message& operator=(message &&other) = default;

		// add a new part to the message
		void add_part(part&& p);

		// stream operator add message part
		message& operator<<(part&& p);

		// generate a nn_msghdr from d_parts
		msghdr_unique_ptr gen_nn_msghdr();

		// get an iterator to the beginning of d_parts
		parts::iterator begin() { return d_parts.begin(); }

		// get a const iterator to the beginning of d_parts
		parts::const_iterator cbegin() const { return d_parts.cbegin(); }

		// get an iterator to the end of d_parts
		parts::iterator end() { return d_parts.end(); }

		// get a const iterator to the end of d_parts
		parts::const_iterator cend() const { return d_parts.cend(); }

		// get the number of parts of the message
		size_t size() const { return d_parts.size(); }
	};

}

#endif
