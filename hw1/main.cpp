#include "tokens.hpp"
#include "output.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace output;
using namespace std;

// Function declarations
void handleString(tokentype token);
string handleEscapeSequence(string* string_mode, int* index);
void handleInvalidEscapeSequence();
void handleInvalidHexSequence();

// Global variable for string tokens from scanner.lex
extern std::string yylval_string;

int main() {
    enum tokentype token;

    // Read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex())) != 0) {
        switch (token) {
            case VOID: case INT: case BYTE: case BOOL: case AND:
            case OR: case NOT: case TRUE: case FALSE: case RETURN:
            case IF: case ELSE: case WHILE: case BREAK: case CONTINUE:
            case SC: case COMMA: case LPAREN: case RPAREN:
            case LBRACE: case RBRACE: case ASSIGN: case RELOP:
            case BINOP: case COMMENT: case ID: case NUM: case NUM_B:
                printToken(yylineno, token, yytext);
                break;

            case STRING:
                handleString(token);
                break;

            default:
                cout << "Error: Unrecognized token " << yytext << endl;
                exit(1);
        }
    }

    return 0;
}

// Handle string tokens
void handleString(tokentype token) {
    std::string raw_string = yylval_string; // Get the raw string from scanner.lex
    std::string processed_string;

    int i = 0;
    while (i < raw_string.size()) {
        if (raw_string[i] == '\\') {
            // Handle escape sequences
            processed_string += handleEscapeSequence(&raw_string, &i);
            i++;
        } else {
            // Append regular characters
            processed_string += raw_string[i];
        }
        i++;
    }

    printToken(yylineno, token, processed_string.c_str());
}

// Handle escape sequences within strings
string handleEscapeSequence(string* string_mode, int* index) {
    char next_char = (*string_mode)[*index + 1];

    switch (next_char) {
        case 'n': return "\n";
        case 'r': return "\r";
        case 't': return "\t";
        case '\\': return "\\";
        case '\"': return "\"";

        case 'x': {
            int length = string_mode->size();
            if (*index + 3 < length && isxdigit((*string_mode)[*index + 2]) && isxdigit((*string_mode)[*index + 3])) {
                string hex_value = string_mode->substr(*index + 2, 2);
                char hex_char = stoi(hex_value, nullptr, 16);
                if (hex_char >= 32 && hex_char <= 126) {
                    *index += 3;
                    return string(1, hex_char);
                }
            }
            handleInvalidHexSequence();
            break;
        }

        default:
            handleInvalidEscapeSequence();
            break;
    }
    return "";
}

// Handle invalid escape sequences
void handleInvalidEscapeSequence() {
    output::errorUndefinedEscape(yytext);
    exit(1);
}

// Handle invalid hexadecimal escape sequences
void handleInvalidHexSequence() {
    cout << "Error: Invalid hexadecimal escape sequence " << yytext << endl;
    exit(1);
}
