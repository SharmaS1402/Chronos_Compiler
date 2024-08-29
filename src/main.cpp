#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/optional>
#include <vector>

using namespace std;

enum class TokenType {
    _return,
    int_lit, 
    semi
};

struct Token {
    TokenType type;
    experimental::optional<string> value;
};

string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::_return: return "return";
        case TokenType::int_lit: return "int_lit";
        case TokenType::semi: return "semi";
        default: return "unknown";
    }
}

void printTokens(const vector<Token>& things) {
    for (const auto& token : things) {
        cout << "TokenType: " << tokenTypeToString(token.type);
        if (token.value) {
            cout << ", Value: " << *token.value;
        } else {
            cout << ", Value: (none)";
        }
        cout << endl;
    }
}

vector<Token> tokenize(const string &str) {
    vector<Token> tokens;
    string buf = "";
    for(int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if(isalpha(c)) {
            buf.push_back(c);
            i++;
            while(isalnum(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            if(buf == "return") {
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
            }
            else {
                cerr << "You messed up..." <<endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(isdigit(c)) {
            buf.push_back(c);
            i++;
            while(isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType:: int_lit, .value = buf});
            buf.clear();
        }
        else if(c == ';') {
            tokens.push_back({.type = TokenType::semi});
        }
        else if(isspace(c)) {
            continue;
        }
        else {
            cerr << "You messed up..." << endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

string tokens_to_asm(const vector<Token> &tokens) {
    stringstream output;
    output << "global _start \n _start: \n";
    for(int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if(token.type == TokenType::_return) {
            if(i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if(i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
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
   vector<Token> things =  tokenize(contents);
    // printTokens(things);

    // cout << tokens_to_asm(things) << endl;
    {
        fstream file("../out.asm", ios::out);
        file << tokens_to_asm(things);
    }

    return 0;

}