#pragma once

#include<iostream>
#include<vector>
#include<experimental/optional>
#include<string>
#include "tokenization.hpp"

struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};


class Parser {
    public : 

    inline explicit Parser(std::vector<Token> tokens) 
        : m_tokens(std::move(tokens)) 
        {

        }
    std::experimental::optional<NodeExpr> parseExpr() {

        if(peak() != std::experimental::nullopt && peak().value().type == TokenType::int_lit) {
            return NodeExpr{.int_lit = consume()};
        }
        else {
            return std::experimental::nullopt;
        }
    }
    std::experimental::optional<NodeExit> parse() {
        std::experimental::optional<NodeExit> exit_node;

        while(peak() != std::experimental::nullopt) {
            if(peak().value().type == TokenType::exit) {
                consume();
                auto node_expr = parseExpr();
                if(node_expr != std::experimental::nullopt) {
                    exit_node = NodeExit{.expr = node_expr.value()};
                }
                else {
                    std::cerr << "Invalid Expression " << std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peak() != std::experimental::nullopt && peak().value().type == TokenType::semi) {
                    consume();
                }
                else {
                    std::cerr << "Expected a semi-colon" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        m_index = 0; 
        return exit_node;
    }
    private : 
    const std::vector<Token> m_tokens;

            size_t m_index = 0;
        
        [[nodiscard]] inline std::experimental::optional<Token> peak(int ahead = 1) {
            if(m_index + ahead > m_tokens.size()) {
                return std::experimental::nullopt;
            }
            else {
                return m_tokens.at(m_index);
            }
        }

        inline Token consume() {
            return m_tokens.at(m_index++);
        }
};