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

#include <nanomsgpp/poller.hpp>
#include <nanomsgpp/socket.hpp>

namespace nn = nanomsgpp;

TEST_CASE("pollers can be manipulated", "[poller]") {
	nn::socket s1(nn::socket_domain::sp, nn::socket_type::pair);
	REQUIRE_NOTHROW(s1.bind("inproc://test"));

	nn::socket s2(nn::socket_domain::sp, nn::socket_type::pair);
	REQUIRE_NOTHROW(s2.connect("inproc://test"));

	SECTION("default constructor") {
		nn::poller poller;
	}
	SECTION("poll event receive") {
		nn::message m;
		s1.sendmsg(std::move(m));

		nn::poller poller;
		poller.add_socket(s2, nn::poll_event::in);
		REQUIRE(true == poller.poll(100));
	}
	SECTION("poll event receive timeout") {
		nn::poller poller;
		poller.add_socket(s2, nn::poll_event::in);
		REQUIRE(false == poller.poll(100));
	}
	SECTION("poll event send") {
		nn::poller poller;
		poller.add_socket(s1, nn::poll_event::out);
		REQUIRE(true == poller.poll());
	}
}
