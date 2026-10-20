#include "../include/metro.hpp"
#include <iostream>
#define DEMO_PEAK_PERCENT 20
enum LegacyZone { LegacyInner, LegacyMiddle, LegacyOuter };
int main() {
    const int runtimeConstant = metro::PEAK_PERCENT;
    constexpr int compileTimeConstant = metro::PEAK_PERCENT;
    static_assert(compileTimeConstant == 20, "Peak rate must match assignment");
    const int legacyIndex = LegacyInner; // Unscoped enum converts implicitly.
    const auto safeZone = metro::Zone::Inner;
    const metro::FareTable table;
    std::cout << "Fares (paise):";
    for (const auto value : metro::BASE_PAISE) std::cout << ' ' << value;
    std::cout << "\nMacro/const/constexpr: " << DEMO_PEAK_PERCENT << ' '
              << runtimeConstant << ' ' << compileTimeConstant << '\n';
    std::cout << "Legacy enum index: " << legacyIndex << '\n';
    std::cout << "Scoped zone index (explicit): " << static_cast<int>(safeZone) << '\n';
    std::cout << "Minimum Rs " << metro::money(metro::MIN_BALANCE_PAISE) << '\n';
    std::cout << "Student Inner peak Rs " << metro::money(table.quote(safeZone,8,metro::Category::Student)) << '\n';
    return table.quote(safeZone,8,metro::Category::Student) == 1080 ? 0 : 1;
}
