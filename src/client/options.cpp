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
namespace nn = nanomsgpp;

using namespace client;
using namespace std;

#define HAVE_JSON 0

bpo::options_description
socket_options()
{
	boost::program_options::options_description options("Socket Options");
	options.add_options()
		("bind,b",    bpo::value<std::vector<std::string>>(), "bind to specified endpoint")
		("connect,c", bpo::value<std::vector<std::string>>(), "connect to specified endpoint")
		("recv-timeout", "timeout for receiving a message")
		("send-timeout", "timeout for sending a message")
	;
	return options;
}

bpo::options_description
sub_socket_options()
{
	boost::program_options::options_description options("SUB Socket Options");
	options.add_options()
		("subscribe", "subscribe to a topic. note: socket will be subscribed to everything if no topics are specified")
	;
	return options;
}

bpo::options_description
input_options()
{
	boost::program_options::options_description options("Input Options");
	options.add_options()
		("format",   "use echo format")
		("raw",      "dump message as is")
		("ascii,A",  "print ascii part of message delimeted by newline")
		("quoted,Q", "print each message on separate line in double quotes with C-like character escaping")
	;
	return options;
}

bpo::options_description
output_options()
{
	boost::program_options::options_description options("Output Options");
	options.add_options()
		("interval,i", "send message (or request) every number of seconds")
		("delay,d",    "wait for number of seconds before sending message")
		("data,D",     "send data to the socket and quit")
		("file,F",     "same as --data but get data from file")
	;
	return options;
}

bpo::options_description
miscellaneous_options()
{
	bpo::options_description options("Miscellaneous Options");
	options.add_options()
		("verbose,v", "increase verbosity")
		("version",   "display version")
		("help,?",    "show this help page")
	;
	return options;
}

options 
process_command_line(int argc, char const* argv[]) {
	bpo::positional_options_description arguments;
	arguments.add("type", 1);

	bpo::options_description all;
	all.add_options()
		("type", "nanomsg socket type")
	;
	all.add(miscellaneous_options());
	all.add(socket_options());
	all.add(sub_socket_options());
	all.add(input_options());
	all.add(output_options());

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

	if ((0 == vm.count("type")) || vm.count("help") || (0 == vm.count("connect") && 0 == vm.count("bind"))) {
		ops.show_usage = true;
	}

	ops.show_version = (vm.count("version") > 0);
	ops.show_help = (vm.count("help") > 0);
	ops.verbose = (vm.count("verbose") > 0);

	if (vm.count("type")) {
		// options.type = vm["type"].as<std::string>();
	}
	if (vm.count("bind")) {
		// options.binds = vm["bind"].as<std::vector<std::string>>();
	}
	if (vm.count("connect")) {
		// options.connects = vm["connect"].as<std::vector<std::string>>();
	}

	return ops;
}

ostream& 
show_usage(ostream& stream) {
	stream << "nanomsg command line tool usage:" << endl;
	stream << "  nanomsgpp req {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-i SEC] [-AQ]" << endl;
	stream << "  nanomsgpp rep {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-AQ]" << endl;
	stream << "  nanomsgpp push {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-i SEC]" << endl;
	stream << "  nanomsgpp pull {--connect ADDR|--bind ADDR} [-AQ]" << endl;
	stream << "  nanomsgpp pub {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-i SEC]" << endl;
	stream << "  nanomsgpp sub {--connect ADDR|--bind ADDR} [--subscribe TOPIC ...] [-AQ]" << endl;
	stream << "  nanomsgpp surveyor {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-i SEC] [-AQ]" << endl;
	stream << "  nanomsgpp respondent {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-AQ]" << endl;
	stream << "  nanomsgpp bus {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-i SEC] [-AQ]" << endl;
	stream << "  nanomsgpp pair {--connect ADDR|--bind ADDR} {--data DATA|--file PATH} [-i SEC] [-AQ]" << endl;
	return stream;
}

ostream& 
show_help(ostream& stream) {
	stream << miscellaneous_options() << endl;
	stream << socket_options() << endl;
	stream << sub_socket_options() << endl;
	stream << input_options() << endl;
	stream << output_options() << endl;
	return stream;
}
