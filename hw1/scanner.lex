%{
#include "tokens.hpp"
#include "output.hpp"

// חתימות לפונקציות עזר

%}

%option yylineno
%option noyywrap
digits ([0-9])
letters ([a-zA-Z])
whitespaces ([ \t\r\n])
printable ([\x20-\x21\x23-\x5B\x5D-\x7E])
%x STR // הגדרת מצב חדש

%%

void
int
byte
bool
and
or
not
true
false
return
if
else
while
break
continue
;
,
(
)
{
}
=
==|!=|<|>|<=|>=
+|-|*|/
\/\/[^\n\r]+
[letters]+[a-zA-Z0-9]*
0
[1-9]+[0-9]*
0[bB]
[1-9]+[0-9]*[bB]
\"printable*\"
