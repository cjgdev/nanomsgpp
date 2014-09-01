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
	, d_size(other.d_size)
	, d_malloc(other.d_malloc)
{}

part::part(const void* ptr, size_t size, bool deep_copy)
	: d_msg(const_cast<void*>(ptr))
	, d_size(size)
	, d_malloc(deep_copy)
{
	if (deep_copy) {
		d_msg  = std::malloc(size);
		d_size = size;
		std::memcpy(d_msg, ptr, size);
	}
}

part::part(size_t size, int type)
	: d_msg(nn_allocmsg(size, type))
	, d_size(NN_MSG)
	, d_malloc(false)
{}

part::part(size_t size)
	: d_msg(std::malloc(sizeof(char) * size))
	, d_size(size)
	, d_malloc(true)
{}

part::~part() {
	if (d_msg != nullptr) {
		if (!d_malloc) {
			int result = nn_freemsg(d_msg);
			if (-1 == result) {
				throw internal_exception();
			}
		} else {
			std::free(d_msg);
		}
		d_msg = nullptr;
	}
}

part&
part::operator=(part &&other) {
	d_msg    = other.release();
	d_size   = other.d_size;
	d_malloc = other.d_malloc;
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
{}

message::message(parts&& msgparts)
	: d_parts(std::move(msgparts))
{}

message::~message()
{}

void
message::add_part(part&& p) {
	d_parts.push_back(std::move(p));
}

message&
message::operator<<(part&& p) {
	add_part(std::move(p));
	return (*this);
}

template<>
void message::write(const std::string& data) {
	part p(data.c_str(), data.size(), true);
	add_part(std::move(p));
}

msghdr_unique_ptr
message::gen_nn_msghdr() {
	struct nn_msghdr *hdr = static_cast<nn_msghdr*>
		(std::malloc(sizeof(nn_msghdr)));
	struct nn_iovec *iov = static_cast<nn_iovec*>
		(std::malloc(sizeof(nn_iovec) * d_parts.size()));
	int i = 0;
	for (auto& p : d_parts) {
		iov[i].iov_base = static_cast<void*>(p);
		iov[i].iov_len = p.size();
		i++;
	}
	std::memset(hdr, 0, sizeof(nn_msghdr));
	hdr->msg_iov = iov;
	hdr->msg_iovlen = i;
	return msghdr_unique_ptr(hdr);
}

void
message::release() {
	for (auto& p : d_parts) {
		p.release();
	}
	d_parts.clear();
}
