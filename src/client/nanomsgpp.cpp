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

#include "options.hpp"
#include <nanomsgpp/nanomsgpp.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace client;
using namespace std;

namespace nn = nanomsgpp;

nn::socket&&
create_socket(const options& ops) {
	nn::socket socket(nn::socket_domain::sp, ops.get_type());
	socket.set_option(NN_SOL_SOCKET, nn::socket_option::send_timeout, ops.send_timeout * 1000);
	socket.set_option(NN_SOL_SOCKET, nn::socket_option::receive_timeout, ops.recv_timeout);
	if (ops.get_type() == nn::socket_type::subscribe) {
		for (auto& subscription : ops.subscribe) {
			socket.set_option(NN_SUB, nn::socket_option::sub_subscribe, subscription);
		}
	}
	return std::move(socket);
}

void
connect_socket(nn::socket& socket, const options& ops) {
	for (auto& addr : ops.bind) {
		socket.bind(addr);
	}
	for (auto& addr : ops.connect) {
		socket.connect(addr);
	}
}

void
print_message(std::unique_ptr<nn::message>& msg, const options& ops) {
	for (auto& part : *msg) {
		switch (ops.get_format()) {
		case echo_format::none:
			return;
		case echo_format::raw:
			break;
		case echo_format::ascii:
			break;
		case echo_format::quoted:
			break;
		case echo_format::msgpack:
			break;
		}
	}
}

void
send_loop(nn::socket& socket, const options& ops) {
	std::string data = ops.get_data();
	while (true) {
		try {
			nn::message m;
			//m << data;
			socket.sendmsg(std::move(m));
		} catch (nn::internal_exception &e) {
			if (e.error() == EAGAIN) {
				cerr << "Message not sent (EAGAIN)" << endl;
			} else {
				throw (e);
			}
		}
		if (ops.interval >= 0) {
			std::this_thread::sleep_for(std::chrono::seconds(ops.interval));
		} else {
			break;
		}
	}
}

void
recv_loop(nn::socket& socket, const options& ops) {
	while (true) {
		try {
			std::unique_ptr<nn::message> m = socket.recvmsg(1);
			print_message(m, ops);
		} catch (nn::internal_exception &e) {
			if (e.error() == EAGAIN) {
				continue;
			} else if (e.error() == ETIMEDOUT || e.error() == EFSM) {
				return;
			} else {
				throw (e);
			}
		}
	}
}

void
rw_loop(nn::socket& socket, const options& ops) {
	std::string data = ops.get_data();
	while (true) {
		try {
			nn::message m;
			//m << data;
			socket.sendmsg(std::move(m));
		} catch (nn::internal_exception &e) {
			if (e.error() == EAGAIN) {
				cerr << "Message not sent (EAGAIN)" << endl;
			} else {
				throw (e);
			}
		}
		if (ops.interval < 0) {
			recv_loop(socket, ops);
		}

		while (true) {
			try {
				std::unique_ptr<nn::message> m = socket.recvmsg(1);
				print_message(m, ops);
			} catch (nn::internal_exception &e) {
				if (e.error() == EAGAIN) {
					continue;
				} else if (e.error() == ETIMEDOUT || e.error() == EFSM) {
					std::this_thread::sleep_for(std::chrono::seconds(ops.interval));
				}
			}
		}
	}
}

void
resp_loop(nn::socket& socket, const options& ops) {
	std::string data = ops.get_data();
	while (true) {
		try {
			std::unique_ptr<nn::message> m = socket.recvmsg(1);
			print_message(m, ops);
		} catch (nn::internal_exception &e) {
			if (e.error() == EAGAIN) {
				continue;
			} else {
				throw (e);
			}
		}
		try {
			nn::message m;
			//m << data;
			socket.sendmsg(std::move(m));
		} catch (nn::internal_exception &e) {
			if (e.error() == EAGAIN) {
				cerr << "Message not sent (EAGAIN)" << endl;
			} else {
				throw (e);
			}
		}
	}
}

int main(int argc, char const* argv[]) {
	options const ops = process_command_line(argc, argv);
	if (ops.show_version) {
		cout << "nanomsgpp version 0.1.0" << endl;
		return EXIT_FAILURE;
	}
	if (ops.show_usage || ops.show_help) {
		show_usage(cout);
		if (ops.show_help) {
			cout << endl << show_help(cout);
		}
		return EXIT_FAILURE;
	}
	try {
		nn::socket socket(create_socket(ops));
		connect_socket(socket, ops);
		if (ops.delay != -1) {
			std::this_thread::sleep_for(std::chrono::seconds(ops.delay));
		}
		switch (ops.get_type()) {
		case nn::socket_type::publish:
		case nn::socket_type::push:
			send_loop(socket, ops);
			break;
		case nn::socket_type::subscribe:
		case nn::socket_type::pull:
			recv_loop(socket, ops);
			break;
		case nn::socket_type::bus:
		case nn::socket_type::pair:
			if (!ops.get_data().empty()) {
				rw_loop(socket, ops);
			} else {
				recv_loop(socket, ops);
			}
			break;
		case nn::socket_type::surveyor:
		case nn::socket_type::request:
			rw_loop(socket, ops);
			break;
		case nn::socket_type::reply:
		case nn::socket_type::respondent:
			if (!ops.get_data().empty()) {
				resp_loop(socket, ops);
			} else {
				recv_loop(socket, ops);
			}
			break;
		}
	} catch (std::invalid_argument &e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	} catch (nn::internal_exception &e) {
		cerr << e.reason() << endl;
		return EXIT_FAILURE;
	} catch (...) {
		cerr << "Unknown exception" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
