#include <cassert>
#include <vector>
#include <sstream>
#include <iostream>
#include "../src/binary.hpp"

int main() {
    assert(Binary().to_l() == 0);
    assert(Binary().to_str() == "");
    
    assert(Binary(0).to_l() == 0);
    assert(Binary(0).to_str() == "0");
    assert(Binary(1).to_l() == 1);
    assert(Binary(1).to_str() == "1");
    assert(Binary(666).to_l() == 666);
    assert(Binary(666).to_str() == "1010011010");
    
    assert(Binary(std::vector<bool> {false}).to_l() == 0);
    assert(Binary(std::vector<bool> {true}).to_l() == 1);
    assert(Binary(std::vector<bool> {true,false,true}).to_l() == 5);
    
    { Binary b; std::stringstream("0") >> b; assert(b.to_l() == 0); }
    { Binary b; std::stringstream("00") >> b; assert(b.to_l() == 0); }
    { Binary b; std::stringstream("1") >> b; assert(b.to_l() == 1); }
    { Binary b; std::stringstream("101") >> b; assert(b.to_l() == 5); }

    { Binary b(0); std::stringstream ss; ss << b; assert(ss.str() == "0"); }
    { Binary b(1); std::stringstream ss; ss << b; assert(ss.str() == "1"); }
    { Binary b(5); std::stringstream ss; ss << b; assert(ss.str() == "101"); }

    assert((Binary(5) & Binary(7)) == Binary(5));
    assert((Binary(5) & Binary(0)) == Binary(0));
    assert((Binary(4) & Binary(1)) == Binary(0));

    assert((Binary(5) | Binary(7)) == Binary(7));
    assert((Binary(5) | Binary(0)) == Binary(5));
    assert((Binary(4) | Binary(1)) == Binary(5));

    assert((Binary(5) ^ Binary(7)) == Binary(2));
    assert((Binary(5) ^ Binary(0)) == Binary(5));
    assert((Binary(4) ^ Binary(1)) == Binary(5));

    assert((Binary(1) / Binary(1)) == Binary(1));
    assert((Binary(5) / Binary(7)) == Binary(0));
    assert((Binary(5) / Binary(1)) == Binary(5));
    assert((Binary(4) / Binary(2)) == Binary(2));
    assert((Binary(5) / Binary(2)) == Binary(2));

    assert(Binary(1).div(Binary(1)) == std::make_tuple(Binary(1), Binary(0)));
    assert(Binary(5).div(Binary(7)) == std::make_tuple(Binary(0), Binary(7)));
    assert(Binary(5).div(Binary(1)) == std::make_tuple(Binary(5), Binary(0)));
    assert(Binary(4).div(Binary(2)) == std::make_tuple(Binary(2), Binary(0)));
    assert(Binary(5).div(Binary(2)) == std::make_tuple(Binary(2), Binary(1)));

    assert(Binary(0).concat(Binary(0)) == Binary(0));
    assert(Binary(1).concat(Binary(0)) == Binary(2));
    assert(Binary(5).concat(Binary(5)) == Binary(45));

    assert(Binary(0).parity() == 0);
    assert(Binary(1).parity() == 1);
    assert(Binary(5).parity() == 2);

    assert(Binary(0) < Binary(1));
    assert(Binary(1) < Binary(2));
    assert(Binary(5) < Binary(6));

    assert(Binary(1) > Binary(0));
    assert(Binary(2) > Binary(1));
    assert(Binary(6) > Binary(5));

    assert(Binary(0) == Binary(0));
    assert(Binary(1) == Binary(1));
    assert(Binary(100) == Binary(100));

    assert(Binary(0) != Binary(1));
    assert(Binary(1) != Binary(0));
    assert(Binary(100) != Binary(101));

    return 0;
}
