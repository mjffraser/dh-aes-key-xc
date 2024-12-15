# Diffie-Hellman Key Exchange with AES Message Encryption

This is a toy implementation of the Diffie-Hellman (DH) key exchange protocol over sockets. DH is a method used to establish a symmetric cryptograhic key over an unsecure channel, so long as certain values are kept secret. This program doubles as a server/client all-in-one implementation, where a server instance opens a socket and waits for a client connection to start the key exchange. Once the client connects and the key is agreed on, it's put through a key-derivation function (scrypt) and an AES key is produced for encrypting the messages. Then, a messge of ASCII-printable characters is encrypted with AES-256 in Galaois Counter Mode, and sent to the server by the client over the unsecured connection.

## How it's vulnerable: ## 

This is written to demonstrate an example of [CWE-215: Insertion of Sensitive Information Into Debugging Code](https://cwe.mitre.org/data/definitions/215.html), a common software vulnerability where vital information is leaked due to code meant to assist in debugging. DH is a prime example of how leaking vital data could compromise the entire security of a given software. 

What constitutes sensitive information is infinitely broad, but in this case there are several instances of debugging code logging values that, if obtained, could allow an adversary to recompute the agreed upon DH key, and therefore recompute the agreed upon AES key & IV. Examples of weak portions of code and why are available in a writeup in `docs/exploit.md` along with exploit code to recompute the AES-key for an example leaked DH 'a' value. Instructions for how to compile this code are available in `docs/usage.md`.

## Firewall

If you have a firewall running and attempt to use this outside of the default loopback interface you'll most likely have to make an allowance for it in your firewall.

## Code Tabbing:

I prefer to use a tabbing of 2 spaces for my C/C++ code. If you don't adjust your Github appearance settings to match, certain code sections where I've done alignment with tabs could get *very* ugly. I'd recommend cloning and viewing the code in your own environment if possible.
