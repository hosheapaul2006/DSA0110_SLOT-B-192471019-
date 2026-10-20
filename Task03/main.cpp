#include "../include/metro.hpp"
#include <climits>
#include <iomanip>
#include <iostream>
#include <limits>
#include <type_traits>
template <class T> void sizeRow(const char* name) {
    std::cout << name << " bytes=" << sizeof(T) << " lowest="
              << std::numeric_limits<T>::lowest() << " max=" << std::numeric_limits<T>::max() << '\n';
}
int main() {
    sizeRow<bool>("bool");
    std::cout << "char bytes=" << sizeof(char) << " lowest=" << CHAR_MIN << " max=" << CHAR_MAX << '\n';
    sizeRow<int>("int"); sizeRow<long long>("long long"); sizeRow<unsigned>("unsigned");
    sizeRow<float>("float"); sizeRow<double>("double");
    auto journeys = 3000000LL;
    std::cout << "auto journeys is long long: " << std::boolalpha
              << std::is_same<decltype(journeys),long long>::value << '\n';
    std::cout << "INT_MAX=" << INT_MAX << " LLONG_MAX=" << LLONG_MAX << '\n';
    std::cout << "Network maximum baseline total paise=" << journeys * 3600LL << '\n';
    std::cout << "First 3600-paise addition exceeding int: " << INT_MAX / 3600 + 1 << '\n';
    const long long counts[] = {20000,500000,3000000};
    std::cout << std::fixed << std::setprecision(12);
    for (const auto n : counts) {
        double actual = 0.0, whole = 0.0;
        long long exact = 0;
        for (long long i=0;i<n;++i) { actual += 10.10; exact += 1010; whole += 36.0; }
        const long double reference = static_cast<long double>(exact) / 100.0L;
        std::cout << "n=" << n << " diagnostic Rs10.10 total=" << actual
                  << " exact paise=" << exact << " error rupees="
                  << static_cast<long double>(actual)-reference
                  << " whole-Rs36 error=" << whole-static_cast<double>(n*36) << '\n';
    }
    const auto mixed = 10 + 0.5 + true;
    std::cout << "int + double + bool = " << mixed << '\n';
    return journeys * 3600LL > INT_MAX && mixed == 11.5 ? 0 : 1;
}
