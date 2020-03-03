#include "include/net.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

int tcp_listen(int port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenfd < 0) {
        return -1;
    }

    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(port);
    svraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int open = 1;

    // 开启reuse
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &open, sizeof(open)) == -1) {
        return -1;
    }

    // 关闭nagle
    if (setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, &open, sizeof(open)) == -1) {
        return -1;
    }

    if (bind(listenfd, (struct sockaddr*)&svraddr, sizeof(svraddr)) < 0) {
        return -1;
    }

    if (listen(listenfd, 65535) < 0) {
        return -1;
    }

    return listenfd;
}