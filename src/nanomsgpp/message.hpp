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

	class part {
		void* d_msg;
	public:
		part(part&& other);

		part(size_t size, int type);

		~part();

		part& operator=(part &&other);

		operator void*() const { return d_msg; }

		void* release();

	private:
		part(const part& other) = delete;

		part& operator=(const part &other) = delete;
	};

	typedef std::vector<part> parts;

	class message {
		std::unique_ptr<nn_msghdr> d_header;
		parts                      d_parts;
	public:
		message();

		message(const message &other) = default;

		message(message &&other) = default;

		~message();

		message& operator=(const message &other) = default;

		message& operator=(message &&other) = default;

		void add_part(part&& p);

		message& operator<<(part&& p);

		operator nn_msghdr*() const { return d_header.get(); }

		nn_msghdr* release() { return d_header.release(); }
	};

}

#endif
