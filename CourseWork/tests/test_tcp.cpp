#include "FileDescriptor.h"

#include <cerrno>
#include <cstdio>

#include <netinet/in.h>
#include <sys/socket.h>

int main() {
    const auto fail = [](const char* operation) {
        std::perror(operation);
        return 1;
    };

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    {
        const FileDescriptor listener(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        const FileDescriptor client(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        if (listener.get() < 0 || client.get() < 0) return fail("socket TCP");
        if (bind(listener.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0)
            return fail("bind TCP");
        socklen_t length = sizeof(address);
        if (getsockname(listener.get(), reinterpret_cast<sockaddr*>(&address), &length) < 0)
            return fail("getsockname TCP");
        if (listen(listener.get(), 1) < 0) return fail("listen TCP");
        if (connect(client.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0)
            return fail("connect TCP");
        sockaddr_in peer{};
        length = sizeof(peer);
        const FileDescriptor server(accept(listener.get(), reinterpret_cast<sockaddr*>(&peer), &length));
        if (server.get() < 0) return fail("accept TCP");

        constexpr char sent = 'T';
        char received = 0;
        if (send(client.get(), &sent, 1, MSG_NOSIGNAL) != 1) return fail("send TCP");
        if (recv(server.get(), &received, 1, 0) != 1 || received != sent) return 1;
    }

    // После закрытия слушающего сокета ожидается отказ на том же порту.
    const FileDescriptor retry(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if (retry.get() < 0) return fail("socket TCP");
    if (connect(retry.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == 0) {
        std::fputs("TCP: ожидался отказ подключения\n", stderr);
        return 1;
    }
    return errno == ECONNREFUSED ? 0 : fail("повторный connect TCP");
}
