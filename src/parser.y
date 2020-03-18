%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "include/http.h"

int yylex();
int yyerror(char *s);
Request* G_REQ;
%}

%token T_CRLF T_SP
%token T_GET T_HTTP T_SCHEMA T_COLON
%token <str> T_HOSTNAME T_IPV4 T_ABS_PATH T_HTTP_VERSION T_HEADER_HOST T_USER_AGENT T_ACCEPT T_ANY
%token <num> T_NUMBER

%type <request> request
%type <request_url> request_url
%type <request_line> request_line
%type <str> abs_path host
%type <num> port
%type <ht> request_header_list request_header
%type <req_method> method

%union {
    int num;
    char* str;
    RequestLine* request_line;
    RequestUrl* request_url; 
    Request* request; 
    HashTable* ht;
    RequestMethod req_method;
}

%%
request:
      request_line T_CRLF request_header T_CRLF {
        $$ = (Request*)malloc(sizeof(Request)); 
        $$->request_line = $1; 
        $$->header = $3;
        G_REQ = $$;
        YYACCEPT;
      }
    ;

request_line:
      method T_SP request_url T_SP T_HTTP_VERSION {
        $$ = (RequestLine*)malloc(sizeof(RequestLine)); 
        $$->request_url = $3; 
        $$->method = $1;
        $$->version = $5; 
      }
    ;

request_header:
      /* empty */ { $$ = NULL; }
    | request_header_list { $$ = $1; }
    ;

request_header_list:
      T_ANY T_COLON T_ANY T_CRLF { $$ = ht_init((HtValueCtor)strdup, free); ht_insert_str($$, $1, $3); }
    | request_header_list T_ANY T_COLON T_ANY T_CRLF { ht_insert_str($$, $2, $4); }
    ;

method:
      T_GET { $$ = GET; }
    ;

request_url:
      '*' { 
      $$ = (RequestUrl*)malloc(sizeof(RequestUrl)); 
      $$->host = NULL; 
      $$->port = 0;
      $$->abs_path = NULL; 
    }
    | T_SCHEMA host port abs_path { 
      $$ = (RequestUrl*)malloc(sizeof(RequestUrl)); 
      $$->host = $2; 
      $$->port = $3;
      $$->abs_path = $4; 
    }
    | host port abs_path { 
      $$ = (RequestUrl*)malloc(sizeof(RequestUrl)); 
      $$->host = $1; 
      $$->port = $2;
      $$->abs_path = $3; 
    }
    | abs_path { 
      $$ = (RequestUrl*)malloc(sizeof(RequestUrl)); 
      $$->host = NULL; 
      $$->port = 0;
      $$->abs_path = $1; 
    }
    ; 

abs_path:
      /* empty */ {$$ = NULL;}
    | T_ABS_PATH {$$ = $1;}
    ;

port:
      /* empty */ {$$ = 0;}
    | T_COLON T_NUMBER {$$ = $2;}
    ;

host:
      T_HOSTNAME {$$ = $1;}
    | T_IPV4 {$$ = $1;}
    ;
%%

int yyerror(char *s)
{
    extern int yylineno;
    extern char* yytext;
    printf("line %d: %s occur >>%s<<\n", yylineno, s, yytext);
    return 1;
}