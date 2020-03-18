#include "include/http.h"
#include "include/net.h"
#include "include/util.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define WWWROOT "www"

extern Request* G_REQ;
extern int yyparse();
extern int lex_read(char* s, int len);
extern void yyrestart(FILE* input_file);
extern void lex_read_line(FILE* fp, char* s, int len);

static int bad_request(int fd)
{
    Response* resp;
    int len;
    char* out;
    resp = new_response();
    with_status(resp, BAD_REQUEST);
    out = serialize_response(resp, &len);
    write(fd, out, len);
    free(out);
    destory_response(resp);
    return 1;
}

static int not_found(int fd)
{
    Response* resp;
    int len;
    char* out;
    resp = new_response();
    with_status(resp, NOT_FOUND);
    out = serialize_response(resp, &len);
    write(fd, out, len);
    free(out);
    destory_response(resp);
    return 1;
}

static int ok_response(int fd, char* body)
{
    Response* resp;
    int len;
    char* out;
    resp = new_response();
    with_status(resp, OK);
    with_body(resp, body);
    out = serialize_response(resp, &len);
    write(fd, out, len);
    free(out);
    destory_response(resp);
    return 1;
}

static void destory_request(Request* req)
{
    ht_release(req->header);
    free(req->request_line->request_url->abs_path);
    free(req->request_line->request_url->host);
    free(req->request_line->request_url);
    free(req->request_line->version);
    free(req->request_line);
    free(req);
}

int main(int argc UNUSED, char const* argv[] UNUSED)
{
    int listenfd, connfd, read_file_fd, nread = 0, totalread = 0;
    char *buf, *origin;
    char filename[1024];
    char file_content[1024];

    listenfd = tcp_listen(8088);

    if (listenfd < 0) {
        sys_err("listen");
    }

    buf = calloc(1, 1024);
    origin = buf;

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

                if (G_REQ->request_line->method != GET) {
                    bad_request(connfd);
                    goto close;
                }

                sprintf(filename, WWWROOT "%s", G_REQ->request_line->request_url->abs_path);
                read_file_fd = open(filename, O_RDONLY);

                if (read_file_fd < 0) {
                    if (errno == ENOENT) {
                        not_found(connfd);
                    } else if (errno == EACCES) {
                        // 403
                    }
                } else {
                    ssize_t r = read(read_file_fd, file_content, 1024);
                    file_content[r] = '\0';
                    ok_response(connfd, file_content);
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
        if (G_REQ != NULL) {
            destory_request(G_REQ);
            G_REQ = NULL;
        }

        // 服务端关闭链接
        close(connfd);
        printf("close conn\n");
    }

    free(buf);
    return 0;
}
