#include "include/http.h"

Response* new_response()
{
    Response* resp;
    resp = calloc(1, sizeof(Response));
    resp->header = ht_init((HtValueCtor)strdup, free);
    return resp;
}

void with_status(Response* resp, ResponseCode code)
{
    if (code == OK) {
        resp->code = 200;
        resp->msg = strdup("OK");
    } else if (code == BAD_REQUEST) {
        resp->code = 400;
        resp->msg = strdup("Bad Request");
    } else if (code == NOT_FOUND) {
        resp->code = 404;
        resp->msg = strdup("Not Found");
        resp->body = strdup("file not exist.");
        resp->content_length = strlen(resp->body);
    }
}

void with_body(Response* resp, char* body)
{
    resp->body = strdup(body);
    resp->content_length = strlen(body);
}

void destory_response(Response* resp)
{
    ht_release(resp->header);

    if (resp->body != NULL) {
        free(resp->body);
    }

    if (resp->msg != NULL) {
        free(resp->msg);
    }

    free(resp);
}

char* serialize_response(Response* resp, int* len)
{
    char* str;
    str = calloc(1, 1024);
    *len = sprintf(str, "HTTP/1.1 %d %s\r\n", resp->code, resp->msg);

    if (resp->body != NULL) {
        *len += sprintf(str + *len, "Content-Length: %d\r\n\r\n"
                                    "%s",
            resp->content_length, resp->body);
    }

    return str;
}