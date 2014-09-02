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

#include <nanomsgpp/socket.hpp>

namespace nn = nanomsgpp;

TEST_CASE("sockets can be manipulated", "[socket]") {
	SECTION("construct with existing socket") {
		nn::socket socket(nn_socket(
				static_cast<int>(nn::socket_domain::sp),
				static_cast<int>(nn::socket_type::request)
		));
		REQUIRE(socket.get_fd() >= 0);
	}
	SECTION("constructor") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);
		REQUIRE(socket.get_fd() >= 0);
	}
	SECTION("connect and close") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);
		REQUIRE_NOTHROW(socket.connect("ipc://test"));
		REQUIRE_NOTHROW(socket.close());
		REQUIRE(socket.get_fd() < 0);
	}
	SECTION("get and set socket option") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);

		socket.set_option(NN_SOL_SOCKET, nn::socket_option::linger, 1000);
		REQUIRE(socket.get_option<int>(NN_SOL_SOCKET, nn::socket_option::linger) == 1000);

		socket.set_option(NN_SOL_SOCKET, nn::socket_option::socket_name, "nanomsg");
		REQUIRE(socket.get_option<std::string>(NN_SOL_SOCKET, nn::socket_option::socket_name) == "nanomsg");
	}
	SECTION("get and set tcp option") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);

		socket.set_option(NN_TCP, nn::socket_option::tcp_nodelay, 1);
		REQUIRE(socket.get_option<int>(NN_TCP, nn::socket_option::tcp_nodelay) == 1);
	}
	SECTION("get and set reqrep option") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);

		socket.set_option(NN_REQ, nn::socket_option::request_resend_interval, 60000);
		REQUIRE(socket.get_option<int>(NN_REQ, nn::socket_option::request_resend_interval) == 60000);
	}
	SECTION("get and set pubsub option") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::subscribe);

		socket.set_option(NN_SUB, nn::socket_option::sub_subscribe, "topic_a");
		socket.set_option(NN_SUB, nn::socket_option::sub_unsubscribe, "topic_a");
	}
//	SECTION("get and set surveyor option") {
//		nn::socket socket(nn::socket_domain::sp, nn::socket_type::surveyor);
//		socket.set_option(NN_SURVEYOR, nn::socket_option::surveyor_deadline, 1000);
//		REQUIRE(socket.get_option<int>(NN_SURVEYOR, nn::socket_option::surveyor_deadline) == 1000);
//	}
	SECTION("bind and shutdown") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);

		REQUIRE_THROWS(socket.bind("invalid"));
		REQUIRE_THROWS(socket.shutdown("invalid"));

		REQUIRE_NOTHROW(socket.bind("tcp://*:3000"));
		REQUIRE_NOTHROW(socket.shutdown("tcp://*:3000"));
	}
	SECTION("connect and shutdown") {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::request);

		REQUIRE_THROWS(socket.connect("invalid"));
		REQUIRE_THROWS(socket.shutdown("invalid"));

		REQUIRE_NOTHROW(socket.connect("tcp://localhost:3000"));
		REQUIRE_NOTHROW(socket.shutdown("tcp://localhost:3000"));
	}
}
