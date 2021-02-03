#include <iostream>
#include <sstream>
#include <vector>
#include "./binary.hpp"
#include "./parser.hpp"

int main(int argc, char** argv) {
    // Binary b1(6);
    // Binary b2(5);
    // Binary b3;
    // std::string s = "1010";
    // std::istringstream ss(s);
    // ss >> b3;

    // std::cout << b1 << "\t" << b1.to_l() << std::endl;
    // std::cout << b2 << "\t" << b2.to_l() << std::endl;
    // std::cout << b3 << "\t" << b3.to_l() << std::endl;
    // std::cout << (b1 & b2) << std::endl;
    // b1 &= b2;
    // std::cout << b1 << std::endl;
    // std::cout << (b1 == b2) << std::endl;
    // std::cout << (b1 != b2) << std::endl;
    // std::cout << (b1 > b2) << std::endl;
    // std::cout << (b1 < b2) << std::endl;
    // std::cout << (b1.concat(b2)) << std::endl;
    // std::cout << (b1.concat(b2)).parity() << std::endl;

    if (argc > 1) {
        std::vector<std::string> args;
        args.assign(argv + 1, argv + argc);
        std::ostringstream oss;
        std::copy(args.begin(), args.end(), std::ostream_iterator<std::string>(oss,""));
        std::string arg = oss.str();
        
        auto t = evaluate(arg);
        if (t->type == Token::Type::Bin) {
            std::cout << t->bin() << " " << t->bin().to_l() << std::endl;
        } else if (t->type == Token::Type::Bool) {
            std::cout << (t->boolean() ? "true" : "false") << std::endl;
        } else if (t->type == Token::Type::Int) {
            std::cout << t->num() << std::endl;
        }
    }

    return 0;
}