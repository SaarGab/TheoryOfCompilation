#include "tokens.hpp"
#include "output.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace output;
using namespace std;

static const std::string token_names[] = {
        "__FILLER_FOR_ZERO",
        "VOID",
        "INT",
        "BYTE",
        "BOOL",
        "AND",
        "OR",
        "NOT",
        "TRUE",
        "FALSE",
        "RETURN",
        "IF",
        "ELSE",
        "WHILE",
        "BREAK",
        "CONTINUE",
        "SC",
        "COMMA",
        "LPAREN",
        "RPAREN",
        "LBRACE",
        "RBRACE",
        "ASSIGN",
        "RELOP",
        "BINOP",
        "COMMENT",
        "ID",
        "NUM",
        "NUM_B",
        "STRING"
};

// Function declarations for modularity
void handleString(tokentype token);
string handleEscapeSequence(string* string_mode, int* index);
void handleInvalidEscapeSequence();
void handleInvalidHexSequence();

int main() {
    enum tokentype token;

    // Read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex())) != 0) {
        // Print token details using the format: <line_number> <token_name> <value>
        switch (token) {
            case VOID:
                printToken(yylineno, token, yytext);
                break;

            case INT:
                printToken(yylineno, token, yytext);
                break;

            case BYTE:
                printToken(yylineno, token, yytext);
                break;

            case BOOL:
                printToken(yylineno, token, yytext);
                break;

            case AND:
                printToken(yylineno, token, yytext);
                break;

            case OR:
                printToken(yylineno, token, yytext);
                break;

            case NOT:
                printToken(yylineno, token, yytext);
                break;

            case TRUE:
                printToken(yylineno, token, yytext);
                break;

            case FALSE:
                printToken(yylineno, token, yytext);
                break;

            case RETURN:
                printToken(yylineno, token, yytext);
                break;

            case IF:
                printToken(yylineno, token, yytext);
                break;

            case ELSE:
                printToken(yylineno, token, yytext);
                break;

            case WHILE:
                printToken(yylineno, token, yytext);
                break;

            case BREAK:
                printToken(yylineno, token, yytext);
                break;

            case CONTINUE:
                printToken(yylineno, token, yytext);
                break;

            case SC:
                printToken(yylineno, token, yytext);
                break;

            case COMMA:
                printToken(yylineno, token, yytext);
                break;

            case LPAREN:
                printToken(yylineno, token, yytext);
                break;

            case RPAREN:
                printToken(yylineno, token, yytext);
                break;

            case LBRACE:
                printToken(yylineno, token, yytext);
                break;

            case RBRACE:
                printToken(yylineno, token, yytext);
                break;

            case ASSIGN:
                printToken(yylineno, token, yytext);
                break;

            case RELOP:
                printToken(yylineno, token, yytext);
                break;

            case BINOP:
                printToken(yylineno, token, yytext);
                break;

            case COMMENT:
                printToken(yylineno, token, yytext);
                break;

            case ID:
                printToken(yylineno, token, yytext);
                break;

            case NUM:
                printToken(yylineno, token, yytext);
                break;

            case NUM_B:
                printToken(yylineno, token, yytext);
                break;

            case STRING:
                handleString(token); // Process string tokens separately
                break;

            default:
                cout << "Error: Unrecognized token " << yytext << endl;
                exit(1);
        }
    }

    return 0;
}

// Handle string tokens and process escape sequences
void handleString(tokentype token) {
    string string_mode = string(yytext); // Convert the string token to a C++ string
    // string string_mode(yytext); // Convert the string token to a C++ string
    string processed_string = "";
    int i = 0;
    int length = string_mode.size();
    while (*yytext != '\0') {
        cout << "@@@ yytext[i] = " << *yytext << " @@@" << endl;
        //cout << "@@@ yytext[i+1] = " << yytext[i+1] << " @@@" << endl;
        //cout << "@@@ yytext[i+2] = " << yytext[i+2] << " @@@" << endl;
        yytext++;
    }

    // Iterate through the string to process escape sequences
    while (yytext[i] != '\0') {
    // while (i < length) {
        cout << "@@@ The string is: " << string_mode << " @@@" << endl; 
        cout << "@@@ " << i << " < " << length << " @@@" << endl;
        if (string_mode[i] == '\\' && i + 1 < length) {
            processed_string += handleEscapeSequence(&string_mode, &i); // Handle escape sequence
        } else if (string_mode[i] == '"') {
            cout << "@@@ manual debug @@@" << endl;
            i++; // Skip quotation marks
        } else {
            processed_string += string_mode[i];
            i++;
        }
    }

    printToken(yylineno, token, yytext);
}

// Process valid escape sequences and handle invalid ones
string handleEscapeSequence(string* string_mode, int* index) {
    char next_char = (*string_mode)[*index + 1];

    switch (next_char) {
        case 'n':
            return "\n";
            break;

        case 'r':
            return "\r";
            break;

        case 't':
            return "\t";
            break;

        case '\\':
            return "\\";
            break;
            
        case '\"':
            return "\"";
            break;
            
        case 'x': {
            // Handle hexadecimal escape sequence (\xNN)
            int length = (*string_mode).size();
            if (*index + 3 < length && isxdigit((*string_mode)[*index + 2]) && isxdigit((*string_mode)[*index + 3])) {
                string hex_value = (*string_mode).substr(*index + 2, 2);
                char hex_char = stoi(hex_value, nullptr, 16);
                if (0x20 <= hex_char <= 0x21 ||
                    0x23 <= hex_char <= 0x5B ||
                    0x5D <= hex_char <= 0xFE) {
                    *index += 3; // Advance past \xNN
                    return string(1, hex_char);
                }
            }
            handleInvalidHexSequence(); // Invalid or incomplete hex escape
            break;
        }
        
        default:
            handleInvalidEscapeSequence(); // Undefined escape sequence
            break;
    }
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
