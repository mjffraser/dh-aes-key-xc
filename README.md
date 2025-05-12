# Diffie-Hellman Key Exchange with AES_GCM Message Encryption

This is a toy implementation of the Diffie-Hellman (DH) key exchange protocol over POSIX sockets. DH is a method used to establish a symmetric cryptographic key over an unsecure channel, often used in combination with protocols like TLS or SSH. This program doubles as a server/client all-in-one implementation, where a server instance opens a socket and waits for a client connection to start the key exchange. Once the client connects and the key is agreed upon, it's put through a key-derivation function (scrypt) and a 256-bit AES key is produced for encrypting subsequent messages. An encrypted chat session then begins, and the client and server can exchange messages encrypted with AES-256 in Galaois Counter Mode over an unsecured connection.

## Usage:

For help building and using this software, see `docs/usage`

## Security

*This implementation is not secure.*

It is vulnerable primarily to man-in-the-middle (MITM) attacks, since this does not perform any authentication, an attacker could intercept g^a and g^b in transit, and impersonate a client/server. 

However, this software is not intended to be a secure end-end encrypted messager, but rather an example of the DH key exchange to understand its protocol. There's also no attempt made to secure memory for handling critical private values. This implementation is only secure for eavesdropping attacks on messages sent between secured machines over an unsecured network.

## General Security Recommendations:

TL;DR: 2048-bit keys are good enough. Anything more is unlikely to provide meaningful security improvements.

Long version: \
The estimated strength of various MODP groups from RFC3526 are as follows:
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
To combat this I made the work factor for scrypt intentionally large for the application, as it's makes it that much harder to precompute
pre-images due to the memory requirements. An attacker could also choose to skip scrypt and attempt to precompute 2^256 AES keys. This is 
probably still less practical than attacking DH here, but I'm no cryptographer.
