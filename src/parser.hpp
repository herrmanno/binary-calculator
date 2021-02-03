#pragma once
#include "./binary.hpp"

#define METHOD_NOT_IMPLEMENTED assert(false && "Method not implemented")

/**
 * Represents supported operators / reserved symbols (e.g. parenthesis)
 */
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

/**
 * @returns an operator's precedence
 */
unsigned int precedence(Op op);


/**
 * Represents a parsed / evaluated token value
 *
 * A token may hold a value which *must* be representable in at least one of the following forms:
 * - Operator
 * - Binary
 * - int
 * - bool
 *
 * The token's `type` indicates which representation will be guaranteed available, other
 * representations are possible but not guaranteed to succeed.
 */
struct Token {
    /** A token's type
     *
     * Acts as a tag in the tagged union type `Token` with instances
     * - `OpToken` (`Token::Type::Op`)
     * - `BinToken` (`Token::Type::Bin`)
     * - `IntToken` (`Token::Type::Int`)
     * - `BoolToken` (`Token::Type::Bool`)
     */
    enum class Type {
        /** Indicates that a token of this type can represent its value as Operator */
        Op,
        /** Indicates that a token of this type can represent its value as Binary */
        Bin,
        /** Indicates that a token of this type can represent its value as int */
        Int,
        /** Indicates that a token of this type can represent its value as bool */
        Bool
    };

    /** The token's primary type */
    const Type type;

    /**
     * @returns this token's value as instance of `Op` enum
     *
     * Is only guaranteed not to throw if `type` is `Type::Op`.
     */
    virtual Op op() const { METHOD_NOT_IMPLEMENTED; return {}; };

    /**
     * @returns this token's value as `Binary` instance
     *
     * Is only guaranteed not to throw if `type` is `Type::Bin`.
     */
    virtual Binary bin() const { METHOD_NOT_IMPLEMENTED; return {}; };

    /**
     * @returns this token's value as int
     *
     * Is only guaranteed not to throw if `type` is `Type::Int`.
     */
    virtual int num() const { METHOD_NOT_IMPLEMENTED; return {}; };

    /**
     * @returns this token's value as bool
     *
     * Is only guaranteed not to throw if `type` is `Type::Bool`.
     */
    virtual bool boolean() const { METHOD_NOT_IMPLEMENTED; return {}; };

    /**
     * @returns this token's type as human readable string
     */
    std::string typeName() const;

    /**
     * Creates a value-less token of type `t`
     * @param t the token's type
     */
    Token(Type t);

    /**
     * No-Op. Exists so that Subclasses may declare a custom destructor.
     */
    virtual ~Token() {};
};

/**
 * A token that holds a value which is representable as `Op`
 */
struct OpToken final : public Token {
    /**
     * Creates a Token of type `Token::Type::Op`
     * @param op the value this token holds
     */
    OpToken(Op op);

    Op op() const override;

    private:
    Op m_op;
};

/**
 * A token that holds a value which is representable as `Binary`
 */
struct BinToken : public Token {
    /**
     * Creates a Token of type `Token::Type::Bin`
     * @param bin the value this token holds
     */
    BinToken(Binary bin);

    Binary bin() const override;

    private:
    Binary m_bin;
};

/**
 * A token that holds a value which is representable as `int`
 */
struct IntToken : public Token {
    /**
     * Creates a Token of type `Token::Type::Int`
     * @param i the value this token holds
     */
    IntToken(int i);

    int num() const override;

    private:
    int m_n;
};

/**
 * A token that holds a value which is representable as `bool`
 */
struct BoolToken : public Token {
    /**
     * Creates a Token of type `Token::Type::Bool`
     * @param b the value this token holds
     */
    BoolToken(bool b);

    bool boolean() const override;

    private:
    bool m_b;
};

/**
 * Evaluates an expression to a single token containing its final value
 * @param expr the expression to evaluate as string
 * @throws if the expression is misformed in terms of syntax or types
 */
std::shared_ptr<Token> evaluate(const std::string& expr);

/**
 * Evaluates a vector of tokens that are formed in
 * <a href="https://en.wikipedia.org/wiki/Reverse_Polish_notation">RPN</a>
 * @param expr the expression to evaluate
 */
static std::shared_ptr<Token> evaluate(std::vector<std::shared_ptr<Token> >& expr);

/**
 * Parses an expression to a vector of tokens
 *
 * @param expr the expression to parse
 * @throws if the expression is misformed in terms of syntax
 */
static std::vector<std::shared_ptr<Token> > tokenize(const std::string&);