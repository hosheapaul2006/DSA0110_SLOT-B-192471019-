#include "../include/metro.hpp"
#include <iostream>
metro::Money switchBase(metro::Zone zone) {
    switch (zone) {
        case metro::Zone::Inner: return metro::BASE_PAISE[0];
        case metro::Zone::Middle: return metro::BASE_PAISE[1];
        case metro::Zone::Outer: return metro::BASE_PAISE[2];
    }
    throw std::invalid_argument("Unknown zone");
}
int main() {
    const metro::FareTable table;
    int passed=0;
    for (const int hour : {10,20}) {
        const auto f=table.quote(metro::Zone::Outer,hour);
        metro::SmartCard card{8000};
        const bool allowed=card.deduct(f);
        std::cout << hour << ":00 peak=" << std::boolalpha << metro::isPeak(hour)
                  << " fare Rs " << metro::money(f) << " allowed=" << allowed
                  << " balance Rs " << metro::money(card.balance()) << '\n';
        if (f==3000 && allowed && card.balance()==5000) ++passed;
    }
    for (int i=0;i<3;++i) {
        const auto z=static_cast<metro::Zone>(i);
        if (switchBase(z)==table.base(z)) ++passed;
    }
    std::cout << "Switch/array and time checks: " << passed << "/5\n";
    return passed==5 ? 0 : 1;
}
