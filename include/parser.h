/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INCLUDE_PARSER_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_CRLF = 258,
    T_SP = 259,
    T_GET = 260,
    T_HTTP = 261,
    T_SCHEMA = 262,
    T_COLON = 263,
    T_HOSTNAME = 264,
    T_IPV4 = 265,
    T_ABS_PATH = 266,
    T_HTTP_VERSION = 267,
    T_HEADER_HOST = 268,
    T_USER_AGENT = 269,
    T_ACCEPT = 270,
    T_ANY = 271,
    T_NUMBER = 272
  };
#endif
/* Tokens.  */
#define T_CRLF 258
#define T_SP 259
#define T_GET 260
#define T_HTTP 261
#define T_SCHEMA 262
#define T_COLON 263
#define T_HOSTNAME 264
#define T_IPV4 265
#define T_ABS_PATH 266
#define T_HTTP_VERSION 267
#define T_HEADER_HOST 268
#define T_USER_AGENT 269
#define T_ACCEPT 270
#define T_ANY 271
#define T_NUMBER 272

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 23 "src/parser.y" /* yacc.c:1909  */

    int num;
    char* str;
    RequestLine* request_line;
    RequestUrl* request_url; 
    Request* request; 

#line 96 "include/parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_H_INCLUDED  */
