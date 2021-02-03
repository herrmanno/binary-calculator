#include <stdexcept>
#include <sstream>
#include <ostream>
#include <algorithm>
#include "./functions.hpp"
#include "./binary.hpp"

Binary::Binary() : m_bits {} {}

Binary::Binary(unsigned long long int l) : m_bits { }
{
    while (l) {
        m_bits.insert(m_bits.begin(), l & 1);
        l = l >> 1;
    }
}

Binary::Binary(const std::vector<bool>& bits) : m_bits { bits }
{
    while (m_bits.size() > 1 && !m_bits.front()) {
        m_bits.erase(m_bits.begin());
    }
}

Binary Binary::operator&(const Binary& rhs) const {
    std::vector<bool> bits = this->accumulate(rhs, std::logical_and<bool>());
    return { bits };
}

Binary& Binary::operator&=(const Binary& rhs) {
    this->m_bits = this->accumulate(rhs, std::logical_and<bool>());
    return *this;
}

Binary Binary::operator|(const Binary& rhs) const {
    std::vector<bool> bits = this->accumulate(rhs, std::logical_or<bool>());
    return { bits };
}

Binary& Binary::operator|=(const Binary& rhs) {
    this->m_bits = this->accumulate(rhs, std::logical_or<bool>());
    return *this;
}

Binary Binary::operator^(const Binary& rhs) const {
    std::vector<bool> bits = this->accumulate(rhs, std::not_equal_to<bool>());
    return { bits };
}

Binary& Binary::operator^=(const Binary& rhs) {
    this->m_bits = this->accumulate(rhs, std::not_equal_to<bool>());
    return *this;
}

bool Binary::operator>(const Binary& rhs) const {
    return this->compare(rhs) == Binary::CMP::GT;
}

bool Binary::operator<(const Binary& rhs) const {
    return this->compare(rhs) == Binary::CMP::LT;
}

bool Binary::operator==(const Binary& rhs) const {
    return this->compare(rhs) == Binary::CMP::EQ;
}

bool Binary::operator!=(const Binary& rhs) const {
    return this->compare(rhs) != Binary::CMP::EQ;
}

std::ostream& operator<<(std::ostream& os, const Binary& bin) {
    os << bin.to_str();
    return os;
}

std::istream& operator>>(std::istream& is, Binary& bin) {
    std::string s;
    is >> s;
    bin.emplace(s);
    return is;
}

Binary Binary::concat(const Binary& other) const {
    std::vector<bool> bits = this->m_bits;
    std::copy(other.m_bits.begin(), other.m_bits.end(), std::back_inserter(bits));
    return { bits };
}

size_t Binary::parity() const {
    return std::count_if(m_bits.begin(), m_bits.end(), identity<bool>);
}

std::string Binary::to_str() const {
    std::stringstream ss;
    for (const bool bit : m_bits) {
        ss << (bit ? "1" : "0");
    }
    return ss.str();
}

unsigned long long int Binary::to_l() const {
    unsigned long long int i = 0;
    for (bool b : m_bits) {
        i = (i << 1) + b;
    }
    return i;
}

void Binary::emplace(const std::string& s) {
    std::vector<bool> bits;
    for (auto &c : s) {
        if (c == '0') {
            bits.push_back(false);
        } else if (c == '1') {
            bits.push_back(true);
        } else {
            throw std::invalid_argument("Binary string representation must only contain '0' or '1'");
        }
    }
    while (bits.size() > 1 && !bits.front()) {
        bits.erase(bits.begin());
    }
    m_bits = bits;
}

std::vector<bool> Binary::accumulate(const Binary& other, std::function<bool(bool,bool)> accumulator) const {
    std::vector<bool> bits1 = this->m_bits;
    std::vector<bool> bits2 = other.m_bits;
    
    size_t l = std::max(bits1.size(), bits2.size());

    while (bits1.size() < l) {
        bits1.insert(bits1.begin(), false);
    }
    while (bits2.size() < l) {
        bits2.insert(bits2.begin(), false);
    }
    
    for (int i = 0; i < l; i++) {
        bits1[i] = accumulator(bits1[i], bits2[i]);
    }

    return bits1;
}

Binary::CMP Binary::compare(const Binary& other) const {
    std::vector<bool> bits1 = this->m_bits;
    std::vector<bool> bits2 = other.m_bits;
    
    size_t l = std::max(bits1.size(), bits2.size());
    
    while (bits1.size() < l) {
        bits1.insert(bits1.begin(), false);
    }
    while (bits2.size() < l) {
        bits2.insert(bits2.begin(), false);
    }
    
    CMP c = CMP::EQ;
    for (int i = 0; i < l; i++) {
        if (bits1[i] > bits2[i]) {
            c = CMP::GT;
        } else if (bits1[i] < bits2[i]) {
            c = CMP::LT;
        }
        
        if (c != CMP::EQ) {
            break;
        }
    }

    return c;
}