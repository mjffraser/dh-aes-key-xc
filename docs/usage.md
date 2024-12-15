<!-- If you're viewing this without markdown formatting disabling linewrap can help to view the tables. --->
# DH Key Agreement with AES-GCM Message Encryption:

Windows support is unknown. Linux should work fine on any modern kernel with the following:

### Required:
> C++17 \
> CMake ver. >3.28 \
> C++ Boost (multiprecision::cpp_int & random libraries)\
> OpenSSL (libcrypto)

### Setup:
```
$ mkdir build && cd build 
$ cmake ..
$ make
$ ./dh-key-xc <args>
```

### Args:

| full      | short | description                                                                                | operand-type | default   |
| --------- | ----- | ------------------------------------------------------------------------------------------ | ------------ | --------- |
| --help    | -h    | Overrides all other flags. Exits with a help menu.                                         | N/A          | false     |
| --server  | -s    | If this is a server instance.                                                              | N/A          | N/A       |
| --client  | -c    | If this is a client instance                                                               | N/A          | N/A       |
| --quiet   | -q    | If messages that would normally be printed to the console should be suppressed.            | N/A          | false     |
| --verbose | -v    | If all messages that would be logged should also be printed to console.                    | N/A          | false     |
| --log     | N/A   | Specify the name of the log file to write to. Accepts a relative or absolute path.         | string       | ./log     |
| --bits    | N/A   | Specify bits to use for DH's prime p. Should be one of 1536, 2048, 3072, 4096, 6144, 8192. | int          | 2048      |
| --ip      | N/A   | Specify the IP address to bind/connect to.                                                 | string       | 127.0.0.1 |
| --port    | N/A   | Specify the port to use/connect to. Disallows first 1024 ports.                            | int          | 65000     |

All short options can be combined into one as they require no operand. Ex:
> ./dh-key-xc -sdv

All long options should be immediatly followed by the required operand:
> ./dh-key-xc -sdv --port 8080

The only option that's required is to select either client or server, not both. 

#### WARNING:
If you run a client and server, both running in the same working directory, you **MUST** set the `--log` option for *either* the client or 
server (or both). 

Otherwise the client instance will most likely fail to open the default `log` file as the server most likely already has it open.

### Security & Recommendations:

TL;DR: 2048-bit keys are good enough. Anything more is unlikely to provide meaningful security improvements.

The estimated strength of the various MODP Groups from RFC3526 are as follows:
| modulus  | estimate 1 | estimate 2 |
| -------- | ---------- | ---------- |
| 1536-bit | 90-bits    | 120-bits   |
| 2048-bit | 110-bits   | 160-bits   |
| 3072-bit | 130-bits   | 210-bits   |
| 4096-bit | 150-bits   | 240-bits   |
| 6144-bit | 170-bits   | 270-bits   |
| 8192-bit | 190-bits   | 310-bits   |

The actual expected strength is estimated to be somewhere between the two estimates, there's no 100% consensus among cryptographers.

This means that the limiting factor for the security of our encryption is the strength of the DH-key, not the result of the KDF. 
To combat this I made the work factor for Scrypt intentionally large for the application, as it's makes it that much harder to precompute
pre-images due to the memory requirements. An attacker could also choose to skip Scrypt and attempt to precompute 2^256 AES keys. This is 
probably still less practical than attacking DH here, but I'm no cryptographer.

Beside the glaring issues with key values being logged, values being written to unsecured memory is probably the biggest security hole to 
this protocol. For the purposes of this protocol it assumes that we're transferring messages between two secured machines, over an unsecured 
communication line to passive attackers. This program also makes absolutely ***ZERO*** attempts to protect against active attacks like 
Man-in-the-Middle.
