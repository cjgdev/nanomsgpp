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

#include "nanomsgpp/poller.hpp"
#include "nanomsgpp/exception.hpp"
#include <nanomsg/nn.h>

using namespace nanomsgpp;

poller::poller() {}

void
poller::add_socket(socket& s, poll_event e) {
	nn_pollfd pfd = { s.get_fd(), (short)e, 0 };
	d_pollfds.push_back(std::move(pfd));
}

void
poller::remove_socket(socket& s) {
	auto it = d_pollfds.begin();
	for (; it != d_pollfds.end(); it++) {
		if ((*it).fd == s.get_fd()) {
			break;
		}
	}
	if (it != d_pollfds.end()) { d_pollfds.erase(it); }
}

bool poller::poll(int timeout) {
	int rc = nn_poll(d_pollfds.data(), d_pollfds.size(), timeout);
	if (-1 == rc) {
		throw internal_exception();
	} else if (rc > 0) {
		return (true);
	}
	return (false);
}

bool
poller::has_event(socket& s, poll_event e) {
	for (auto& pfd : d_pollfds) {
		if (pfd.fd == s.get_fd()) {
			return ((pfd.revents & (short)e) != 0);
		}
	}
	return (false);
}
