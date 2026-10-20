#include <iostream>
int main() {
    const int a{7}, b{2};
    const double c{static_cast<double>(a) / static_cast<double>(b)};
    const double d{static_cast<double>(a) / static_cast<double>(b)};
    std::cout << c << ' ' << d << '\n';
    const char ch{static_cast<char>(static_cast<int>('A') + 2)};
    std::cout << ch << ' ' << static_cast<int>(ch) + 1 << '\n';
    const bool flag{static_cast<bool>(5)};
    std::cout << static_cast<int>(flag) << ' ' << std::boolalpha << flag << '\n';
    const unsigned u{1U}; const int s{-1};
    const bool less{s < 0 || static_cast<unsigned>(s) < u};
    std::cout << (less ? "s < u" : "s >= u") << '\n';
    return c == 3.5 && less ? 0 : 1;
}
