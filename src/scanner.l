%option noyywrap nodefault noyylineno case-insensitive nounput noinput

%{
#include "include/http.h"
#include "include/parser.h"

int yyerror(char *s);
%}

num [0-9]
alpha [a-zA-Z]
alphanum [a-zA-Z0-9]
domainlabel {alphanum}*|({alphanum}({alphanum}|-)*{alphanum})
toplabel {alpha}*|{alpha}({alphanum}|-)*{alphanum}
hostname ({domainlabel}\.)*{toplabel}
ipv4 {num}+\.{num}+\.{num}+\.{num}+
path_segements {segement}(\/{segement})*
segement ({alphanum}|\.)+

%s start_header_val
%%

"\r\n" {return T_CRLF;}

" " {return T_SP;}

":" {return T_COLON;}

"GET" {return T_GET;}

"http://" {return T_SCHEMA;}

"HTTP/"{num}\.{num} {
    yylval.str = strdup(yytext);
    return T_HTTP_VERSION;
}

"HTTP" {return T_HTTP;}

"Host" {
    yylval.str = strdup(yytext);
    return T_HEADER_HOST;
}

"User-Agent" {
    yylval.str = strdup(yytext);
    BEGIN(start_header_val);
    return T_USER_AGENT;
}

"Accept" {
    yylval.str = strdup(yytext);
    BEGIN(start_header_val);
    return T_ACCEPT;
}

<start_header_val>[^:\r\n ]+ {
    yylval.str = strdup(yytext);
    printf("match any word:%s\n", yytext);
    BEGIN(0);
    return T_ANY;
}

{num}+ {
    yylval.num = atoi(yytext);
    return T_NUMBER;
}

{hostname} {
    yylval.str = strdup(yytext);
    return T_HOSTNAME;
}

{ipv4} {
    yylval.str = strdup(yytext);
    return T_IPV4;
}

"/"{path_segements} {
    yylval.str = strdup(yytext);
    return T_ABS_PATH;
}

. { return *yytext; }
%%

int lex_read_line(FILE* fp, char *s, int len)
{
    YY_BUFFER_STATE bp;
    int res;
    fgets(s, len, fp); //读取一行
    bp = yy_scan_bytes(s, len);
    yy_switch_to_buffer(bp);
    res = yyparse();
    yy_delete_buffer(bp);
    return res;
}

int lex_read(char *s, int len)
{
    YY_BUFFER_STATE bp;
    int res;
    bp = yy_scan_bytes(s, len);
    yy_switch_to_buffer(bp);
    res = yyparse();
    yy_delete_buffer(bp);
    return res;
}