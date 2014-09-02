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

#ifndef NANOMSGPP_EXCEPTION_HPP_INCLUDED
#define NANOMSGPP_EXCEPTION_HPP_INCLUDED

#include <nanomsg/nn.h>

#include <stdexcept>
#include <string>

namespace nanomsgpp {

	// The base exception class for all nanomsgpp exceptions.
	class exception : public std::runtime_error {
	public:
		exception(std::string const& message)
			: std::runtime_error(message) {}
	};

	// The internal_exception class encapsulates errors caused by calls to
	// the nanomsg client library that return an error code. These errors are
	// identified by their error code given by nn_errno, and their associated
	// reason string deduced by nn_strerror.
	class internal_exception : public exception {
		int         d_error;
		std::string d_reason;
		
	public:
		// Default constructor.
		internal_exception()
			: exception(nn_strerror(nn_errno()))
			, d_error(nn_errno())
			, d_reason(nn_strerror(nn_errno())) {}

		// Constructor.
		internal_exception(const std::string& message)
			: exception(message)
			, d_error(nn_errno())
			, d_reason(nn_strerror(nn_errno())) {}

		// Destructor.
		~internal_exception() throw() {}

		// MANIPULATORS

		// Get the error code associated with this exception.
		int error() const { return d_error; }

		// Get the reason string describing this exception.
		std::string const& reason() const { return d_reason; }
	};

}

#endif
