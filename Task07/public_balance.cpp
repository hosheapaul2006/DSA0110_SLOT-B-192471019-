#include "../include/metro.hpp"
#include <iostream>
struct PublicCard { long long balance; };
int main() {
    PublicCard card{10000};
    card.balance=-1; // Legal access, broken application invariant.
    std::cout << "Public member allows balance=" << card.balance << " paise\n";
    std::cout << "Public/private object bytes: " << sizeof(card) << '/' << sizeof(metro::SmartCard) << '\n';
    return card.balance<metro::MIN_BALANCE_PAISE ? 0 : 1;
}
