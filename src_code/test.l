%{
				
#include "y.tab.h"
extern int lineno;
%}

digit       [0-9]
integer		{digit}+
float		{digit}*\.{digit}+
letter      [a-zA-Z]
identifier  {letter}+
whitespace  [ \t]
dothfile    {letter}+\.h
mychar		'{letter}'
%option noyywrap

%%
"#include"  {return INCL;}
"if"        {return IF;}
"int"       {return INT;}
"float"     {return FLT;}
"char"      {return CHR;}
"void"      {return VOID;}
"<"         {return LT;}
">"         {return GT;}
"<="         {return LE;}
">="         {return GE;}
"=="         {return EQ;}
"!="         {return NE;}
";"          {return SEMI;}
","          {return COMA;}
"{"          {return LBRACE;}
"}"          {return RBRACE;}
"("          {return LBRACKET;}
")"          {return RBRACKET;}
"+"          {return PLUS;}
"-"          {return MINUS;}
"*"          {return MUL;}
"/"          {return DIV;}
"="          {return ASSIGN;}
{mychar}	 {return CHARCTR;}
{dothfile}  {return DOTHFILE;}
{integer}    {yylval.d=atof(yytext);return INUM;}
{float}		{yylval.d=atof(yytext); return FNUM;}
{identifier}    {strcpy(yylval.ch, yytext);return ID;}
{whitespace}    {/* skip */}
\n              {++lineno;}
.               {return ERROR;}
%%