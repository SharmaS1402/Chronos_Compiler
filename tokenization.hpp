#pragma once

#include<string> 
#include<vector>
#include<optional>
#include<cctype>

using namespace std;

enum class TokenType {
    exit,
    int_lit, 
    semi,
    open_par,
    close_par,
    ident,
    let,
    eq
};

struct Token {
    TokenType type;
    optional<string> value;
};

bool has_value(const optional<char>& opt) {
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
        while(peak().has_value()) {
            if(isalpha(peak().value())) {
                buf.push_back(consume());
                while(peak().has_value() && isalnum(peak().value())) {
                    buf.push_back(consume());
                }
                if(buf == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                }
                else if(buf == "let") {
                    tokens.push_back({.type = TokenType::let});
                    buf.clear();
                    continue;
                }
                else {
                    tokens.push_back({.type = TokenType::ident, .value = buf});
                    buf.clear();
                    continue;
                }
            }
            else if(isdigit(peak().value())) {
                buf.push_back(consume());
                while(peak().has_value() && isdigit(peak().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({.type = TokenType:: int_lit, .value = buf});
                buf.clear();
            }
            else if(peak().value() == '(') {
                consume();
                tokens.push_back({.type = TokenType::open_par});
            }
            else if(peak().value() == ')') {
                consume();
                tokens.push_back({.type = TokenType::close_par});
            }
            else if(peak().value() == '=') {
                consume();
                tokens.push_back({.type = TokenType::eq});
            }
            else if(peak().value() == ';') {
                consume();               
                tokens.push_back({.type = TokenType::semi}); 
                continue;
            }
            else if(isspace(peak().value())) {
                consume();
                continue;
            }
            else {
                cerr << "You messed up" << endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }

    private : 
        const string m_src;
        int m_index = 0;
        
        [[nodiscard]]optional<char> peak(int offset = 0) {
            if(m_index + offset >= m_src.length()) {
                return {};
            }
            else {
                return m_src.at(m_index + offset);
            }
        }

        inline char consume() {
            return m_src.at(m_index++);
        }
};