#pragma once

#include<fstream>
#include<sstream>
#include "parser.hpp"

using namespace std;

class Generator {
public:
    inline Generator(NodeExit root) 
        : m_root(move(root))
        {

        }

    string generate() {
        stringstream output;
        output << "global _start \n _start: \n";
        output << "    mov rax, 60 \n";
        output << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
        output << "    syscall";
        return output.str();
    }
private:
    const NodeExit m_root;
};  