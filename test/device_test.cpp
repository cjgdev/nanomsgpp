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

#include <nanomsgpp/device.hpp>
#include <nanomsgpp/socket.hpp>

#include <thread>

namespace nn = nanomsgpp;

TEST_CASE("devices can be created", "[device]") {
	SECTION("default constructor") {
		nn::socket s1(nn::socket_domain::sp_raw, nn::socket_type::pair);
		REQUIRE_NOTHROW(s1.bind("inproc://testa"));

		nn::socket s2(nn::socket_domain::sp_raw, nn::socket_type::pair);
		REQUIRE_NOTHROW(s2.bind("inproc://testb"));

//		std::thread t1([&]() {
//			REQUIRE_THROWS(nn::device d(s1, s2));
//		});

		s2.close();
		s1.close();
//		t1.join();
	}
}
