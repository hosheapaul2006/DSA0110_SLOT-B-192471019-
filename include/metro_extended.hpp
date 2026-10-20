#ifndef METRO_EXTENDED_HPP
#define METRO_EXTENDED_HPP
#include "metro.hpp"
#include <algorithm>
#include <array>
namespace extended {
using metro::Money;
enum class Zone { Inner, Middle, Outer, Airport };
constexpr Money AIRPORT_PAISE=6000;
constexpr int WEEKEND_PERCENT=10;
constexpr std::array<Money,4> FARES{{metro::BASE_PAISE[0],metro::BASE_PAISE[1],metro::BASE_PAISE[2],AIRPORT_PAISE}};
struct Date { int year,month,day; };
inline bool validDate(Date d) {
    if (d.year<1 || d.year>9999 || d.month<1 || d.month>12) return false;
    const int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
    const bool leap=d.year%4==0 && (d.year%100!=0 || d.year%400==0);
    const int limit=days[d.month-1]+(d.month==2 && leap ? 1:0);
    return d.day>=1 && d.day<=limit;
}
inline bool isWeekend(Date d) {
    if (!validDate(d)) throw std::invalid_argument("Invalid date");
    // Gregorian calendar weekday calculation: 0=Sunday, 6=Saturday.
    const int offsets[]={0,3,2,5,0,3,5,1,4,6,2,4};
    int y=d.year;
    if (d.month<3) --y;
    const int weekday=(y+y/4-y/100+y/400+offsets[d.month-1]+d.day)%7;
    return weekday==0 || weekday==6;
}
inline bool validZone(Zone z) { const int value=static_cast<int>(z); return value>=0 && value<4; }
struct Journey { Zone from,to; Date date; int hour,minute; metro::Category category; };
inline void validate(const Journey& j) {
    if (!validZone(j.from) || !validZone(j.to)) throw std::invalid_argument("Unknown route zone");
    if (!validDate(j.date)) throw std::invalid_argument("Invalid date");
    (void)metro::isPeak(j.hour,j.minute);
    (void)metro::concession(j.category);
}
class MonthlyPass {
    bool active_=false;
    int year_=0,month_=0;
    Zone first_=Zone::Inner,second_=Zone::Middle;
public:
    MonthlyPass() = default;
    MonthlyPass(int year,int month,Zone first,Zone second)
        : active_(true),year_(year),month_(month),first_(first),second_(second) {
        if (!validDate(Date{year,month,1}) || !validZone(first) || !validZone(second) || first==second)
            throw std::invalid_argument("Pass requires a valid month and two distinct zones");
    }
    bool covers(const Journey& j) const {
        validate(j);
        if (!active_ || year_!=j.date.year || month_!=j.date.month) return false;
        const int start=std::min(static_cast<int>(j.from),static_cast<int>(j.to));
        const int finish=std::max(static_cast<int>(j.from),static_cast<int>(j.to));
        for (int z=start;z<=finish;++z)
            if (z!=static_cast<int>(first_) && z!=static_cast<int>(second_)) return false;
        return true;
    }
};
struct Quote { Money fare; bool passApplied,weekendApplied,peakApplied; };
class FareRules {
public:
    Quote quote(const Journey& j,const MonthlyPass& pass) const {
        validate(j);
        if (pass.covers(j)) return Quote{0,true,false,false};
        const bool weekend=isWeekend(j.date);
        const bool peak=!weekend && metro::isPeak(j.hour,j.minute);
        const int adjustment=weekend ? 100-WEEKEND_PERCENT : (peak ? 100+metro::PEAK_PERCENT : 100);
        const auto index=static_cast<std::size_t>(std::max(static_cast<int>(j.from),static_cast<int>(j.to)));
        const Money fare=metro::roundHalfUp(FARES[index]*adjustment*(100-metro::concession(j.category)),10000);
        return Quote{fare,false,weekend,peak};
    }
};
}
#endif
