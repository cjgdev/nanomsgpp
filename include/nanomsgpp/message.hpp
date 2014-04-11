/*
 * Copyright (c) the libnanomsgpp contributors. All rights reserved.
 *
 * This file is a part of libnanomsgpp, distributed under the MIT
 * license. For full terms see the included COPYING file.
 */

#ifndef NANOMSGPP_MESSAGE_HPP_INCLUDED
#define NANOMSGPP_MESSAGE_HPP_INCLUDED

#include <iostream>

namespace nanomsgpp {

class message {
	std::ostream d_stream;
public:
	message() = default;

	message(const message &other) = default;

	message(message &&other) = default;

	~message() = default;

	message& operator=(const message &other) = default;

	message& operator=(message &&other) = default;

	template <typename T>
	message& operator<<(const T& t) {
		d_stream << t;
		return (*this);
	}
};

}

#endif