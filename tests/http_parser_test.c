#include "include/http.h"
#include "include/util.h"
#include <assert.h>
#include <unistd.h>

extern Request* G_REQ;
extern int yyparse();
extern int lex_read(char* s, int len);
extern void yyrestart(FILE* input_file);
extern void lex_read_line(FILE* fp, char* s, int len);

void UNUSED assert_eq_str(char* str, char* str2)
{
    if (strcmp(str, str2) != 0) {
        printf("str not eq s1: %s, s2: %s\n", str, str2);
        exit(-1);
    }
}

int main(int argc UNUSED, char const* argv[] UNUSED)
{
    // FILE* f;
    // f = fopen("./tests/parser_test.txt", "r+");
    char s[] = "GET /index.html HTTP/1.1\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
               "Accept-Encoding: gzip, deflate, br\r\n"
               "Accept-Language: zh-CN, zh; q=0.9, en; q=0.8\r\n"
               "Cache-Control: no-cache\r\n"
               "Connection: keep-alive\r\n"
               "Host: 127.0.0.1:8088\r\n"
               "Pragma: no-cache\r\n"
               "Sec-Fetch-Dest: document\r\n"
               "Sec-Fetch-Mode: navigate\r\n"
               "Sec-Fetch-Site: cross-site\r\n"
               "Sec-Fetch-User: ?1\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0(Macintosh; Intel Mac OS X 10_15_3)AppleWebKit/537.36(KHTML, like Gecko)Chrome/80.0.3987.122 Safari/537.36\r\n\r\n";
    lex_read(s, sizeof(s));
    // fclose(f);
    char str[1024] = { 0 };
    sprintf(str, "abs_path:%s method:%s version:%s",
        G_REQ->request_line->request_url->abs_path,
        G_REQ->request_line->method,
        G_REQ->request_line->version);
    assert_eq_str(str, "abs_path:/index.html method:get version:HTTP/1.1");
    assert(G_REQ->header->cnt == 13);
    assert_eq_str(ht_find_str(G_REQ->header, "Sec-Fetch-Mode"), "navigate");

    printf("header:\n");
    FOREACH_HT(G_REQ->header, k, v)
    printf("%s:%s\n", k, (char*)v);
    ENDFOREACH()

    char s2[] = "GET /index.html HTTP/1.1\r\n\r\n";
    lex_read(s2, sizeof(s2));
    // fclose(f);
    char str2[1024] = { 0 };
    sprintf(str2, "host:%s port:%d abs_path:%s method:%s version:%s", G_REQ->request_line->request_url->host,
        G_REQ->request_line->request_url->port,
        G_REQ->request_line->request_url->abs_path,
        G_REQ->request_line->method,
        G_REQ->request_line->version);
    assert_eq_str(str2, "host:(null) port:0 abs_path:/index.html method:get version:HTTP/1.1");

    printf("parser pass\n");
    return 0;
}
