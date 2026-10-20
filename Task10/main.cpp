#include "../include/metro.hpp"
#include <chrono>
#include <climits>
#include <iomanip>
#include <iostream>
int main() {
    const metro::FareTable table;
    const long long fares[]={table.quote(metro::Zone::Inner,8),table.quote(metro::Zone::Middle,10),
        table.quote(metro::Zone::Outer,17),table.quote(metro::Zone::Middle,10,metro::Category::Student),
        table.quote(metro::Zone::Outer,17,metro::Category::Senior)};
    const long long counts[]={20000,500000,3000000};
    bool passed=true;
    std::cout << "Accumulator bytes: int=" << sizeof(int) << " long long=" << sizeof(long long) << " double=" << sizeof(double) << '\n';
    std::cout << "Cycle fares paise:";
    for (const auto f:fares) std::cout << ' ' << f;
    std::cout << "\njourneys,exact_paise,double_rupees,error_rupees,int_us,double_us,int_safe,first_int_overflow\n";
    for (const auto n:counts) {
        volatile long long exact=0;
        volatile double actual=0.0;
        auto start=std::chrono::steady_clock::now();
        for (long long i=0;i<n;++i) exact+=fares[i%5];
        auto middle=std::chrono::steady_clock::now();
        for (long long i=0;i<n;++i) actual+=static_cast<double>(fares[i%5])/100.0;
        auto end=std::chrono::steady_clock::now();
        const auto integerUs=std::chrono::duration_cast<std::chrono::microseconds>(middle-start).count();
        const auto doubleUs=std::chrono::duration_cast<std::chrono::microseconds>(end-middle).count();
        int checkedTotal=0; long long firstOverflow=0;
        for (long long i=0;i<n;++i) {
            const int f=static_cast<int>(fares[i%5]);
            if (checkedTotal>INT_MAX-f) { firstOverflow=i+1; break; }
            checkedTotal+=f;
        }
        const long double difference=static_cast<long double>(actual)-static_cast<long double>(exact)/100.0L;
        std::cout << n << ',' << exact << ',' << std::fixed << std::setprecision(12) << actual << ',' << difference
                  << ',' << integerUs << ',' << doubleUs << ',' << (firstOverflow==0 ? "yes":"no") << ',' << firstOverflow << '\n';
        if (exact!=(n/5)*11480 || (n==3000000 && firstOverflow!=935316)) passed=false;
    }
    std::cout << "Microbenchmark includes volatile reads/writes and loop overhead; not a gate throughput guarantee.\n";
    std::cout << "Hand total / overflow checks: " << (passed ? "PASS":"FAIL") << '\n';
    return passed ? 0 : 1;
}
