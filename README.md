# nanomsgpp (C++)

## Introduction

C++ client library for [nanomsg](https://github.com/nanomsg/nanomsg).

## Installation

The library uses autotools. Use the following commands to build and install libnanomsgpp:

```bash
$ git clone git://github.com/bigdatadev/libnanomsgpp.git
$ cd libnanomsgpp
$ ./autogen.sh
$ ./configure && make && make check && make install
```

## Getting Started

Create a socket and bind to a port:

```cpp
#include <nanomsgpp/nanomsgpp.hpp>

namespace nn = nanomsgpp;

int main() {
	try {
		nn::socket socket(nn::socket_domain::sp, nn::socket_type::reply);
		socket.bind("tcp://127.0.0.1:1234");
	} catch (nn::internal_exception &e) {
		std::cerr << "Error: " << e.error() << " - " << e.reason() << "." << std::endl;
		exit (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
```

Send a message:

```cpp
nn::message message;
message << "Hello, world!";
socket.sendmsg(message);
```

Receive a message:

```cpp
std::unique_ptr<nn::message> recv;
socket.recvmsg(1);

// print each message part
for (auto& part : recv) {
	std::cout << part.as<char>() << std::endl;
}
```

For more examples, refer to the following resources:
* [Tests](test/)
* [API Documentation](docs/API.md)
* [Command-Line Client](src/client/) - [Manual](docs/Client.md)

## Contributing

Just send me a pull request. Please take a look at the project issues and see how you can help. Here are some tips:
* please add more tests.
* please check your syntax.

## Author

Christopher Gilbert

* Web: [http://cjgilbert.me](http://cjgilbert.me)
* Twitter: [@bigdatadev](https://twitter.com/bigdatadev)
* Linkedin: [/in/christophergilbert](https://www.linkedin.com/in/christophergilbert)

## Copyright

See [LICENSE](LICENSE) document