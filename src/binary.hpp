#pragma once
#include <vector>
#include <istream>
#include <ostream>
#include <functional>

class Binary {
    public:
    Binary();
    Binary(unsigned long long int);
    Binary(const std::vector<bool>& bits);

    Binary operator&(const Binary& rhs) const;
    Binary& operator&=(const Binary& rhs);

    Binary operator|(const Binary& rhs) const;
    Binary& operator|=(const Binary& rhs);

    Binary operator^(const Binary& rhs) const;
    Binary& operator^=(const Binary& rhs);

    bool operator>(const Binary& rhs) const;
    bool operator<(const Binary& rhs) const;
    bool operator!=(const Binary& rhs) const;
    bool operator==(const Binary& rhs) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Binary& bin);
    friend std::istream& operator>>(std::istream& os, Binary& bin);

    Binary concat(const Binary& other) const;
    // TODO: implement
    // Binary divide(const Binary& other) const;

    size_t parity() const;
    unsigned long long int to_l() const;
    std::string to_str() const;

    private:
    enum class CMP {
        EQ,
        GT,
        LT
    };
    static CMP compareBits(bool b1, bool b2);

    std::vector<bool> accumulate(const Binary& other, std::function<bool(bool,bool)> accumulator) const;
    CMP compare(const Binary& other) const;
    void emplace(const std::string& s);

    std::vector<bool> m_bits;
};