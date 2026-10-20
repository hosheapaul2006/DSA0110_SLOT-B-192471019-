#ifndef METRO_HPP
#define METRO_HPP
#include <array>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace metro {
using Money = long long;
enum class Zone { Inner, Middle, Outer };
enum class Category { Regular, Student, Senior };
constexpr std::array<Money, 3> BASE_PAISE{{1000, 2000, 3000}};
constexpr int PEAK_PERCENT = 20;
constexpr int STUDENT_PERCENT = 10; // Assumption: the assignment does not set rates.
constexpr int SENIOR_PERCENT = 20;
constexpr Money MIN_BALANCE_PAISE = 5000;
constexpr Money MAX_BALANCE_PAISE = 1000000000000LL; // Explicit application limit.

inline bool validZone(Zone z) {
    const int index = static_cast<int>(z);
    return index >= 0 && index < static_cast<int>(BASE_PAISE.size());
}
inline bool isPeak(int hour, int minute = 0) {
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        throw std::invalid_argument("Invalid time");
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}
inline int concession(Category category) {
    switch (category) {
        case Category::Regular: return 0;
        case Category::Student: return STUDENT_PERCENT;
        case Category::Senior: return SENIOR_PERCENT;
    }
    throw std::invalid_argument("Unknown passenger category");
}
inline Money roundHalfUp(Money numerator, Money denominator) {
    if (numerator < 0 || denominator <= 0)
        throw std::invalid_argument("Invalid nonnegative ratio");
    const Money quotient = numerator / denominator;
    const Money remainder = numerator % denominator;
    return quotient + (remainder >= denominator / 2 + denominator % 2 ? 1 : 0);
}
class FareTable {
public:
    Money base(Zone zone) const {
        if (!validZone(zone)) throw std::invalid_argument("Unknown zone");
        return BASE_PAISE[static_cast<std::size_t>(zone)];
    }
    Money quote(Zone zone, int hour, Category category = Category::Regular, int minute = 0) const {
        const int multiplier = isPeak(hour, minute) ? 100 + PEAK_PERCENT : 100;
        // Multiply bounded fares and round once, after all percentages.
        return roundHalfUp(base(zone) * multiplier * (100 - concession(category)), 10000);
    }
};
struct Passenger { std::string name; Category category; };
struct Journey { Zone zone; int hour; int minute; Category category; };
class SmartCard {
    Money balance_;
public:
    explicit SmartCard(Money initial) : balance_(initial) {
        if (initial < 0 || initial > MAX_BALANCE_PAISE)
            throw std::invalid_argument("Invalid initial balance");
    }
    Money balance() const { return balance_; }
    bool canTravel(Money fare) const {
        return fare >= 0 && balance_ >= MIN_BALANCE_PAISE && fare <= balance_ - MIN_BALANCE_PAISE;
    }
    bool deduct(Money fare) {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }
    bool recharge(Money amount) {
        if (amount <= 0 || amount > MAX_BALANCE_PAISE - balance_) return false;
        balance_ += amount;
        return true;
    }
};
inline std::string money(Money amount) {
    if (amount < 0) throw std::invalid_argument("Cannot print negative money");
    std::ostringstream out;
    out << amount / 100 << '.' << std::setw(2) << std::setfill('0') << amount % 100;
    return out.str();
}
inline bool readInteger(std::istream& in, std::ostream& out, const std::string& prompt,
                        long long minimum, long long maximum, long long& value) {
    for (;;) {
        out << prompt;
        in >> value;
        if (in.fail()) {
            if (in.eof()) { out << "End of input\n"; return false; }
            out << "Invalid number; try again\n";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::string rest;
        std::getline(in, rest);
        if (rest.find_first_not_of(" \t\r") != std::string::npos) {
            out << "Unexpected trailing characters; try again\n";
            continue;
        }
        if (value < minimum || value > maximum) {
            out << "Out of range; try again\n";
            continue;
        }
        return true;
    }
}
}
#endif
