#include "../include/metro.hpp"
#include <iostream>
#include <stdexcept>
int main() {
    bool peak[24] = {};
    for (int hour=0;hour<24;++hour) peak[hour] = (hour>=8 && hour<10) || (hour>=17 && hour<20);
    int matches=0;
    for (int hour=0;hour<24;++hour) if (peak[hour] == metro::isPeak(hour)) ++matches;
    std::cout << "Lookup vs expression matches: " << matches << "/24\n";
    std::cout << "Lookup storage bytes: " << sizeof(peak) << '\n';
    for (const int hour : {8,10,17,20}) std::cout << "Hour " << hour << " peak=" << std::boolalpha << metro::isPeak(hour) << '\n';
    const metro::FareTable table;
    std::cout << "Student Outer peak Rs " << metro::money(table.quote(metro::Zone::Outer,8,metro::Category::Student)) << '\n';
    std::cout << "Senior Outer offpeak Rs " << metro::money(table.quote(metro::Zone::Outer,10,metro::Category::Senior)) << '\n';
    bool invalidRejected=false;
    try { (void)metro::isPeak(24); } catch (const std::invalid_argument&) { invalidRejected=true; }
    std::cout << "Invalid hour rejected: " << invalidRejected << '\n';
    return matches==24 && invalidRejected ? 0 : 1;
}
