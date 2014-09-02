# Client Manual


## Name

nanomsgpp - a command-line interface to nanomsg


## Synopsis

nanomsgpp req {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-i SEC] [-AQ]
nanomsgpp rep {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-AQ]
nanomsgpp push {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-i SEC]
nanomsgpp pull {--connect=ADDR|--bind=ADDR} [-AQ]
nanomsgpp pub {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-i SEC]
nanomsgpp sub {--connect=ADDR|--bind=ADDR} [--subscribe=TOPIC ...] [-AQ]
nanomsgpp surveyor {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-i SEC] [-AQ]
nanomsgpp respondent {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-AQ]
nanomsgpp bus {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-i SEC] [-AQ]
nanomsgpp pair {--connect=ADDR|--bind=ADDR} {--data=DATA|--file=PATH} [-i SEC] [-AQ]


## Description

The nanomsgpp client is a command-line tool to send and receive data via nanomsg sockets. It can be
used for debugging, and testing nanomsgpp.


## Options

### Miscellaneous Options:
* `-v [ --verbose ]` increase verbosity
* `--version` display version
* `-? [ --help ]` show this help page

### Socket Options:
* `-b [ --bind ] arg` bind to specified endpoint
* `-c [ --connect ] arg` connect to specified endpoint
* `--recv-timeout arg` timeout for receiving a message
* `--send-timeout arg` timeout for sending a message

### SUB Socket Options:
* `--subscribe arg` subscribe to a topic. note: socket will be subscribed to everything if no topics 
  are specified

### Input Options:
* `--format arg` use echo format
* `--raw` dump message as is
* `-A [ --ascii ]` print ascii part of message delimeted by newline
* `-Q [ --quoted ]` print each message on separate line in double quotes with C-like character 
  escaping

### Output Options:
* `-i [ --interval ] arg1 send message (or request) every number of seconds
* `-d [ --delay ] arg` wait for number of seconds before sending message
* `-D [ --data ] arg` send data to the socket and quit
* `-F [ --file ] arg` same as --data but get data from file


## Examples

### Ping

```bash
$ nanomsgpp rep --bind=tcp://*:1234 --data=pong --format=ascii
$ nanomsgpp req --connect=tcp://localhost:1234 --data=ping --format=ascii
```

### Request once per second

```bash
$ nanomsgpp req --connect=tcp://localhost:1234 --data=ping --format=ascii --interval=1
```

### Redirect output to push socket

```bash
$ ls | nanomsgpp push --connect://tcp://localhost:1234 --file=-
```

## Further Reading

* [nanomsg](https://github.com/nanomsg/nanomsg)
* Read the client code and tests.
