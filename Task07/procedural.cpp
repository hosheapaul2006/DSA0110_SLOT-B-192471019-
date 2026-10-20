#include "../include/metro.hpp"
#include <iostream>
long long balance=10000;
bool canTravel(long long fare) {
    return fare>=0 && balance>=metro::MIN_BALANCE_PAISE && fare<=balance-metro::MIN_BALANCE_PAISE;
}
bool deduct(long long fare) {
    if (!canTravel(fare)) return false;
    balance-=fare; return true;
}
int main() {
    for (const long long fare : {1000LL,3600LL,2000LL,-1LL}) {
        const bool accepted=deduct(fare);
        std::cout << "Fare paise " << fare << " accepted=" << std::boolalpha << accepted
                  << " balance Rs " << metro::money(balance) << '\n';
    }
    std::cout << "Balance storage bytes: " << sizeof(balance) << '\n';
    return balance==5400 ? 0 : 1;
}
