#pragma once
#include "./binary.hpp"

#define METHOD_NOT_IMPLEMENTED assert(false && "Method not implemented")

enum class Op {
    GT,
    LT,
    EQ,
    NEQ,

    And,
    Or,
    Xor,
    Concat,

    Parity,
    
    LPar,
    RPar,
};

unsigned int precedence(Op op);


struct Token {
    enum class Type { Op, Bin, Int, Bool };
    const Type type;
    
    virtual Op op() { METHOD_NOT_IMPLEMENTED; return {}; };

    virtual Binary bin() { METHOD_NOT_IMPLEMENTED; return {}; };

    virtual int num() { METHOD_NOT_IMPLEMENTED; return {}; };

    virtual bool boolean() { METHOD_NOT_IMPLEMENTED; return {}; };

    Token(Type t) : type { t }{ }

    virtual ~Token() {}
};

struct OpToken : public Token {
    OpToken(Op op) : Token(Type::Op), m_op { op } {}

    Op op() override {
        return m_op;
    }

    private:
    Op m_op;
};

struct BinToken : public Token {
    BinToken(Binary bin) : Token(Type::Bin), m_bin { bin } {}

    Binary bin() override {
        return m_bin;
    }

    private:
    Binary m_bin;
};

struct IntToken : public Token {
    IntToken(int i) : Token(Type::Int), m_n { i } {}

    int num() override {
        return m_n;
    }

    private:
    int m_n;
};

struct BoolToken : public Token {
    BoolToken(bool b) : Token(Type::Bool), m_b { b } {}

    bool boolean() override {
        return m_b;
    }

    private:
    bool m_b;
};

std::shared_ptr<Token> evaluate(const std::string&);

std::shared_ptr<Token> evaluate(std::vector<std::shared_ptr<Token>>&);

std::vector<std::shared_ptr<Token>> lex(const std::string&);