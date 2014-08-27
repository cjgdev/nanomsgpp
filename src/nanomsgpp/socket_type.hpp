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

#ifndef NANOMSGPP_SOCKET_TYPE_HPP_INCLUDED
#define NANOMSGPP_SOCKET_TYPE_HPP_INCLUDED

#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/survey.h>
#include <nanomsg/bus.h>
#include <nanomsg/pair.h>

namespace nanomsgpp {

	enum class socket_domain : int {
		/**
		 * Standard full-blown SP socket.
		 */
		sp = AF_SP,

		/**
		 * Raw SP socket. Raw sockets omit the end-to-end functionality found in
		 * AF_SP sockets and thus can be used to implement intermediary devices
		 * in SP topologies.
		 */
		sp_raw = AF_SP_RAW,
	};

	enum class socket_type : int {
		/**
		 * Socket for communication with exactly one peer. Each party can send
		 * messages at any time. If the peer is not available or send buffer is full
		 * subsequent calls to nn_send(3) will block until it’s possible to send the
		 * message.
		 */
		pair       = NN_PAIR,

		/**
		 * Used to implement the client application that sends requests and receives
		 * replies.
		 */
		request    = NN_REQ,

		/**
		 * Used to implement the stateless worker that receives requests and sends
		 * replies.
		 */
		reply      = NN_REP,

		/**
		 * This socket is used to distribute messages to multiple destinations.
		 * Receive operation is not defined.
		 */
		publish    = NN_PUB,

		/**
		 * Receives messages from the publisher. Only messages that the socket is
		 * subscribed to are received. When the socket is created there are no
		 * subscriptions and thus no messages will be received. Send operation is
		 * not defined on this socket.
		 */
		subscribe  = NN_SUB,

		/**
		 * Used to send the survey. The survey is delivered to all the connected
		 * respondents. Once the query is sent, the socket can be used to receive
		 * the responses. When the survey deadline expires, receive will return
		 * ETIMEDOUT error.
		 */
		surveyor   = NN_PAIR,

		/**
		 * Use to respond to the survey. Survey is received using receive function,
		 * response is sent using send function. This socket can be connected to at
		 * most one peer.
		 */
		respondent = NN_PAIR,

		/**
		 * This socket is used to send messages to a cluster of load-balanced nodes.
		 * Receive operation is not implemented on this socket type.
		 */
		push       = NN_PUSH,

		/**
		 * This socket is used to receive a message from a cluster of nodes. Send
		 * operation is not implemented on this socket type.
		 */
		pull       = NN_PULL,

		/**
		 * Sent messages are distributed to all nodes in the topology. Incoming
		 * messages from all other nodes in the topology are fair-queued in the
		 * socket.
		 */
		bus        = NN_BUS,
	};

}

#endif
