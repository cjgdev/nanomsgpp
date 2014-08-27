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

#include "nanomsgpp/message.hpp"
#include "nanomsgpp/exception.hpp"
#include <nanomsg/nn.h>
#include <cstring>

using namespace nanomsgpp;

part::part(part&& other)
	: d_msg(other.release())
{}

part::part(size_t size, int type)
	: d_msg(nn_allocmsg(size, type))
{}

part::~part() {
	if (d_msg != nullptr) {
		int result = nn_freemsg(d_msg);
		if (-1 == result) {
			throw internal_exception();
		}
	}
}

part&
part::operator=(part &&other) {
	d_msg = other.release();
	return (*this);
}

void*
part::release() {
	void* msg = d_msg;
	d_msg = nullptr;
	return msg;
}

// *** MESSAGE IMPLEMENTATION

message::message()
	: d_header(new nn_msghdr)
{
	std::memset(d_header.get(), 0, sizeof(nn_msghdr));
}

message::~message() {
	if (d_header) {
		delete d_header.release();
	}
}

void
message::add_part(part&& p) {
	d_parts.push_back(std::move(p));
}

message&
message::operator<<(part&& p) {
	add_part(std::move(p));
	return (*this);
}
