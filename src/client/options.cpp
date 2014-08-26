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
connection_options()
{
	boost::program_options::options_description options("Connection Options");
	options.add_options()
		("bind,b", bpo::value<std::vector<std::string>>(), "bind to specified endpoint")
		("connect,c", bpo::value<std::vector<std::string>>(), "connect to specified endpoint")
		("verbose,v", "display output sent over socket to stderr")
		;
	return options;
}

bpo::options_description
miscellaneous_options()
{
	bpo::options_description options("Miscellaneous Options");
	options.add_options()
		("version", "display version")
		("help,?",  "show this help page")
	;
	return options;
}

options 
process_command_line(int argc, char const* argv[]) {
	bpo::positional_options_description arguments;
	arguments.add("type", 1);
	arguments.add("endpoint", 1);

	bpo::options_description all;
	all.add_options()
		("type", "nanomsg socket type")
		;
	all.add(miscellaneous_options());
	all.add(connection_options());

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
		// ...
	} catch (nn::internal_exception &e) {
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
	stream << "Usage: nanomsgpp [options] SOCKETTYPE ENDPOINT" << endl;
	stream << "nanomsg command line client." << endl;
	stream << "SOCKETTYPE is one of the supported nanomsg socket types." << endl;
	stream << "  pull, push, pub, sub, req, rep" << endl;
	stream << "ENDPOINT is any valid nanomsg endpoint." << endl;
	return stream;
}

ostream& 
show_help(ostream& stream) {
	stream << connection_options() << endl;
	stream << miscellaneous_options() << endl;
	return stream;
}
