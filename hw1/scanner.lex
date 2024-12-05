%{

#include <ostream>   // For handling output streams
#include <iostream>  // Provides input and output stream objects like std::cout
#include "output.hpp" // Includes utility functions for error reporting and token printing

// Declare a function to handle valid tokens and print them
int processToken(tokentype tokenType); 

// Declare a function to handle string tokens specifically
void StringTokenHandler(); 

// Declare a function to process invalid escape sequences in strings
void processInvalidEscapeSequence(); 

%}

%option noyywrap
%option yylineno

/* Define patterns for matching */

TavimLevanim        ([ \t\r\n])

content_of_string   ("[^\\\"\n]*")

INVALID_ESCAPE      ["]((\\x[7][0-9a-eA-E]|\\x[2-6][0-9a-fA-F]|\\[\\\"nrt0]|[^\\\n\r])*)([\\][^\\\"nrt0]|[\\][x]|[\\][x][^"]|[\\][x][^"][^"])

PATTERN_OF_STRING   (["]((\\x[0][9aAdD]|\\x[7][0-9a-eA-E]|\\x[2-6][0-9a-fA-F]|\\[\\\"nrt0]|[^\"\\\n\r])*["]))

UNCLOSED_STRING     ["](\\x[7][0-9a-eA-E]|\\x[2-6][0-9a-fA-F]|\\[\\\"nrt0]|[^\\\"\n\r])*

%%

"void"                          { processToken(tokentype::VOID); }
"int"                           { processToken(tokentype::INT); }
"byte"                          { processToken(tokentype::BYTE); }
"bool"                          { processToken(tokentype::BOOL); }
"and"                           { processToken(tokentype::AND); }
"or"                            { processToken(tokentype::OR); }
"not"                           { processToken(tokentype::NOT); }
"true"                          { processToken(tokentype::TRUE); }
"false"                         { processToken(tokentype::FALSE); }
"return"                        { processToken(tokentype::RETURN); }
"if"                            { processToken(tokentype::IF); }
"else"                          { processToken(tokentype::ELSE); }
"while"                         { processToken(tokentype::WHILE); }
"break"                         { processToken(tokentype::BREAK); }
"continue"                      { processToken(tokentype::CONTINUE); }

";"                             { processToken(tokentype::SC); }
","                             { processToken(tokentype::COMMA); }
"("                             { processToken(tokentype::LPAREN); }
")"                             { processToken(tokentype::RPAREN); }
"{"                             { processToken(tokentype::LBRACE); }
"}"                             { processToken(tokentype::RBRACE); }
"="                             { processToken(tokentype::ASSIGN); }
[=][=]|[!][=]|[<]|[>]|[>][=]|[<][=] { processToken(tokentype::RELOP); }
[+]|[-]|[*]|[\/]                { processToken(tokentype::BINOP); }

\/\/[^\n\r]*                    { processToken(tokentype::COMMENT); }

[a-zA-Z][a-zA-Z0-9]*            { processToken(tokentype::ID); }
[1-9][0-9]*|0                   { processToken(tokentype::NUM); }
([1-9][0-9]*|0)[bB]             { processToken(tokentype::NUM_B); }

{PATTERN_OF_STRING}             { StringTokenHandler(); } 

{TavimLevanim}                  {  }

{INVALID_ESCAPE}                { processInvalidEscapeSequence(); } 

{UNCLOSED_STRING}               { output::errorUnclosedString(); } 

.                               { output::errorUnknownChar(*yytext); }

%%

/* Handle valid string tokens */
void StringTokenHandler() {
    const char* content = yytext + 1;  // Skip the opening quotation mark

    std::cout << yylineno << " STRING ";  // Print the line number and token type

    for (const char* charPtr = content; *(charPtr + 1) != '\0'; charPtr++) {  // Loop through each character
        if (*charPtr == '\\') {  // Handle escape sequences
            switch (*(charPtr + 1)) {  // Look at the character following the backslash
                case 't':
                    std::cout << "\t";  // Print a tab
                    break;
                case 'n':
                    std::cout << std::endl;  // Print a newline
                    break;
                case 'r':
                    std::cout << "\r";  // Print a carriage return
                    break;
                case '\\':
                    std::cout << "\\";  // Print a backslash
                    break;
                case '"':
                    std::cout << '"';  // Print a double quote
                    break;
                case '0':  // Handle null terminator
                    return;  // Stop processing further characters
                case 'x': {  // Handle hexadecimal escape sequences
                    int firstDigit, secondDigit;

                    // Convert the first hex digit
                    if (*(charPtr + 2) >= '0' && *(charPtr + 2) <= '9') {
                        firstDigit = *(charPtr + 2) - '0';
                    } else if (*(charPtr + 2) >= 'a' && *(charPtr + 2) <= 'f') {
                        firstDigit = *(charPtr + 2) - 'a' + 10;
                    } else if (*(charPtr + 2) >= 'A' && *(charPtr + 2) <= 'F') {
                        firstDigit = *(charPtr + 2) - 'A' + 10;
                    }

                    // Convert the second hex digit
                    if (*(charPtr + 3) >= '0' && *(charPtr + 3) <= '9') {
                        secondDigit = *(charPtr + 3) - '0';
                    } else if (*(charPtr + 3) >= 'a' && *(charPtr + 3) <= 'f') {
                        secondDigit = *(charPtr + 3) - 'a' + 10;
                    } else if (*(charPtr + 3) >= 'A' && *(charPtr + 3) <= 'F') {
                        secondDigit = *(charPtr + 3) - 'A' + 10;
                    }

                    char hexValue = static_cast<char>((firstDigit << 4) | secondDigit);  // Combine the digits into a single character
                    std::cout << hexValue;  // Print the hexadecimal value
                    charPtr += 2;  // Skip the two hex digits
                    break;
                }
                default:
                    break;  // Ignore invalid cases
            }
            charPtr++;  // Skip the escape character
        } else {
            std::cout << *charPtr;  // Print the character as-is
        }
    }
    std::cout << std::endl;  // End the line after processing the string
}

/* Handle invalid escape sequences */
void processInvalidEscapeSequence() {
    const char* iterator = yytext;  // Start at the beginning of the string

    while (*iterator != '\0') {  // Find the end of the string
        iterator++;
    }

    // Traverse backwards to find the invalid escape sequence
    for (const char* charPtr = iterator; ; charPtr--) {
        if (*charPtr == '\\') {  // Check for the backslash
            output::errorUndefinedEscape(charPtr + 1);  // Report the invalid escape
        }
    }
}

/* Handle valid tokens and print them */
int processToken(tokentype tokenType) {
    output::printToken(yylineno, tokenType, yytext);  // Print the token type and value
    return 0;  // Indicate success
}
