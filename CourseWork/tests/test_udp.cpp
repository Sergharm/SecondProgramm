#include "FileDescriptor.h"

#include <cstdio>

#include <netinet/in.h>
#include <sys/socket.h>

int main() {
    const auto fail = [](const char* operation) {
        std::perror(operation);
        return 1;
    };
    const FileDescriptor server(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
    const FileDescriptor client(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
    if (server.get() < 0 || client.get() < 0) return fail("socket UDP");

    // Ядро выбирает свободный порт на loopback.
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(server.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0)
        return fail("bind UDP");
    socklen_t length = sizeof(address);
    if (getsockname(server.get(), reinterpret_cast<sockaddr*>(&address), &length) < 0)
        return fail("getsockname UDP");

    constexpr char sent = 'U';
    if (sendto(client.get(), &sent, 1, 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) != 1)
        return fail("sendto UDP");
    char received = 0;
    sockaddr_in peer{};
    length = sizeof(peer);
    if (recvfrom(server.get(), &received, 1, 0, reinterpret_cast<sockaddr*>(&peer), &length) != 1)
        return fail("recvfrom UDP");
    return received == sent && peer.sin_addr.s_addr == htonl(INADDR_LOOPBACK) ? 0 : 1;
}
