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

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace bpo = boost::program_options;
namespace nm = nanomsgpp;

using namespace client;
using namespace std;

#define HAVE_JSON 0

bpo::options_description 
send_options()
{
	bpo::options_description options("Send Command Options");
	options.add_options()
		( "state,s",         bpo::value<string>(),  "Set the state of the event." )
		( "service,S",       bpo::value<string>(),  "Set the service sending the event." )
		( "host,h",          bpo::value<string>(),  "Set the origin host of the event." )
		( "description,D",   bpo::value<string>(),  "Set the description of the event." )
		( "attribute,a",     bpo::value<string>(),  "Add a new attribute to the event." )
		( "tag,t",           bpo::value<string>(),  "Add a tag to the event." )
		( "metric-sint64,i", bpo::value<int64_t>(), "Set the 64-bit integer metric of the event." )
		( "metric-d,d",      bpo::value<double>(),  "Set the double metric of the event." )
		( "metric-f,f",      bpo::value<float>(),   "Set the float metric of the event." )
		( "ttl,L",           bpo::value<float>(),   "Set the TTL of the event." )
		( "tcp,T",           bpo::value<bool>()->default_value(true), "Send the message over TCP (default)." )
		( "udp,U",           bpo::value<bool>(),    "Send the message over UDP." )
		;
	return options;
}

bpo::options_description
query_options() {
	bpo::options_description options("Query Command Options");
	options.add_options()
		( "query,q", bpo::value<string>(), "Query to send to Riemann." )
#if HAVE_JSON
		( "json,j",  "Output the results as a JSON array." )
#endif
		;
	return options;
}

bpo::options_description
miscellaneous_options() {
	bpo::options_description options("Miscellaneous Options");
	options.add_options()
		( "version,V", "Display version." )
		( "help,?",    "Show this help page." )
		;
	return options;
}

void
process_command_send(const bpo::variables_map& vm) {
	rpp::client_type type = rpp::client_type::tcp;
	if (vm.count("tcp")) {
		type = (vm["tcp"].as<bool>()) ? rpp::client_type::tcp : rpp::client_type::udp;
	} else if (vm.count("udp")) {
		type = (vm["udp"].as<bool>()) ? rpp::client_type::udp : rpp::client_type::tcp;
	}

	rpp::event e;
	e << "riemannpp-client" << "example:send-events";
	e.attribute_add(rpp::attribute("x-client", "riemannpp"));

	if (vm.count("state")) {
		e.set_state(vm["state"].as<string>());
	}
	if (vm.count("service")) {
		e.set_state(vm["service"].as<string>());
	}
	if (vm.count("host")) {
		e.set_state(vm["host"].as<string>());
	}
	if (vm.count("description")) {
		e.set_description(vm["description"].as<string>());
	}
	if (vm.count("attribute")) {
		string tmp = vm["attribute"].as<string>();
		if (size_t it = tmp.find('=') != string::npos) {
			e.attribute_add(rpp::attribute(tmp.substr(0, (it+2)), tmp.substr(it+3)));
		} else {
			e.attribute_add(rpp::attribute(tmp, ""));
		}
	}
	if (vm.count("tag")) {
		e.tag_add(vm["tag"].as<string>());
	}
	if (vm.count("metric-sint64")) {
		e.set_metric(vm["metric-sint64"].as<int64_t>());
	}
	if (vm.count("metric-dbl")) {
		e.set_metric(vm["metric-dbl"].as<double>());
	}
	if (vm.count("metric-flt")) {
		e.set_metric(vm["metric-flt"].as<float>());
	}
	if (vm.count("ttl")) {
		e.set_ttl(vm["ttl"].as<float>());
	}

	rpp::client client(type, vm["rhost"].as<string>(), vm["rport"].as<int>());
	client << std::move(e);

	std::unique_ptr<rpp::message> response;
	client >> response;

	if (!(riemann_message_t*)(*response)) {
		cerr << "Error when asking for a message receipt: " << strerror(errno) << endl;
	} else if (!response->get_ok()) {
		cerr << "Message receipt failed: " << response->get_error() << endl;
	}
}

void
process_command_query(const bpo::variables_map& vm) {
	rpp::client client;
	client.connect(rpp::client_type::tcp, vm["rhost"].as<string>(), vm["rport"].as<int>());

	rpp::query query;
	if (vm.count("query")) {
		query.set_string(vm["query"].as<string>());
	}
	client.send_oneshot(std::move(query));

#if HAVE_JSON
	if (vm.count("json")) {

	}
#endif

	std::unique_ptr<rpp::message> response(client.recv());

	if (!(riemann_message_t*)(*response)) {
		cerr << "Error when asking for a message receipt: " << strerror(errno) << endl;
	} else if (!response->get_ok()) {
		cerr << "Message receipt failed: " << response->get_error() << endl;
	} else {
		cout << *response;
	}
}

options 
process_command_line(int argc, char const* argv[]) {
	bpo::positional_options_description arguments;
	arguments.add("command", 1);
	arguments.add("rhost", 1);
	arguments.add("rport", 1);

	bpo::options_description all;
	all.add(miscellaneous_options());
	all.add_options()
		( "command", bpo::value<string>(), "Either query or send" )
		;
	all.add(send_options());
	all.add(query_options());
	all.add_options()
		( "rhost", bpo::value<string>()->default_value("localhost"), "The address of the Riemann server (default: localhost)" )
		( "rport", bpo::value<int>()->default_value(5555),           "The port of the Riemann server (default: 5555)" )
		;

	bpo::variables_map vm;
	options ops;

	try {
		bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(arguments).run(), vm);
	} catch(bpo::too_many_positional_options_error& e) {
		cerr << "Too many arguments provided." << endl;
		ops.show_usage = true;
	} catch(bpo::unknown_option& e) {
		cerr << "Unknown option '" << e.get_option_name() << "'." << endl;
		ops.show_usage = true;
	} catch(bpo::error& e) {
		cerr << "command line parse error: " << e.what() << "'." << endl;
		ops.show_usage = true;
	}

	if ((0 == vm.count("command")) || vm.count("help")) {
		ops.show_usage = true;
	} if (vm.count("command") && (vm["command"].as<string>() == string("query")) && (0 == vm.count("query"))) {
		ops.show_usage = true;
	}

	ops.show_version = (vm.count("version") > 0);
	ops.show_help = (vm.count("help") > 0);

	try {
		if (vm.count("command")) {
			if (vm["command"].as<string>() == "send") {
				process_command_send(vm);
			} else if (vm["command"].as<string>() == "query") {
				process_command_query(vm);
			}
		}
	} catch (rpp::internal_exception &e) {
		cerr << "Error: " << e.error() << " - " << e.reason() << "." << endl;
		ops.show_usage = true;
	} catch (std::exception &e) {
		cerr << "Error: " << e.what() << endl;
		ops.show_usage = true;
	} catch (...) {
		cerr << "Unknown error." << endl;
		ops.show_usage = true;
	}
	return ops;
}

ostream& 
show_usage(ostream& stream) {
	stream << "Usage: nanomsgpp COMMAND [options...] [HOST] [PORT]" << endl;
	stream << endl;
	stream << "The HOST and PORT arguments are optional, and they default to" << endl;
	stream << "\"localhost\" and 5555, respectively." << endl;
	stream << endl;
	stream << "Command can be either `send` or `query`." << endl;
	return stream;
}

ostream& 
show_help(ostream& stream) {
	stream << "Available commands: send and query." << endl;
	stream << send_options() << endl;
	stream << endl;
	stream << query_options() << endl;
	stream << endl;
	stream << miscellaneous_options() << endl;
	return stream;
}
