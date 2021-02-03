#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "./binary.hpp"
#include "./parser.hpp"

std::shared_ptr<Token> evaluate(const std::string& s) {
    std::vector<std::shared_ptr<Token>> input = lex(s);
    std::vector<Op> operators;
    std::vector<std::shared_ptr<Token>> output;

    // build RPN stack by using shunting yard algorithm
    // see http://mathcenter.oxford.emory.edu/site/cs171/shuntingYardAlgorithm/
    for (auto &t : input) {
        if (t->type == Token::Type::Bin) {
            output.push_back(t);
        } else if (t->op() == Op::LPar) {
            operators.push_back(t->op());
        } else if (t->op() == Op::RPar) {
            while (operators.back() != Op::LPar) {
                if (operators.empty()) {
                    throw std::invalid_argument("Bad expression. Found ')' wihtout matching '('");
                }
                output.push_back(std::make_shared<OpToken>(OpToken(operators.back())));
                operators.pop_back();
            }
            operators.pop_back(); // pop '('
        } else if (operators.empty() || precedence(t->op()) > precedence(operators.back())) {
            operators.push_back(t->op());
        } else if (precedence(t->op()) <= precedence(operators.back())) {
            do {
                output.push_back(std::make_shared<OpToken>(OpToken(operators.back())));
                operators.pop_back();
            } while (!operators.empty() && precedence(t->op()) <= precedence(operators.back()));
            operators.push_back(t->op());
        }
    }

    while (operators.size()) {
        Op op = operators.back();
        if (op == Op::LPar) {
            throw std::invalid_argument("Bad expression. Found '(' wihtout matching ')'");
        }
        output.push_back(std::make_shared<OpToken>(OpToken(op)));
        operators.pop_back();
    }

    //evaluate RPN expression
    return evaluate(output);
}

std::shared_ptr<Token> evaluate(std::vector<std::shared_ptr<Token>>& expr) {
    std::vector<std::shared_ptr<Token>> s;
    
    // TODO: make bool operators also work on two IntTokens (or two BoolTokens)
    for (auto t : expr) {
        if (t->type == Token::Type::Bin) {
            s.push_back(t);
        } else if (t->type == Token::Type::Bool) {
            s.push_back(t);
        } else if (t->op() == Op::And) {
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            // TODO: assert two binary terms
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin() & operand2->bin())));
        } else if (t->op() == Op::Or) {
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            // TODO: assert two binary terms
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin() | operand2->bin())));
        } else if (t->op() == Op::Xor) {
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            // TODO: assert two binary terms
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin() ^ operand2->bin())));
        } else if (t->op() == Op::Concat) {
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            // TODO: assert two binary terms
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin().concat(operand2->bin()))));
        } else if (t->op() == Op::EQ) {
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            // TODO: assert two identicl terms [binary, int, bool]
            s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() == operand2->bin())));
        } else if (t->op() == Op::NEQ) {
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            // TODO: assert two identicl terms [binary, int, bool]
            s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() != operand2->bin())));
        } else if (t->op() == Op::GT) {
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            // TODO: assert two identicl terms [binary, int, bool]
            s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() > operand2->bin())));
        } else if (t->op() == Op::LT) {
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            // TODO: assert two identicl terms [binary, int, bool]
            s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() < operand2->bin())));
        } else if (t->op() == Op::Parity) {
            auto operand = s.back(); s.pop_back();
            // TODO: assert binary terms
            s.push_back(std::make_shared<IntToken>(IntToken(operand->bin().parity())));
        }
    }

    assert(s.size() == 1);
    return s.front();
}

std::vector<std::shared_ptr<Token>> lex(const std::string& s) {
    std::deque<char> input(s.begin(), s.end());
    std::vector<std::shared_ptr<Token>> output;
    
    std::string curr("");

    auto push_binary_token = [&]() {
        if (!curr.empty()) {
            Binary b;
            std::stringstream(curr) >> b;
            curr.clear();
            BinToken tok(b);
            output.push_back(std::make_shared<BinToken>(tok));
        }
    };

    auto push_operator_token = [&](Op op) {
        OpToken tok(op);
        output.push_back(std::make_shared<OpToken>(tok));
    };

    while (input.size()) {
        while (isspace(input.front())) {
            input.pop_front();
        }
        if (input.front() == '1' || input.front() == '0') {
            curr.push_back(input.front());
            input.pop_front();
        } else if (input.front() == '&') {
            push_binary_token();
            push_operator_token(Op::And);
            input.pop_front();
        } else if (input.front() == '|') {
            push_binary_token();
            push_operator_token(Op::Or);
            input.pop_front();
        } else if (input.front() == '^') {
            push_binary_token();
            push_operator_token(Op::Xor);
            input.pop_front();
        } else if (input.front() == '.') {
            push_binary_token();
            push_operator_token(Op::Concat);
            input.pop_front();
        } else if (input.front() == '>') {
            push_binary_token();
            push_operator_token(Op::GT);
            input.pop_front();
        } else if (input.front() == '<') {
            push_binary_token();
            push_operator_token(Op::LT);
            input.pop_front();
        } else if (input.front() == '=') {
            if (input.at(1) == '=') { //TODO: handle abprupt end of stream
                push_binary_token();
                push_operator_token(Op::EQ);
                input.pop_front();
                input.pop_front();
            } else {
                throw std::invalid_argument("Invalid input character '" + std::string(1, input.front()) + "'. Expected '='");
            }
        } else if (input.front() == '!') {
            if (input.at(1) == '=') { //TODO: handle abprupt end of stream
                push_binary_token();
                push_operator_token(Op::NEQ);
                input.pop_front();
                input.pop_front();
            } else {
                throw std::invalid_argument("Invalid input character '" + std::string(1, input.front()) + "'. Expected '='");
            }
        } else if (input.front() == 'p') {
            push_binary_token();
            push_operator_token(Op::Parity);
            input.pop_front();
        } else if (input.front() == '(') {
            push_binary_token();
            push_operator_token(Op::LPar);
            input.pop_front();
        } else if (input.front() == ')') {
            push_binary_token();
            push_operator_token(Op::RPar);
            input.pop_front();
        } else {
            throw std::invalid_argument("Invalid input character '" + std::string(1, input.front()) + "'");
        }
    }

    push_binary_token();

    return output;
}


unsigned int precedence(Op op) {
    switch (op) {
        case Op::Parity:
            return 4;

        case Op::And:
        case Op::Or:
        case Op::Xor:
        case Op::Concat:
            return 3;

        case Op::EQ:
        case Op::NEQ:
        case Op::GT:
        case Op::LT:
            return 2;

        default:
            return 0;
    }
};