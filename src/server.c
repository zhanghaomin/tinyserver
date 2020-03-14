#include "include/http.h"
#include "include/net.h"
#include "include/util.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

extern Request* G_REQ;
extern int yyparse();
extern int lex_read(char* s, int len);
extern void yyrestart(FILE* input_file);
extern void lex_read_line(FILE* fp, char* s, int len);

static int bad_request(int fd)
{
    return write(fd, "HTTP/1.1 400 Bad Request\r\n\r\n", 28);
}

int main(int argc UNUSED, char const* argv[] UNUSED)
{
    /* code */
    int listenfd, connfd;
    listenfd = tcp_listen(8088);

    if (listenfd < 0) {
        sys_err("listen");
    }

    char* buf = calloc(1, 1024);
    char* origin = buf;
    int nread = 0, totalread = 0;

    while (1) {
        buf = origin;
        connfd = accept(listenfd, NULL, 0);

        if (connfd < 0) {
            sys_err("accept");
        }

        printf("connected\n");

    read:
        while ((nread = read(connfd, buf, 1)) > 0) {
            totalread += nread;
            buf += nread;

            if (totalread >= 4 && buf[-4] == '\r' && buf[-3] == '\n' && buf[-2] == '\r' && buf[-1] == '\n') {
                // 数据完整
                printf("http request: \n%s", origin);

                // parse request
                if (lex_read(origin, totalread) != 0) {
                    bad_request(connfd);
                    goto close;
                }

                // handle request and response
                printf("host:%s port:%d abs_path:%s method:%s version:%s\n", G_REQ->request_line->request_url->host,
                    G_REQ->request_line->request_url->port,
                    G_REQ->request_line->request_url->abs_path,
                    G_REQ->request_line->method,
                    G_REQ->request_line->version);

                if (strcmp(G_REQ->request_line->request_url->abs_path, "/index.html") == 0) {
                    write(connfd, "HTTP/1.1 200 OK\r\nContent-Length:11\r\n\r\nhello world", 50);
                } else {
                    bad_request(connfd);
                }
                break;
            }
        }

        // read被信号中断，重新读
        if (nread == -1) {
            if (errno == EINTR) {
                goto read;
            }

            sys_notice("read");
        } else if (nread == 0) {
            // read eof, client close the conn
            printf("client close conn\n");
        }
    close:
        // 服务端关闭链接
        close(connfd);
        printf("close conn\n");
    }

    free(buf);
    return 0;
}
