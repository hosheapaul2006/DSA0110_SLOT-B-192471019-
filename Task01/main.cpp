#include "../include/metro.hpp"
#include <iostream>
int main() {
    const metro::Passenger passenger{"D Hoshea Paul", metro::Category::Regular};
    const metro::Journey journey{metro::Zone::Outer, 8, 0, passenger.category};
    const metro::FareTable table;
    metro::SmartCard card{10000};
    const auto fare = table.quote(journey.zone, journey.hour, journey.category, journey.minute);
    std::cout << "Passenger: " << passenger.name << '\n';
    std::cout << "Fare Rs " << metro::money(fare) << '\n';
    std::cout << "Journey allowed: " << std::boolalpha << card.deduct(fare) << '\n';
    std::cout << "Balance Rs " << metro::money(card.balance()) << '\n';
    const auto before = card.balance();
    const bool denied = !card.deduct(3000);
    std::cout << "Unaffordable trip denied: " << denied << '\n';
    std::cout << "Balance unchanged: " << (before == card.balance()) << '\n';
    return denied && before == card.balance() && card.balance() >= 5000 ? 0 : 1;
}
