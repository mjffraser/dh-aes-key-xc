#pragma once

#include "dh-aes-params.hpp"

#include <optional>

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * newSocket
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Opens a new socket for a client/server to use. If ConfigParams indicates
 *    this is a server, also sets up the socket as a listener.
 *
 * Takes:
 * -> params:
 *    A ConfigParams object to draw server address info, as well as the
 *    client/server designation of this process instance.
 *
 * Returns:
 * -> On success:
 *    The socket fd.
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int newSocket(ConfigParams const& params);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * doKeygen
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> With the agreeded upon Diffie-Hellman key, generates the AES key and IV to
 *    use, and returns the AESParams object housing those values. If this
 *    operation could not be completed, returns an error.
 *
 * Takes:
 * -> dh_v:
 *    The DHParams object with the agreed upon dh_key set.
 *
 * Returns:
 * -> On success:
 *    An AESParams object.
 * -> On failure:
 *    std::nullopt
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::optional<AESParams> doKeygen(DHParams const& dh_v);

namespace client {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * client::teardown
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Closes the clients socket.
 *
 * Takes:
 * -> The socket that was/is connected to the server.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void teardown(int server);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * connectToServer
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Connects the provided socket to a server's listening socket at the address
 *    in the provided ConfigParams object.
 *
 * Takes:
 * -> serv_sock:
 *    The socket to call connect() on.
 * -> params:
 *    The ConfigParams object with the server's listening socket address.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int connectToServer(int serv_sock, ConfigParams const& params);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * client::dhHandshake
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Performs the Diffie-Hellman handshake from the client's perspective. That
 *    is,
 *    -> Receives p & g from the server.
 *    -> Picks a, sends A.
 *    -> Receives B.
 *    -> Calculates dh_key.
 *    See dh-param-gen module for more about what these values correspond to.
 *
 * If this function fails, the socket is NOT closed.
 *
 * Takes:
 * -> server:
 *    A socket that's connected to the server.
 *    
 * Returns:
 * -> On success:
 *    A DHParams object with all fields populated.
 * -> On failure:
 *    std::nullopt
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::optional<DHParams> dhHandshake(int server);

} //client

namespace server {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * server::teardown
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Closes the servers listening socket. If there's an active client
 *    connection it's closed as well.
 *
 * Takes:
 * -> my_sock:
 *    The server's listening socket.
 * -> client:
 *    Optionally, the client's socket.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void teardown(int my_sock, int client=-1);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * acceptNewClient
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Accepts an incoming client connection (blocking), and returns the socket
 *    fd on success.
 *
 * Takes:
 * -> my_sock:
 *    The server's listening socket, as setup by newSocket().
 * -> params:
 *    The ConfigParams object to draw client/server designation from.
 *
 * Returns:
 * -> On success:
 *    The socket fd of the connected client.
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int acceptNewClient(int my_sock, ConfigParams const& params);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * server::dhHandshake
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Performs the Diffie-Hellman handshake from the server's perspective. That
 *    is,
 *    -> Picks a prime p and generator g with the config in params.
 *    -> Sends p & g to client.
 *    -> Receives A.
 *    -> Picks b, sends B.
 *    -> Calculate dh_key.
 *    See dh-param-gen module for more about what these values correspond to.
 *
 * If this function fails, the client socket is NOT closed.
 *
 * Returns:
 * -> On success:
 *    A DHParams object with all fields populated.
 * -> On failure:
 *    std::nullopt
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::optional<DHParams> dhHandshake(int client, ConfigParams const& params);

} //server

} //dh
