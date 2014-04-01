/*
 * Copyright (c) the libnanomsgpp contributors. All rights reserved.
 *
 * This file is a part of libnanomsgpp, distributed under the MIT
 * license. For full terms see the included COPYING file.
*/

#ifndef NANOMSGPP_SOCKET_TYPE_HPP_INCLUDED
#define NANOMSGPP_SOCKET_TYPE_HPP_INCLUDED

#include <nanomsg/nn.h>

namespace nanomsgpp {

enum class socket_option {
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