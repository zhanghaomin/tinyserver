#ifndef _HTTP_H
#define _HTTP_H

#include "ht.h"

typedef struct RequestUrl {
    char* host;
    int port;
    char* abs_path;
} RequestUrl;

typedef struct RequestLine {
    RequestUrl* request_url;
    char* method;
    char* version;
} RequestLine;

typedef struct Request {
    RequestLine* request_line;
    char* header;
} Request;

Request* G_REQ;
#endif