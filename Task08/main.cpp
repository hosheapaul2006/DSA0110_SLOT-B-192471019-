#include "../include/metro.hpp"
#include <iostream>
int main() {
    using metro::readInteger;
    long long balance=0, zone=0, hour=0, minute=0, category=0;
    if (!readInteger(std::cin,std::cout,"Balance in paise: ",0,metro::MAX_BALANCE_PAISE,balance) ||
        !readInteger(std::cin,std::cout,"Zone (0 Inner, 1 Middle, 2 Outer): ",0,2,zone) ||
        !readInteger(std::cin,std::cout,"Hour (0-23): ",0,23,hour) ||
        !readInteger(std::cin,std::cout,"Minute (0-59): ",0,59,minute) ||
        !readInteger(std::cin,std::cout,"Category (0 Regular, 1 Student, 2 Senior): ",0,2,category)) return 2;
    const metro::FareTable table;
    const auto fare=table.quote(static_cast<metro::Zone>(zone),static_cast<int>(hour),
                               static_cast<metro::Category>(category),static_cast<int>(minute));
    metro::SmartCard card{balance};
    const bool allowed=card.deduct(fare);
    std::cout << "\nJOURNEY REPORT\n" << std::left << std::setw(16) << "Status"
              << std::setw(16) << "Fare (Rs)" << "Balance (Rs)\n";
    std::cout << std::setw(16) << (allowed ? "Allowed" : "Denied") << std::setw(16)
              << metro::money(fare) << metro::money(card.balance()) << '\n';
    std::cout << "Display precision example: " << std::fixed << std::setprecision(2) << 1.0 << '\n';
    return 0;
}
