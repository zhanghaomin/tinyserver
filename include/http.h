#ifndef _HTTP_H
#define _HTTP_H

#include "ht.h"

typedef enum {
    GET,
    POST
} RequestMethod;

typedef struct RequestUrl {
    char* host;
    int port;
    char* abs_path;
} RequestUrl;

typedef struct RequestLine {
    RequestUrl* request_url;
    RequestMethod method;
    char* version;
} RequestLine;

typedef struct Request {
    RequestLine* request_line;
    HashTable* header;
} Request;

typedef enum {
    OK,
    BAD_REQUEST,
    NOT_FOUND
} ResponseCode;

typedef struct Response {
    ResponseCode code;
    char* msg;
    char* body;
    int content_length;
    HashTable* header;
} Response;

Response* new_response();
void with_status(Response* resp, ResponseCode code);
void with_body(Response* resp, char* body);
void destory_response(Response* resp);
char* serialize_response(Response* resp, int* len);
#endif