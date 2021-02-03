#pragma once
#include <vector>
#include <tuple>
#include <istream>
#include <ostream>
#include <functional>

/**
 * Representation of (unsigned) binary numbers of arbritary length
 */
class Binary {
    public:
    /**
     * Construct an *empty* number (equivalent to `NaN`)
     */
    Binary();

    /**
     * Constructs a Binary instance from a digit of base 10
     * @param n the number (of base 10) that shall be represented as binary
     */
    Binary(unsigned long long int n);

    /**
     * Constructs a Binary instance from a bit vector
     *
     * where `true` equals 1 and `false` equals 0
     *
     * @param bits the bits that shall be represented as binary
     */
    Binary(const std::vector<bool>& bits);

    /**
     * Combines two Binary instances by bitwise application of '&'
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to combine with `this`
     * @returns the result of applying '&' bitwise to `this` and `rhs`
     */
    Binary operator&(const Binary& rhs) const;

    /**
     * Assigns the result of `this & rhs` to `this`
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to combine with `this`
     */
    Binary& operator&=(const Binary& rhs);

    /**
     * Combines two Binary instances by bitwise application of '|'
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to combine with `this`
     * @returns the result of applying '|' bitwise to `this` and `rhs`
     */
    Binary operator|(const Binary& rhs) const;

    /**
     * Assigns the result of `this | rhs` to `this`
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to combine with `this`
     */
    Binary& operator|=(const Binary& rhs);

    /**
     * Combines two Binary instances by bitwise application of '^'
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to combine with `this`
     * @returns the result of applying '^' bitwise to `this` and `rhs`
     */
    Binary operator^(const Binary& rhs) const;

    /**
     * Assigns the result of `this ^ rhs` to `this`
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to combine with `this`
     */
    Binary& operator^=(const Binary& rhs);

    /**
     * Subtracts a binary from `this`
     *
     * @param rhs the subtrahend
     * @returns the result of subtracting `rhs` from this
     */
    Binary operator-(const Binary& rhs) const;

    /**
     * Divides `this` by another binary
     *
     * @param divisor the divisor
     * @returns the quotient (w/o potential remainder) of dividing `this` by `rhs`
     * @throws std::domain_error if `divisor` equals zero
     */
    Binary operator/(const Binary& divisor) const;

    /**
     * Divides `this` by another binary
     *
     * @param divisor the divisor
     * @returns a tuple containing the quotient and remainder of division by `divisor`
     * @throws std::domain_error if `divisor` equals zero
     */
    std::tuple<Binary,Binary> div(const Binary& divisor) const;

    /**
     * Compares `this` to binary in regards of the '>' relation
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to compare to `this`
     * @returns `true` if `this` represents a greater number than `rhs`, `false` otherwise
     */
    bool operator>(const Binary& rhs) const;

    /**
     * Compares `this` to binary in regards of the '<' relation
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to compare to `this`
     * @returns `true` if `rhs` represents a greater number than `this`, `false` otherwise
     */
    bool operator<(const Binary& rhs) const;

    /**
     * Compares `this` to binary in regards of the '==' relation
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to compare to `this`
     * @returns `true` if `this` represents the same number than `rhs`, `false` otherwise
     */
    bool operator==(const Binary& rhs) const;

    /**
     * Compares `this` to binary in regards of the '!=' relation
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param rhs the binary to compare to `this`
     * @returns `true` if `this` represents *not* the same number than `rhs`, `false` otherwise
     */
    bool operator!=(const Binary& rhs) const;

    /**
     * Pushes a binary to an outstream in serialized form
     *
     * @example
     *      Binary b(5); // b equals 101
     *      os << b; // writes "101" to os
     *
     * @param os the oustream to push to
     * @param bin the binary to push to os
     * @returns a reference to the outputstream pushed to
     */
    friend std::ostream& operator<<(std::ostream& os, const Binary& bin);

    /**
     * Pulls a binary (in serialized form) from an instream
     *
     * @example
     *      Binary b;
     *      std::stringstream("101") >> b;
     *      // b now equals 101
     *
     * @param is the intream to pull from
     * @param bin the binary deserialize from `is`
     * @returns a reference to the instream pulled from
     */
    friend std::istream& operator>>(std::istream& is, Binary& bin);

    /**
     * Concats a binary to `this`
     *
     * @example
     *      Binary a(5); // equals 101
     *      Binary b(2); // equals 10
     *      a.concat(b); // equals 10110
     *
     * @param other the binary to concat to this
     * @returns a binary representing `this` concated w/ `other`
     */
    Binary concat(const Binary& other) const;

    /**
     * Computes `this`' parity
     *
     * @example
     *      Binary a(1); // equals 1
     *      Binary b(5); // equals 101
     *      a.parity();  // = 1
     *      b.parity();  // = 2
     *
     * @returns the number of _1s_ of `this`
     */
    long parity() const;

    /**
     * Converts `this`' value to number base 10
     *
     * Overflows if the the number represented by `this` is greater than the maximum value
     * of type `unsigned long long int` (e.g. <a href="http://www.cplusplus.com/reference/climits/">(264-1) or greater</a>)
     * @returns a number equivalent to this' value
     */
    unsigned long long int to_l() const;

    /**
     * Converts `this`' value to string
     *
     * @example
     *      Binary b(5); // equals 101
     *      b.to_str(); // "101"
     *
     * @returns the string representation of `this`' value
     */
    std::string to_str() const;

    private:

    /**
     * Combines `this` w/ another binary by appliying a binary function bitwise
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param other the binary to combine `this` with
     * @param combiner the binary function used to combine two bits
     * @returns the bit vector created by applying `combiner` on every ordered bit pair of `this` and `other`
     */
    std::vector<bool> combine(const Binary& other, std::function<bool(bool,bool)> combiner) const;

    /**
     * Compares `this` w/ another binary by performing a bitwise comparison
     *
     * Missing bits in `this` or `rhs` are treated as `false`.
     *
     * @param other the binary to compare `this` with
     * @returns 0 if `this` and `other` are equivalent, < 0, if `this` is less than `other`, > 0 otherwise
     */
    int compare(const Binary& other) const;

    /**
     * Replaces the value represented by `this` by a bit sequence (in serialized form)
     * @param s the bit sequence to emplace `this`' value with
     * @throws if `s` is no valid bit order
     *
     * @example
     *      Binary b(1);        // equals 1
     *      b.emplace("101");
     *      b                   // equals 101
     */
    void emplace(const std::string& s);

    /**
     * The internal bit order representing `this`' value
     */
    std::vector<bool> m_bits;
};
