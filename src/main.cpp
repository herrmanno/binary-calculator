#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include "./binary.hpp"
#include "./parser.hpp"

static void print_help() {
    const char * help = R"EOF(
USAGE
        binary <expr>               evaluates and print <expr>
        binary <help|--help|-h>     shows this help
        binary -                    evaluates and prints expression read from stdin linewise

OUTPUT
        If the expression evaluates to a binary term the output consists of the binary
        representation followed by the base-10 representation of the term's value, seperated by a
        single space character.
        If the expression evaluates to a binary division the output consists of the binary space
        seperated binary representations of the quotient and remainder, where the remainder is
        preceeded by an 'R' character, followed by the base-10 representation of quotient and
        remainder where the remainer is preceeded by an 'R' character, again.
        followed by the base-10 representation of the term's value, seperated by a
        single space character.
        If the expresseion evaluates to a numerical term the output consists of the base-10
        representation of said value, only.
        If the expresseion evaluates to a boolean value the output consists of the word 'true' or
        'false', only.

EXAMPLE
        >>> binary "101 | 010"
        111 7

        >>> binary "101 / 010"
        10 R1 2 R1

        >>> binary "101 . 010"
        10110 22

        >>> binary "p (101 . 010)"
        3

        >>> binary "(p (101 . 010)) == (p (010 . 101))"
        true

EXPRESSION
        Expressions have to be of the following form may consist of binary literals and operators
        where the following operators are supported:
         - &    bitwise logic and
         - |    bitwise logic or
         - ^    bitwise logic xor
         - /    divition
         - .    concatenation
         - p    parity
         - ==   compariton of equality
         - !=   compariton of unequality
         - <    compariton of strict 'greater than'
         - >    compariton of strict 'less than'

        Note that the last four operators work on two binary operands of same type, whereas the
        first five operators are only defined on binary operands.
        Also note that a division's remainder is not part of further computations, if the divion
        result is fed to another operation.
    )EOF";

    std::cout << help << std::endl;
}

static void eval(std::string& s) {
    try {
        auto t = evaluate(s);
        std::cout << *t << std::endl;
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    } catch (std::domain_error& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unexpected error occured" << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc > 1) {
        std::vector<std::string> args;
        args.assign(argv + 1, argv + argc);
        std::ostringstream oss;
        std::copy(args.begin(), args.end(), std::ostream_iterator<std::string>(oss,""));
        std::string arg = oss.str();

        if (arg == "help" || arg == "--help" || arg == "-h") {
            print_help();
            return 0;
        } else if (arg == "-") {
            for (std::string line; std::getline(std::cin, line); ) {
                eval(line);
            }
        } else {
            eval(arg);
        }
    } else {
        print_help();
        return 0;
    }
}
