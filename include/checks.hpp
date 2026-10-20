#ifndef CHECKS_HPP
#define CHECKS_HPP
#include <iostream>
#include <sstream>
#include <string>
class Checks {
    int total_=0, failed_=0;
public:
    Checks() { std::cout << "CASE|EXPECTED|ACTUAL|RESULT\n"; }
    template<class T> void equal(const std::string& label, const T& expected, const T& actual) {
        ++total_;
        const bool pass=expected==actual;
        if (!pass) ++failed_;
        std::cout << label << '|' << expected << '|' << actual << '|' << (pass?"PASS":"FAIL") << '\n';
    }
    int finish() const {
        std::cout << "SUMMARY " << total_-failed_ << '/' << total_ << " passed\n";
        return failed_==0 ? 0 : 1;
    }
};
#endif
