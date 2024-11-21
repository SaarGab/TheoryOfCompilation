#include "tokens.hpp"
#include "output.hpp"
#include "output.cpp"

using namespace output;
using namespace std;

int main() {
    enum tokentype token;

    // read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex()))) {
        switch (token)
        {
            case VOID:
                printToken(yylineno, token, (token_names[VOID]).c_str());
                break;

            case INT:
                printToken(yylineno, token, (token_names[INT]).c_str());
                break;

            case BYTE:
                printToken(yylineno, token, (token_names[BYTE]).c_str());
                break;

            case BOOL:
                printToken(yylineno, token, (token_names[BOOL]).c_str());
                break;

            case AND:
                printToken(yylineno, token, (token_names[AND]).c_str());
                break;

            case OR:
                printToken(yylineno, token, (token_names[OR]).c_str());
                break;

            case NOT:
                printToken(yylineno, token, (token_names[NOT]).c_str());
                break;

            case TRUE:
                printToken(yylineno, token, (token_names[TRUE]).c_str());
                break;

            case FALSE:
                printToken(yylineno, token, (token_names[FALSE]).c_str());
                break;

            case RETURN:
                printToken(yylineno, token, (token_names[RETURN]).c_str());
                break;

            case IF:
                printToken(yylineno, token, (token_names[IF]).c_str());
                break;

            case ELSE:
                printToken(yylineno, token, (token_names[ELSE]).c_str());
                break;

            case WHILE:
                printToken(yylineno, token, (token_names[WHILE]).c_str());
                break;

            case BREAK:
                printToken(yylineno, token, (token_names[BREAK]).c_str());
                break;

            case CONTINUE:
                printToken(yylineno, token, (token_names[CONTINUE]).c_str());
                break;

            case SC:
                printToken(yylineno, token, (token_names[SC]).c_str());
                break;

            case COMMA:
                printToken(yylineno, token, (token_names[COMMA]).c_str());
                break;

            case LPAREN:
                printToken(yylineno, token, (token_names[LPAREN]).c_str());
                break;

            case RPAREN:
                printToken(yylineno, token, (token_names[RPAREN]).c_str());
                break;

            case LBRACE:
                printToken(yylineno, token, (token_names[LBRACE]).c_str());
                break;

            case RBRACE:
                printToken(yylineno, token, (token_names[RBRACE]).c_str());
                break;

            case ASSIGN:
                printToken(yylineno, token, (token_names[ASSIGN]).c_str());
                break;

            case RELOP:
                printToken(yylineno, token, (token_names[RELOP]).c_str());
                break;

            case BINOP:
                printToken(yylineno, token, (token_names[BINOP]).c_str());
                break;

            case COMMENT:
                printToken(yylineno, token, (token_names[COMMENT]).c_str());
                break;

            case ID:
                printToken(yylineno, token, (token_names[ID]).c_str());
                break;

            case NUM:
                printToken(yylineno, token, (token_names[NUM]).c_str());
                break;

            case NUM_B:
                printToken(yylineno, token, (token_names[NUM_B]).c_str());
                break;

            case STRING:
                string string_mode(yytext);
                int length = string_mode.size();
                for (int i = 0; i < length-1; i++)
                {
                    const char current_char = string_mode[i];
                    switch (current_char)
                    {
                    case '\n':
                        cout << '\n';
                        break;
                    
                    case '\r':
                        cout << '\r';
                        break;
                    
                    case '\t':
                        cout << '\t';
                        break;
                    
                    case '\\':
                        cout << '\\';
                        break;
                    
                    case '\"':
                        cout << '\"';
                        break;
                    
                    default:
                        for (int i = 0x20 ; i <= 0x21 ; i++) {
                            if (current_char == (const char)i) {
                                cout << current_char;
                                break;
                            }
                        }
                        for (int i = 0x23 ; i <= 0x5B ; i++) {
                            if (current_char == (const char)i) {
                                cout << current_char;
                                break;
                            }
                        }
                        for (int i = 0x5D ; i <= 0xFE ; i++) {
                            if (current_char == (const char)i) {
                                cout << current_char;
                                break;
                            }
                        }
                        char backslash_array[] = {'\n', '\t', '\r', '\\', '\"', '\0'};
                        int array_length = (int)(sizeof(backslash_array) / sizeof(backslash_array[0]));
                        for (int i=0 ; i<array_length ; i++) {
                            if (current_char == backslash_array[i]) {
                                output::errorUnclosedString();
                            }
                        }
                        break;
                    }
                }
                cout << string_mode[length-1];
                
                printToken(yylineno, token, (token_names[STRING]).c_str());
                break;

            default:
                break;
        }

    }
    return 0;
}