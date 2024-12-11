# Diffie-Hellman Key Exchange 

This is a toy implementation of the Diffie-Hellman (DH) key exchange protocol over sockets. DH is a method used to establish a symmetric cryptograhic session key over an unsecure channel, so long as certain information is properly kept secret. This program doubles as a server/client all-in-one implementation, where a server instance opens a socket and waits for a client connection to start the key exchange. ***Please DO NOT use this for any information that should actually be kept properly secret. Please DO use this to interact with and become familiar with DH as a concept.***


## !!! NOTE: YOU ARE ON THE SECURED BRANCH !!!

This is written in conjunction with a project on [CWE-215: Insertion of Sensitive Information Into Debugging Code](https://cwe.mitre.org/data/definitions/215.html), a common software vulnerability where vital information is leaked due to code meant to assist in debugging. DH is a prime example of how leaking vital data could compromise security of a given software. 

This software was written to intentionally include this vulnerability to demonstate it, and then was further corrected to guard against it. This branch contains the vulnerable code, and documentation about how it could potentially be exploited to break DH.

---

If you'd like to read more about CWE-215:

[Blog Post 1 Link]() \
[Blog Post 2 Link]() \
[Video Link]() 
