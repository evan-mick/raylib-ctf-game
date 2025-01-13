
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>


typedef enum __attribute__((__packed__)) NetworkConnectionState {
    UNESTABLISHED = 0, 
    CONNECTED = 1, 
    DISCONNECTED = 2,
} NetworkConnectionState;

// Server
//  Wait for conn
//      on accept, add to fixed list of connections
//      if no room, don't accept 
//  Wait for messages
//      get full message
//      check state of server
//          See if command is valid for it


// Returns the UDP socket
int SetupUDPConnection(const char* address, const char* port, bool bind_sock, addrinfo** outinfo) {

    int sock;
    struct addrinfo hints, *servinfo;
    int rv;

    hints = {};
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP

    if (bind_sock)
        hints.ai_flags = AI_PASSIVE; // use my IP

    // Resolve the address and port
    if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
        return -1;
    }

    // Create a listening socket for the server on default port
    for (*outinfo = servinfo; *outinfo != nullptr; *outinfo = (*outinfo)->ai_next) {
        if ((sock = socket((*outinfo)->ai_family, (*outinfo)->ai_socktype,
                           (*outinfo)->ai_protocol)) < 0) {
            continue;
        }

        if (bind_sock && bind(sock, (*outinfo)->ai_addr, (*outinfo)->ai_addrlen) < 0) {
            close(sock);
            continue;
        }
        break;
    }

    if (*outinfo == nullptr) {
        return -2;
    }

    return sock;
}
