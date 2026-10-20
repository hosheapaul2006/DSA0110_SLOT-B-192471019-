#include "../include/metro.hpp"
#include <iostream>
int main() {
    metro::SmartCard card{10000};
    for (const long long fare : {1000LL,3600LL,2000LL,-1LL}) {
        const bool accepted=card.deduct(fare);
        std::cout << "Fare paise " << fare << " accepted=" << std::boolalpha << accepted
                  << " balance Rs " << metro::money(card.balance()) << '\n';
    }
    std::cout << "Balance storage bytes: " << sizeof(card) << '\n';
    const auto before=card.balance();
    const bool rechargeRejected=!card.recharge(-100);
    return before==5400 && rechargeRejected && card.balance()==before ? 0 : 1;
}
