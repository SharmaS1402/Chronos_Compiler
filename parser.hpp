#pragma once

#include<iostream>
#include<vector>
#include<optional>
#include<string>
#include<variant>
#include "tokenization.hpp"

struct NodeExprIntlit {
    Token int_lit;
};

struct NodeExprIdent {
    Token ident;
};

struct NodeExpr {
    std::variant<NodeExprIntlit, NodeExprIdent> var;
};

struct NodeStmtExit {
    NodeExpr expr;
};

struct NodeStmtLet {
    Token ident;
    NodeExpr expr;
};

struct NodeStmt {
    std::variant<NodeStmtExit, NodeStmtLet> var;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
}; 


class Parser {
    public : 

    inline explicit Parser(std::vector<Token> tokens) 
        : m_tokens(std::move(tokens)) 
        {

        }
    optional<NodeExpr> parseExpr() {

        if(peak().has_value() && peak().value().type == TokenType::int_lit) {
            return NodeExpr{.var = NodeExprIntlit{.int_lit = consume()}};
        }
        else if(peak().has_value() && peak().value().type == TokenType::ident) {
            return NodeExpr{.var = NodeExprIdent{.ident = consume()}};
        }
        else {
            return {};
        }
    }

    optional<NodeStmt> parseStmt() {
            if(peak().has_value() && peak().value().type == TokenType::exit && peak(1).has_value() && peak(1).value().type == TokenType::open_par) {
                consume();
                consume();
                NodeStmtExit stmt_exit;
                if(auto node_expr = parseExpr()) {
                    stmt_exit = {.expr = node_expr.value()};
                }
                else {
                    std::cerr << "Invalid Expression " << std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peak().has_value() && peak().value().type == TokenType::close_par) {
                    consume();
                }
                else {
                    std::cerr << "Expected a ')' " << std::endl;
                    exit(EXIT_FAILURE);
                }
                if(peak().has_value() && peak().value().type == TokenType::semi) {
                    consume();
                }
                else {
                    std::cerr << "Expected a ';' " << std::endl;
                    exit(EXIT_FAILURE);
                }
                return NodeStmt{.var = stmt_exit};
            }
            else if(peak().has_value() && peak().value().type == TokenType::let && peak(1).has_value() && peak(1).value().type == TokenType::ident && peak(2).has_value() && peak(2).value().type == TokenType::eq) {

                consume();
                auto stmt_let = NodeStmtLet{.ident = consume()};
                consume();
                if(auto expr = parseExpr()) {
                    stmt_let.expr = expr.value();
                }
                else {
                    std::cerr << "Invalid Expression " << endl;
                }

                if(peak().has_value() && peak().value().type == TokenType::semi) {
                    consume();
                }
                else {
                    std::cerr << "Expected a ';' " << endl;
                }
                return NodeStmt{.var = stmt_let};
            }

            return {};
                   
    }
    optional<NodeProg> parseProg() {
        NodeProg prog;
        while(peak().has_value()) {
            if(auto stmt = parseStmt()) {
                prog.stmts.push_back(stmt.value());
            }
            else {
                std::cerr << "Invalid Statement" << endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }
    private : 
    const std::vector<Token> m_tokens;

            size_t m_index = 0;
        
        [[nodiscard]] inline optional<Token> peak(int offset = 0) {
            if(m_index + offset >= m_tokens.size()) {
                return {};
            }
            else {
                return m_tokens.at(m_index + offset);
            }
        }

        inline Token consume() {
            return m_tokens.at(m_index++);
        }
};