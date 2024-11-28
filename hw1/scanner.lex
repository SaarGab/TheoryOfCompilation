%{

#include "tokens.hpp"
#include "output.hpp"

void report_string_error(const char* msg);

%}

%option noyywrap
%option yylineno

DIGITS                 [0-9]                         
LETTERS                [a-zA-Z]                      
ALNUMS                 ([a-zA-Z0-9])                 
WHITESPACES            [ \t\r\n]                     
PRINTABLE              [\x20-\x21\x23-\x5B\x5D-\x7E] 
ESCAPE_SEQUENCES       (\\[ntr\"0])                  
HEX_ESCAPES            (\\x[0-9A-Fa-f]{2})           

%x STRING_MODE 

%%


"void"                 { return VOID; }
"int"                  { return INT; }
"byte"                 { return BYTE; }
"bool"                 { return BOOL; }
"and"                  { return AND; }
"or"                   { return OR; }
"not"                  { return NOT; }
"true"                 { return TRUE; }
"false"                { return FALSE; }
"return"               { return RETURN; }
"if"                   { return IF; }
"else"                 { return ELSE; }
"while"                { return WHILE; }
"break"                { return BREAK; }
"continue"             { return CONTINUE; }


";"                    { return SC; }
","                    { return COMMA; }
"("                    { return LPAREN; }
")"                    { return RPAREN; }
"{"                    { return LBRACE; }
"}"                    { return RBRACE; }
"="                    { return ASSIGN; }
"=="|"<="|">="|"!="    { return RELOP; }
"<"|">"                { return RELOP; }
"+"|"-"|"*"|"/"        { return BINOP; }


\/\/[^\n\r]*           { return COMMENT; }


{LETTERS}({ALNUMS})*   { return ID; }
"0"                    { return NUM; }
[1-9]+[0-9]*           { return NUM; }


"0[bB]"                { return NUM_B; }
[1-9]+[0-9]*[bB]       { return NUM_B; }


\"                     { BEGIN(STRING_MODE); } 

<STRING_MODE>{
    {ESCAPE_SEQUENCES} {  }
    {HEX_ESCAPES}      {  }
    {PRINTABLE}        {  }
    \\[^ntr\"0x]       { report_string_error("Invalid escape sequence"); }
    \\x[0-9A-Fa-f]?    { report_string_error("Malformed hex escape"); }
    \n                 { report_string_error("Unclosed string"); }
    \"                 { BEGIN(INITIAL); return STRING; } /* End of string */
}


{WHITESPACES}+         {  }
\n                     { yylineno++; }


.                      { output::errorUnknownChar(yytext[0]); }

%%


void report_string_error(const char* msg) {
    if (strcmp(msg, "Unclosed string") == 0) {
        output::errorUnclosedString();
    } else if (strstr(msg, "Invalid escape sequence") != NULL) {
        output::errorUndefinedEscape(yytext);
    }
}
