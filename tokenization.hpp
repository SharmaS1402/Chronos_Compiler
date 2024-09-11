#pragma once

#include<string> 
#include<vector>
#include<experimental/optional>
#include<cctype>

using namespace std;

enum class TokenType {
    exit,
    int_lit, 
    semi
};

struct Token {
    TokenType type;
    experimental::optional<string> value;
};

bool has_value(const std::experimental::optional<char>& opt) {
    return static_cast<bool>(opt);  // Converts the optional to bool
}

class Tokenizer {
    public:
    inline explicit Tokenizer(string src) 
        : m_src(move(src))
    {

    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        string buf = "";
        experimental::nullopt;
        while(peak().value() != NULL) {
            if(isalpha(peak().value())) {
                buf.push_back(consume());
                while(peak().value() != NULL && isalnum(peak().value())) {
                    buf.push_back(consume());
                }
                if(buf == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                }
                else {
                    cerr << "You messed up.." << endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(isdigit(peak().value())) {
                buf.push_back(consume());
                while(peak().value() != NULL && isdigit(peak().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({.type = TokenType:: int_lit, .value = buf});
                buf.clear();
            }
            else if(peak().value() == ';') {
                tokens.push_back({.type = TokenType::semi}); 
                consume();               
                continue;
            }
            else if(isspace(peak().value())) {
                consume();
                continue;
            }
            else {
                cerr << "You messed up " << endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }

    private : 
        const string m_src;
        int m_index = 0;
        
        [[nodiscard]]experimental::optional<char> peak(int ahead = 1) {
            if(m_index + ahead > m_src.length()) {
                return NULL;
            }
            else {
                return m_src.at(m_index);
            }
        }

        inline char consume() {
            return m_src.at(m_index++);
        }
};