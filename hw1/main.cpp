#include "tokens.hpp"
#include "output.hpp"
#include "output.cpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace output;
using namespace std;

// Function declarations for modularity
void handleToken(int token);
void handleString();
string handleEscapeSequence(const string& string_mode, int& index);
void handleInvalidEscapeSequence();
void handleInvalidHexSequence();

int main() {
    enum tokentype token;

    // Read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex())) != 0) {
        // Print token details using the format: <line_number> <token_name> <value>
        switch (token) {
            case VOID:
                printToken(yylineno, token, token_names[VOID].c_str());
                break;

            case INT:
                printToken(yylineno, token, token_names[INT].c_str());
                break;

            case BYTE:
                printToken(yylineno, token, token_names[BYTE].c_str());
                break;

            case BOOL:
                printToken(yylineno, token, token_names[BOOL].c_str());
                break;

            case AND:
                printToken(yylineno, token, token_names[AND].c_str());
                break;

            case OR:
                printToken(yylineno, token, token_names[OR].c_str());
                break;

            case NOT:
                printToken(yylineno, token, token_names[NOT].c_str());
                break;

            case TRUE:
                printToken(yylineno, token, token_names[TRUE].c_str());
                break;

            case FALSE:
                printToken(yylineno, token, token_names[FALSE].c_str());
                break;

            case RETURN:
                printToken(yylineno, token, token_names[RETURN].c_str());
                break;

            case IF:
                printToken(yylineno, token, token_names[IF].c_str());
                break;

            case ELSE:
                printToken(yylineno, token, token_names[ELSE].c_str());
                break;

            case WHILE:
                printToken(yylineno, token, token_names[WHILE].c_str());
                break;

            case BREAK:
                printToken(yylineno, token, token_names[BREAK].c_str());
                break;

            case CONTINUE:
                printToken(yylineno, token, token_names[CONTINUE].c_str());
                break;

            case SC:
                printToken(yylineno, token, token_names[SC].c_str());
                break;

            case COMMA:
                printToken(yylineno, token, token_names[COMMA].c_str());
                break;

            case LPAREN:
                printToken(yylineno, token, token_names[LPAREN].c_str());
                break;

            case RPAREN:
                printToken(yylineno, token, token_names[RPAREN].c_str());
                break;

            case LBRACE:
                printToken(yylineno, token, token_names[LBRACE].c_str());
                break;

            case RBRACE:
                printToken(yylineno, token, token_names[RBRACE].c_str());
                break;

            case ASSIGN:
                printToken(yylineno, token, token_names[ASSIGN].c_str());
                break;

            case RELOP:
                printToken(yylineno, token, token_names[RELOP].c_str());
                break;

            case BINOP:
                printToken(yylineno, token, token_names[BINOP].c_str());
                break;

            case COMMENT:
                printToken(yylineno, token, token_names[COMMENT].c_str());
                break;

            case ID:
                printToken(yylineno, token, token_names[ID].c_str());
                break;

            case NUM:
                printToken(yylineno, token, token_names[NUM].c_str());
                break;

            case NUM_B:
                printToken(yylineno, token, token_names[NUM_B].c_str());
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
    string string_mode(yytext); // Convert the string token to a C++ string
    string processed_string;
    int i = 0;
    int length = string_mode.size();

    // Iterate through the string to process escape sequences
    while (i < length) {
        if (string_mode[i] == '\\' && i + 1 < length) {
            processed_string += handleEscapeSequence(string_mode, i); // Handle escape sequence
        } else if (string_mode[i] == '"') {
            i++; // Skip quotation marks
        } else {
            processed_string += string_mode[i];
            i++;
        }
    }

    printToken(yylineno, token, processed_string.c_str());
}

// Process valid escape sequences and handle invalid ones
string handleEscapeSequence(const string& string_mode, int& index) {
    char next_char = string_mode[index + 1];

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
            int length = string_mode.size();
            if (index + 3 < length && isxdigit(string_mode[index + 2]) && isxdigit(string_mode[index + 3])) {
                string hex_value = string_mode.substr(index + 2, 2);
                char hex_char = stoi(hex_value, nullptr, 16);
                if (0x20 <= hex_char <= 0x21 ||
                    0x23 <= hex_char <= 0x5B ||
                    0x5D <= hex_char <= 0xFE) {
                    index += 3; // Advance past \xNN
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
