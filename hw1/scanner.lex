%{
/*
 * אינקלודים של קבצים נדרשים להגדרת אסימונים ולפונקציות פלט.
 * tokens.hpp מספק הגדרות enum עבור האסימונים.
 * output.hpp מספק פונקציות עזר לפלט וטיפול בשגיאות.
 */
#include "tokens.hpp"
#include "output.hpp"

/* חתימות לפונקציות עזר */
void report_string_error(const char* msg);

%}

/* הגדרות Flex */
%option noyywrap
%option yylineno

/* הגדרות מאקרו לשימוש חוזר בתבניות */
DIGITS                 [0-9]                         /* ספרה בודדת */
LETTERS                [a-zA-Z]                      /* אותיות */
ALNUMS                 ([a-zA-Z0-9])                 /* תווים אלפאנומריים */
WHITESPACES            [ \t\r\n]                     /* רווחים, טאב, או ירידות שורה */
PRINTABLE              [\x20-\x21\x23-\x5B\x5D-\x7E] /* תווים ניתנים להדפסה (ללא תווים מיוחדים) */
ESCAPE_SEQUENCES       (\\[ntr\"0])                  /* רצפי בריחה חוקיים */
HEX_ESCAPES            (\\x[0-9A-Fa-f]{2})           /* רצף הקסה-דצימלי */

%x STRING_MODE /* הגדרת מצב חדש לטיפול במחרוזות */

%%

/* === מילות מפתח === */
/*
 * מילות שמורות מזוהות בדיוק ומחזירות את סוג האסימון המתאים.
 */
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

/* === סמלים ואופרטורים === */
/*
 * זיהוי סמלים בודדים ואופרטורים מרובי תווים.
 */
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

/* === הערות === */
/*
 * הערות שורה מתחילות ב- "//" ונמשכות עד סוף השורה.
 */
\/\/[^\n\r]*           { return COMMENT; }

/* === מזהים ומספרים === */
/*
 * מזהים: מתחילים באות או בקו תחתון ויכולים לכלול תווים אלפאנומריים.
 * מספרים: יכולים להיות "0" יחיד או רצף ספרות ללא אפסים מובילים.
 */
{LETTERS}({ALNUMS})*   { return ID; }
"0"                    { return NUM; }
[1-9]+[0-9]*           { return NUM; }

/* מספרים בינאריים מזוהים על ידי סיומת 'b' או 'B'. */
"0[bB]"                { return NUM_B; }
[1-9]+[0-9]*[bB]       { return NUM_B; }

/* === מחרוזות === */
/*
 * מחרוזות מוקפות במרכאות כפולות. הן יכולות לכלול תווים ניתנים להדפסה,
 * רצפי בריחה חוקיים, או רצפי הקסה-דצימליים.
 */
\"                     { BEGIN(STRING_MODE); } /* מעבר למצב מחרוזת */

<STRING_MODE>{
    {PRINTABLE}        { /* צבירת תווים חוקיים */ }
    {ESCAPE_SEQUENCES} { /* טיפול ברצפי בריחה חוקיים */ }
    {HEX_ESCAPES}      { /* טיפול ברצפי הקסה-דצימליים */ }
    \\[^ntr\"0x]       { report_string_error("Invalid escape sequence"); }
    \\x[0-9A-Fa-f]?    { report_string_error("Malformed hex escape"); }
    \n                 { report_string_error("Unclosed string"); }
    \"                 { BEGIN(INITIAL); return STRING; } /* סיום מחרוזת */
}

/* === רווחים לבנים === */
/*
 * התעלמות מרווחים לבנים מחוץ למחרוזות.
 */
{WHITESPACES}+         { /* התעלמות מרווחים */ }
\n                     { yylineno++; }

/* === תווים לא מוכרים === */
/*
 * כל תו שאינו תואם אסימון חוקי מזוהה כשגיאה.
 */
.                      { output::errorUnknownChar(yytext[0]); }

%%

/* === פונקציות עזר === */

/*
 * מדווחת על שגיאה הקשורה למחרוזות.
 * השגיאה נרשמת באמצעות פונקציות פלט ומפסיקה את העיבוד.
 */
void report_string_error(const char* msg) {
    if (strcmp(msg, "Unclosed string") == 0) {
        output::errorUnclosedString();
    } else if (strstr(msg, "Invalid escape sequence") != NULL) {
        output::errorUndefinedEscape(yytext);
    }
}


void escape_sequence_replacer(const char* msg) {
    switch (msg) {
        
            break;
    }
}