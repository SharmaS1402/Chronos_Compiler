#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/optional>
#include <vector>

#include "../parser.hpp"
#include "../generation.hpp"
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
    Parser parser(move(tokens));
    experimental::optional<NodeExit> tree = parser.parse();
    if(tree == experimental::nullopt) {
        cerr << "No exit statement found" << endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(tree.value());

    {
        fstream file("../out.asm", ios::out);
        file << generator.generate();
    }

    return 0;

}