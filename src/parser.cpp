#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "./binary.hpp"
#include "./parser.hpp"

/*--------------------*
 *         Op         *
 *--------------------*/

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
}

/*--------------------*
 *       Token        *
 *--------------------*/

Token::Token(Token::Type t) : type { t } {}

std::string Token::typeName() const {
        switch (type) {
            case Type::Op:
                return "Operator";
            case Type::Bin:
                return "Binary";
            case Type::Int:
                return "Number";
            case Type::Bool:
                return "Bool";
        }
    };

/* OpToken */

OpToken::OpToken(Op op) : Token(Type::Op), m_op { op } {}

Op OpToken::op() const {
    return m_op;
}

/* BinToken */

BinToken::BinToken(Binary bin) : Token(Type::Bin), m_bin { bin } {}

Binary BinToken::bin() const {
    return m_bin;
}

/* IntToken */

IntToken::IntToken(int i) : Token(Type::Int), m_n { i } {}

int IntToken::num() const {
    return m_n;
}

/* BoolToken */

BoolToken::BoolToken(bool b) : Token(Type::Bool), m_b { b } {}

bool BoolToken::boolean() const {
    return m_b;
}

/*--------------------*
 *       Parser       *
 *--------------------*/

std::shared_ptr<Token> evaluate(const std::string& s) {
    std::vector<std::shared_ptr<Token> > input = tokenize(s);
    std::vector<Op> operators;
    std::vector<std::shared_ptr<Token> > output;

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

std::shared_ptr<Token> evaluate(std::vector<std::shared_ptr<Token> >& expr) {
    // s acts as stack during evaluation
    // value tokens are pushed to the stack when read while
    // operator tokens pop their arguments from the stack and
    // push the operations result back to the stack
    std::vector<std::shared_ptr<Token> > s;

    // iterate through tokens of expr
    for (auto t : expr) {
        if (t->type == Token::Type::Bin) {
            s.push_back(t);
        } else if (t->type == Token::Type::Bool) {
            s.push_back(t);
        } else if (t->op() == Op::And) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            if (operand1->type != Token::Type::Bin || operand2->type != Token::Type::Bin) {
                throw std::invalid_argument("Cannot perform '&' on operands of type " + operand2->typeName() + " and " + operand1->typeName());
            }
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin() & operand2->bin())));
        } else if (t->op() == Op::Or) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            if (operand1->type != Token::Type::Bin || operand2->type != Token::Type::Bin) {
                throw std::invalid_argument("Cannot perform '|' on operands of type " + operand2->typeName() + " and " + operand1->typeName());
            }
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin() | operand2->bin())));
        } else if (t->op() == Op::Xor) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand1 = s.back(); s.pop_back();
            auto operand2 = s.back(); s.pop_back();
            if (operand1->type != Token::Type::Bin || operand2->type != Token::Type::Bin) {
                throw std::invalid_argument("Cannot perform '^' on operands of type " + operand2->typeName() + " and " + operand1->typeName());
            }
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin() ^ operand2->bin())));
        } else if (t->op() == Op::Concat) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            if (operand1->type != Token::Type::Bin || operand2->type != Token::Type::Bin) {
                throw std::invalid_argument("Cannot perform 'concat' on operands of type " + operand1->typeName() + " and " + operand2->typeName());
            }
            s.push_back(std::make_shared<BinToken>(BinToken(operand1->bin().concat(operand2->bin()))));
        } else if (t->op() == Op::EQ) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            if (operand1->type == Token::Type::Bin && operand2->type == Token::Type::Bin) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() == operand2->bin())));
            } else if (operand1->type == Token::Type::Int && operand2->type == Token::Type::Int) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->num() == operand2->num())));
            } else if (operand1->type == Token::Type::Bool && operand2->type == Token::Type::Bool) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->boolean() == operand2->boolean())));
            } else {
                throw std::invalid_argument("Cannot perform '==' on operands of type " + operand1->typeName() + " and " + operand2->typeName());
            }
        } else if (t->op() == Op::NEQ) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            if (operand1->type == Token::Type::Bin && operand2->type == Token::Type::Bin) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() != operand2->bin())));
            } else if (operand1->type == Token::Type::Int && operand2->type == Token::Type::Int) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->num() != operand2->num())));
            } else if (operand1->type == Token::Type::Bool && operand2->type == Token::Type::Bool) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->boolean() != operand2->boolean())));
            } else {
                throw std::invalid_argument("Cannot perform '!=' on operands of type " + operand1->typeName() + " and " + operand2->typeName());
            }
        } else if (t->op() == Op::GT) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            if (operand1->type == Token::Type::Bin && operand2->type == Token::Type::Bin) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() > operand2->bin())));
            } else if (operand1->type == Token::Type::Int && operand2->type == Token::Type::Int) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->num() > operand2->num())));
            } else {
                throw std::invalid_argument("Cannot perform '>' on operands of type " + operand1->typeName() + " and " + operand2->typeName());
            }
        } else if (t->op() == Op::LT) {
            if (s.size() < 2) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand2 = s.back(); s.pop_back();
            auto operand1 = s.back(); s.pop_back();
            if (operand1->type == Token::Type::Bin && operand2->type == Token::Type::Bin) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->bin() < operand2->bin())));
            } else if (operand1->type == Token::Type::Int && operand2->type == Token::Type::Int) {
                s.push_back(std::make_shared<BoolToken>(BoolToken(operand1->num() < operand2->num())));
            } else {
                throw std::invalid_argument("Cannot perform '<' on operands of type " + operand1->typeName() + " and " + operand2->typeName());
            }
        } else if (t->op() == Op::Parity) {
            if (s.size() < 1) {
                throw std::invalid_argument("Operation '&' is applied to too few arguments");
            }
            auto operand = s.back(); s.pop_back();
            if (operand->type == Token::Type::Bin) {
                s.push_back(std::make_shared<IntToken>(IntToken(operand->bin().parity())));
            } else {
                throw std::invalid_argument("Cannot perform 'p(arity)' on operand of type " + operand->typeName());
            }
        } else {
            throw std::invalid_argument("Unknown token of type " + t->typeName());
        }
    }

    // after successful evaluation the stack must contain a single value
    if (s.size() != 1) {
        throw std::invalid_argument("Expression cannot be evaluated to a single value");
    }

    return s.front();
}

std::vector<std::shared_ptr<Token> > tokenize(const std::string& s) {
    // TODO: use string_iterator instead of deque?
    std::deque<char> input(s.begin(), s.end());
    std::vector<std::shared_ptr<Token> > output;

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
            if (input.size() < 2) {
                throw std::invalid_argument("Unexpected end of input. Expected '='");
            } else if (input.at(1) == '=') {
                push_binary_token();
                push_operator_token(Op::EQ);
                input.pop_front();
                input.pop_front();
            } else {
                throw std::invalid_argument("Invalid input character '" + std::string(1, input.front()) + "'. Expected '='");
            }
        } else if (input.front() == '!') {
            if (input.size() < 2) {
                throw std::invalid_argument("Unexpected end of input. Expected '='");
            } else if (input.at(1) == '=') {
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

