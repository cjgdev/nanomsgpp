/*
 * Copyright (c) the libnanomsgpp contributors. All rights reserved.
 *
 * This file is a part of libnanomsgpp, distributed under the MIT
 * license. For full terms see the included COPYING file.
*/

#ifndef NANOMSGPP_EXCEPTION_HPP_INCLUDED
#define NANOMSGPP_EXCEPTION_HPP_INCLUDED

namespace nanomsgpp {

class exception : public std::exception {
	int _err;

public:
	exception();

	virtual const char *what() const throw();

	int nn_error() const;
};

}

#endif