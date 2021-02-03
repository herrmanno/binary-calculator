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
    Div,

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
     * - `NumToken` (`Token::Type::Num`)
     * - `BoolToken` (`Token::Type::Bool`)
     */
    enum class Type {
        /** Indicates that a token of this type can represent its value as Operator */
        Op,
        /** Indicates that a token of this type can represent its value as Binary */
        Bin,
        /** Indicates that a token of this type can represent its value as number */
        Num,
        /** Indicates that a token of this type can represent its value as bool */
        Bool
    };

    /** The token's primary type */
    const Type type;

    /**
     * Creates a value-less token of type `t`
     * @param t the token's type
     */
    Token(Type t);

    virtual ~Token() = default;

    /**
     * @returns this token's value as instance of `Op` enum
     *
     * Is only guaranteed not to throw if `type` is `Type::Op`.
     */
    virtual Op op() const;

    /**
     * @returns this token's value as `Binary` instance
     *
     * Is only guaranteed not to throw if `type` is `Type::Bin`.
     */
    virtual Binary bin() const;

    /**
     * @returns this token's second value (if present) as `Binary` instance
     */
    virtual Binary bin2() const;

    /**
     * @returns this token's value as number
     *
     * Is only guaranteed not to throw if `type` is `Type::Num`.
     */
    virtual long num() const;

    /**
     * @returns this token's value as bool
     *
     * Is only guaranteed not to throw if `type` is `Type::Bool`.
     */
    virtual bool boolean() const;

    /**
     * The token't string representation
     * @returns the string representation of this token
     */
    virtual std::string to_str() const;

    /**
     * @returns this token's type as human readable string
     */
    std::string typeName() const;
    
    friend std::ostream& operator<<(std::ostream& os, Token& t);

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
struct BinToken final : public Token {
    /**
     * Creates a Token of type `Token::Type::Bin`
     * @param bin the value this token holds
     */
    BinToken(Binary bin);

    Binary bin() const override;

    virtual std::string to_str() const override;

    private:
    Binary m_bin;
};

/**
 * A token that holds a two values which are representable as `Binary`
 */
struct Bin2Token final : public Token {
    /**
     * Creates a Token of type `Token::Type::Bin`
     * @param b1 the first value this token holds
     * @param b2 the second value this token holds
     */
    Bin2Token(Binary b1, Binary b2);

    Binary bin() const override;

    Binary bin2() const override;

    virtual std::string to_str() const override;

    private:
    Binary m_bin1;
    Binary m_bin2;
};

/**
 * A token that holds a value which is representable as `long`
 */
struct NumToken final : public Token {
    /**
     * Creates a Token of type `Token::Type::Num`
     * @param l the value this token holds
     */
    NumToken(long l);

    long num() const override;

    virtual std::string to_str() const override;

    private:
    long m_n;
};

/**
 * A token that holds a value which is representable as `bool`
 */
struct BoolToken final : public Token {
    /**
     * Creates a Token of type `Token::Type::Bool`
     * @param b the value this token holds
     */
    BoolToken(bool b);

    bool boolean() const override;

    virtual std::string to_str() const override;

    private:
    bool m_b;
};

/**
 * Evaluates an expression to a single token containing its final value
 * @param expr the expression to evaluate as string
 * @throws if the expression is misformed in terms of syntax or types
 */
std::shared_ptr<Token> evaluate(const std::string& expr);
