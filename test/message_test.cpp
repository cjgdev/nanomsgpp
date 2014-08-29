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

#include "catch.hpp"

#include <nanomsgpp/message.hpp>
#include <nanomsgpp/socket.hpp>

namespace nn = nanomsgpp;

TEST_CASE("messages can be manipulated", "[message]") {
	SECTION("default constructor") {
		nn::message m;
	}
	SECTION("add message part") {
		nn::message m;
		m.add_part(nn::part(12, 0));
		REQUIRE(m.size() == 1);
	}
	SECTION("stream operator add message part") {
		nn::message m;
		m << nn::part(12, 0);
		REQUIRE(m.size() == 1);
	}
	SECTION("generate nn_msghdr") {
		nn::message m;
		m.add_part(nn::part(1, 0));
		m.add_part(nn::part(10, 0));
		m.add_part(nn::part(100, 0));
		REQUIRE(m.size() == 3);

		nn::msghdr_unique_ptr hdr = m.gen_nn_msghdr();
		REQUIRE(hdr->msg_iovlen == 3);
		REQUIRE(hdr->msg_iov[0].iov_base != nullptr);
		REQUIRE(hdr->msg_iov[0].iov_len == NN_MSG);
		REQUIRE(hdr->msg_iov[1].iov_base != nullptr);
		REQUIRE(hdr->msg_iov[1].iov_len == NN_MSG);
		REQUIRE(hdr->msg_iov[2].iov_base != nullptr);
		REQUIRE(hdr->msg_iov[2].iov_len == NN_MSG);
	}
	SECTION("range based for message parts") {
		nn::message m;
		for (int i = 0; i < 10; ++i) {
			m.add_part(nn::part(1, 0));
		}
		int i = 0;
		for (auto& part : m) {
			REQUIRE(static_cast<void*>(part) != nullptr);
			REQUIRE(part.size() == NN_MSG);
			i++;
		}
		REQUIRE(i == 10);
	}
}

TEST_CASE("messages can be sent and received", "[message]") {
	SECTION("can send and receive messages") {
		nn::socket s1(nn::socket_domain::sp, nn::socket_type::pair);
		REQUIRE_NOTHROW(s1.bind("inproc://test"));

		nn::socket s2(nn::socket_domain::sp, nn::socket_type::pair);
		REQUIRE_NOTHROW(s2.connect("inproc://test"));

		nn::message send;
		send << nn::part(256, 0);
		REQUIRE(send.size() == 1);
		REQUIRE(s1.sendmsg(std::move(send)) == 256);

		std::unique_ptr<nn::message> recv = s2.recvmsg(1);
		REQUIRE(recv->size() == 1);
	}
	//	SECTION("can send and receive raw") {
	//		nn::socket s1(nn::socket_domain::sp, nn::socket_type::pair);
	//		REQUIRE_NOTHROW(s1.bind("inproc://test"));
	//
	//		nn::socket s2(nn::socket_domain::sp, nn::socket_type::pair);
	//		REQUIRE_NOTHROW(s2.connect("inproc://test"));
	//
	//		unsigned char *buf1, *buf2;
	//		buf1 = static_cast<unsigned char*>
	//			(nn_allocmsg(256, 0));
	//		for (int i = 0; i < 256; ++i)
	//			buf1[i] = static_cast<unsigned char>(i);
	//		REQUIRE(s1.send_raw(&buf1, NN_MSG, 0) == 256);
	//
	//		buf2 = nullptr;
	//		REQUIRE(s2.receive_raw(&buf2, NN_MSG, 0) == 256);
	//		for (int i = 0; i < 256; ++i)
	//			REQUIRE(static_cast<unsigned char>(i) == buf2[i]);
	//		nn_freemsg(buf2);
	//	}
}
