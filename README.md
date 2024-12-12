# Diffie-Hellman Key Exchange with AES Message Encryption

This is a toy implementation of the Diffie-Hellman (DH) key exchange protocol over sockets. DH is a method used to establish a symmetric cryptograhic key over an unsecure channel, so long as certain information is properly kept secret. This program doubles as a server/client all-in-one implementation, where a server instance opens a socket and waits for a client connection to start the key exchange. Once the key is agreed on, it's put through a key-derivation function (scrypt) and an AES key is produced for encrypting the messages. Then, a messge of ASCII-printable characters is encrypted with AES-256 in Galaois Counter Mode, and sent to the server over the unsecured connection. This system protects exclusively against passive attackers.

***Please DO NOT use this for any information that should actually be kept properly secret. Please DO use this to interact with and become familiar with DH as a concept.***


## How it's vulnerable: ## 


This is written to demonstrate an example of [CWE-215: Insertion of Sensitive Information Into Debugging Code](https://cwe.mitre.org/data/definitions/215.html), a common software vulnerability where vital information is leaked due to code meant to assist in debugging. DH is a prime example of how leaking vital data, for instance to verify key calculations as the software developer, could compromise the entire security of a given software. 

What constitutes sensitive information is infinitely broad, but in this case there are several instances of debugging code logging values that, if obtained, could allow an adversary to recompute the agreed upon DH key, and therefore recompute the agreed upon AES key & IV, as the method for doing that is all public in this repository. Examples of weak portions of code and why are available in a writeup in `docs/exploit.md` along with exploit code to recompute the AES-key for each example. Instructions for how to compile this code are available in `docs/usage.md`.

---

If you'd like to read more about CWE-215:

[Blog Post 1 Link]() \
[Blog Post 2 Link]() \
[Video Link]() 

## Code Tabbing:

I prefer to use a tabbing of 2 spaces for my C/C++ code. If you don't adjust your Github appearance settings to match, certain code sections where I've done alignment with tabs could get *very* ugly.
