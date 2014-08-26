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

#ifndef NANOMSGPP_SOCKET_OPTION_HPP_INCLUDED
#define NANOMSGPP_SOCKET_OPTION_HPP_INCLUDED

#include <nanomsg/nn.h>

namespace nanomsgpp {

enum class socket_option {
	/**
	 * Specifies how long should the socket try to send pending outbound 
	 * messages after nn_close() have been called, in milliseconds. Negative 
	 * value means infinite linger. The type of the option is int. Default value 
	 * is 1000 (1 second).
	 */
	linger                 = NN_LINGER,

	/**
	 * Size of the send buffer, in bytes. To prevent blocking for messages 
	 * larger than the buffer, exactly one message may be buffered in addition 
	 * to the data in the send buffer. The type of this option is int. Default 
	 * value is 128kB.
	 */
	send_buffer            = NN_SNDBUF,

	/**
	 * Size of the receive buffer, in bytes. To prevent blocking for messages 
	 * larger than the buffer, exactly one message may be buffered in addition 
	 * to the data in the receive buffer. The type of this option is int. 
	 * Default value is 128kB.
	 */
	receive_buffer         = NN_RCVBUF,

	/**
	 * The timeout for send operation on the socket, in milliseconds. If message 
	 * cannot be sent within the specified timeout, EAGAIN error is returned. 
	 * Negative value means infinite timeout. The type of the option is int. 
	 * Default value is -1.
	 */
	send_timeout           = NN_SNDTIMEO,

	/**
	 * The timeout for recv operation on the socket, in milliseconds. If message 
	 * cannot be received within the specified timeout, EAGAIN error is 
	 * returned. Negative value means infinite timeout. The type of the option 
	 * is int. Default value is -1.
	 */
	receive_timeout        = NN_RCVTIMEO,

	/**
	 * For connection-based transports such as TCP, this option specifies how 
	 * long to wait, in milliseconds, when connection is broken before trying to 
	 * re-establish it. Note that actual reconnect interval may be randomised to 
	 * some extent to prevent severe reconnection storms. The type of the option 
	 * is int. Default value is 100 (0.1 second).
	 */
	reconnect_interval     = NN_RECONNECT_IVL,

	/**
	 * This option is to be used only in addition to NN_RECONNECT_IVL option. It 
	 * specifies maximum reconnection interval. On each reconnect attempt, the 
	 * previous interval is doubled until NN_RECONNECT_IVL_MAX is reached. Value 
	 * of zero means that no exponential backoff is performed and reconnect 
	 * interval is based only on NN_RECONNECT_IVL. If NN_RECONNECT_IVL_MAX is 
	 * less than NN_RECONNECT_IVL, it is ignored. The type of the option is int. 
	 * Default value is 0.
	 */
	reconnect_interval_max = NN_RECONNECT_IVL_MAX,

	/**
	 * Sets outbound priority for endpoints subsequently added to the socket. 
	 * This option has no effect on socket types that send messages to all the 
	 * peers. However, if the socket type sends each message to a single peer 
	 * (or a limited set of peers), peers with high priority take precedence 
	 * over peers with low priority. The type of the option is int. Highest 
	 * priority is 1, lowest priority is 16. Default value is 8.
	 */
	send_priority          = NN_SNDPRIO,

	/**
	 * If set to 1, only IPv4 addresses are used. If set to 0, both IPv4 and 
	 * IPv6 addresses are used. The type of the option is int. Default value is 
	 * 1.
	 */
	ipv4_only              = NN_IPV4ONLY,

	/**
	 * Socket name for error reporting and statistics. The type of the option is 
	 * string. Default value is "socket.N" where N is socket integer. This 
	 * option is experimental, see nn_env(7) for details
	 */
	socket_name            = NN_SOCKET_NAME
};

}

#endif