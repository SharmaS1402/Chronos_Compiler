#pragma once

#include<fstream>
#include<sstream>
#include<unordered_map>
#include "parser.hpp"

using namespace std;

class Generator {
public:
    inline Generator(NodeProg prog) 
        : m_prog(move(prog))
        {

        }
    
    void gen_expr(const NodeExpr &expr) {
        struct ExprVisitor {
            Generator *gen;
            void operator()(const NodeExprIntlit &expr_int_lit) {
                gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->push("rax");
            }
            void operator()(const NodeExprIdent &expr_ident) {
                if(gen->m_vars.find(expr_ident.ident.value.value()) == gen->m_vars.end()) {
                    cerr << "Undeclared Identifier " << expr_ident.ident.value.value() << endl;
                    exit(EXIT_FAILURE);
                }
                const auto &var = gen->m_vars.at(expr_ident.ident.value.value());
                stringstream offset;

                offset << "QWORD [ rsp + " <<(gen->m_stack_size - var.stack_loc - 1) * 8 << "]\n";
                gen->push(offset.str());
            }
        };

        ExprVisitor visitor {.gen = this};
        visit(visitor, expr.var);
    }
    void gen_stmt(const NodeStmt &stmt) {
        struct StmtVisitor {
            Generator *gen;
            void operator()(const NodeStmtExit &stmt_exit) const {
                gen->gen_expr(stmt_exit.expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";                
            }

            void operator()(const NodeStmtLet &stmt_let) {
                if(gen->m_vars.find(stmt_let.ident.value.value()) != gen->m_vars.end()) {
                    cerr << "Identifier already declared " << stmt_let.ident.value.value() << endl;
                    exit(EXIT_FAILURE);
                }
                gen->m_vars.insert({stmt_let.ident.value.value(), Var{.stack_loc = gen->m_stack_size}});
                gen->gen_expr(stmt_let.expr);
            }
        };
        StmtVisitor visitor {.gen = this};
        std::visit(visitor, stmt.var);
    }

    string gen_prog() {
        m_output << "global _start \n _start: \n";

        for(const NodeStmt &stmt : m_prog.stmts) {
            gen_stmt(stmt);
        }

        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0 " << "\n";
        m_output << "    syscall\n";
        return m_output.str();
    }
private:

    void push(const string &reg) {
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const string &reg) {
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var {
        size_t stack_loc;
    };

    size_t m_stack_size = 0;
    const NodeProg m_prog;
    stringstream m_output;
    unordered_map<string,Var> m_vars{};
};  