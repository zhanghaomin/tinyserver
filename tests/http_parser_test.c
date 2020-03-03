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
    char s[] = "GET http://baidu.com:8080/index.html HTTP/1.1\r\n\r\n";
    lex_read(s, sizeof(s));
    // fclose(f);
    char str[1024] = { 0 };
    sprintf(str, "host:%s port:%d abs_path:%s method:%s version:%s", G_REQ->request_line->request_url->host,
        G_REQ->request_line->request_url->port,
        G_REQ->request_line->request_url->abs_path,
        G_REQ->request_line->method,
        G_REQ->request_line->version);
    assert_eq_str(str, "host:baidu.com port:8080 abs_path:/index.html method:get version:HTTP/1.1");

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
