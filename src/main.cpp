#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/optional>
#include <vector>

#include "../tokenization.hpp"

using namespace std;

// string tokenTypeToString(TokenType type) {
//     switch (type) {
//         case TokenType::exit: return "return";
//         case TokenType::int_lit: return "int_lit";
//         case TokenType::semi: return "semi";
//         default: return "unknown";
//     }
// }

// void printTokens(const vector<Token>& tokens) {
//     for (const auto& token : tokens) {
//         cout << "TokenType: " << tokenTypeToString(token.type);
//         if (token.value) {
//             cout << ", Value: " << *token.value;
//         } else {
//             cout << ", Value: (none)";
//         }
//         cout << endl;
//     }
// }

string tokens_to_asm(const vector<Token> &tokens) {
    stringstream output;
    int n = tokens.size();
    output << "global _start \n _start: \n";
    for(int i = 0; i < n; i++) {
        const Token& token = tokens.at(i);
        if(token.type == TokenType::exit) {
            if(i + 1 < n && tokens.at(i + 1).type == TokenType::int_lit) {
                if(i + 2 < n && tokens.at(i + 2).type == TokenType::semi) {
                    output << "    mov rax, 60 \n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }
    return output.str();
}
int main(int argc, char** argv) {

    if(argc != 2) {
        cout << "Incorrect Usage. Correct Usage is...." << endl;
        cerr << "nexus <input.nex>" << endl;
        return EXIT_FAILURE;
    }
    
    string contents;
    {
    fstream input(argv[1], ios::in);

    stringstream contents_stream;
    contents_stream << input.rdbuf();

    contents = contents_stream.str();
    }

    // cout << contents << endl;
    Tokenizer tokenizer(move(contents));
    vector<Token> tokens = tokenizer.tokenize();
    // printTokens(tokens);

    {
        fstream file("../out.asm", ios::out);
        file << tokens_to_asm(tokens);
    }

    return 0;

}