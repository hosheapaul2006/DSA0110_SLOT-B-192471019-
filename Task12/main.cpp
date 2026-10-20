#include "../include/metro_extended.hpp"
#include "../include/checks.hpp"
#include <iostream>
#include <string>
std::string describe(const extended::Quote& q) {
    return metro::money(q.fare)+",pass="+(q.passApplied?"1":"0")+",weekend="+(q.weekendApplied?"1":"0")+",peak="+(q.peakApplied?"1":"0");
}
int main() {
    using extended::Zone; using metro::Category;
    Checks check;
    const extended::FareRules rules;
    const extended::MonthlyPass none;
    const extended::MonthlyPass october{2026,10,Zone::Inner,Zone::Middle};
    const extended::MonthlyPass nonAdjacent{2026,10,Zone::Inner,Zone::Outer};
    struct Case { const char* name; extended::Journey journey; const extended::MonthlyPass* pass; long long fare; bool covered,weekend,peak; };
    const Case cases[]={
        {"Airport weekday offpeak",{Zone::Outer,Zone::Airport,{2026,10,20},10,0,Category::Regular},&none,6000,false,false,false},
        {"Airport weekday peak",{Zone::Outer,Zone::Airport,{2026,10,20},8,0,Category::Regular},&none,7200,false,false,true},
        {"Airport Saturday peak replaced",{Zone::Outer,Zone::Airport,{2026,10,24},8,0,Category::Regular},&none,5400,false,true,false},
        {"Airport Sunday offpeak",{Zone::Outer,Zone::Airport,{2026,10,25},10,0,Category::Regular},&none,5400,false,true,false},
        {"Student Airport weekend",{Zone::Outer,Zone::Airport,{2026,10,24},8,0,Category::Student},&none,4860,false,true,false},
        {"Senior Airport peak",{Zone::Outer,Zone::Airport,{2026,10,20},17,0,Category::Senior},&none,5760,false,false,true},
        {"Pass covered weekday",{Zone::Inner,Zone::Middle,{2026,10,20},8,0,Category::Regular},&october,0,true,false,false},
        {"Pass covered weekend",{Zone::Middle,Zone::Inner,{2026,10,24},8,0,Category::Student},&october,0,true,false,false},
        {"Pass last day",{Zone::Inner,Zone::Middle,{2026,10,31},23,59,Category::Regular},&october,0,true,false,false},
        {"Pass first day",{Zone::Inner,Zone::Middle,{2026,10,1},0,0,Category::Regular},&october,0,true,false,false},
        {"Pass next month expires",{Zone::Inner,Zone::Middle,{2026,11,1},8,0,Category::Regular},&october,1800,false,true,false},
        {"Pass wrong year",{Zone::Inner,Zone::Middle,{2027,10,20},8,0,Category::Regular},&october,2400,false,false,true},
        {"Pass uncovered third zone",{Zone::Middle,Zone::Outer,{2026,10,20},8,0,Category::Regular},&october,3600,false,false,true},
        {"Nonadjacent pass crosses Middle",{Zone::Inner,Zone::Outer,{2026,10,20},10,0,Category::Regular},&nonAdjacent,3000,false,false,false},
        {"Same selected zone covered",{Zone::Outer,Zone::Outer,{2026,10,20},10,0,Category::Regular},&nonAdjacent,0,true,false,false},
        {"Baseline Inner retained",{Zone::Inner,Zone::Inner,{2026,10,20},10,0,Category::Regular},&none,1000,false,false,false},
        {"Baseline Middle retained",{Zone::Inner,Zone::Middle,{2026,10,20},10,0,Category::Regular},&none,2000,false,false,false},
        {"Baseline Outer retained",{Zone::Middle,Zone::Outer,{2026,10,20},20,0,Category::Regular},&none,3000,false,false,false}
    };
    for (const auto& c:cases) {
        const auto actual=rules.quote(c.journey,*c.pass);
        const extended::Quote expected{c.fare,c.covered,c.weekend,c.peak};
        check.equal(c.name,describe(expected),describe(actual));
    }
    const extended::Journey covered{Zone::Inner,Zone::Middle,{2026,10,20},8,0,Category::Regular};
    const auto zero=rules.quote(covered,october).fare;
    metro::SmartCard reserve{5000},low{4999},airportCard{12200},insufficient{12199};
    check.equal("Pass at reserve accepted",true,reserve.deduct(zero));
    check.equal("Pass reserve unchanged",5000LL,reserve.balance());
    check.equal("Pass below reserve denied",false,low.deduct(zero));
    check.equal("Airport peak exact reserve",true,airportCard.deduct(7200));
    check.equal("Airport balance exact reserve",5000LL,airportCard.balance());
    check.equal("Airport one paisa short denied",false,insufficient.deduct(7200));
    check.equal("Airport denied balance unchanged",12199LL,insufficient.balance());
    bool unlimited=true;
    for (int i=0;i<1000;++i) if (!reserve.deduct(rules.quote(covered,october).fare)) unlimited=false;
    check.equal("1000 pass trips have no count limit",true,unlimited && reserve.balance()==5000);
    check.equal("2026-10-24 is weekend",true,extended::isWeekend({2026,10,24}));
    check.equal("2026-10-20 is weekday",false,extended::isWeekend({2026,10,20}));
    check.equal("Leap day 2024 valid",true,extended::validDate({2024,2,29}));
    check.equal("Nonleap day 2026 invalid",false,extended::validDate({2026,2,29}));
    check.equal("Century 1900 not leap",false,extended::validDate({1900,2,29}));
    check.equal("Century 2000 leap",true,extended::validDate({2000,2,29}));
    bool badDate=false,badZone=false,badTime=false,badPass=false,duplicatePass=false;
    auto invalid=covered; invalid.date={2026,2,29};
    try { (void)rules.quote(invalid,october); } catch(const std::invalid_argument&) { badDate=true; }
    invalid=covered; invalid.to=static_cast<Zone>(99);
    try { (void)rules.quote(invalid,october); } catch(const std::invalid_argument&) { badZone=true; }
    invalid=covered; invalid.hour=24;
    try { (void)rules.quote(invalid,october); } catch(const std::invalid_argument&) { badTime=true; }
    try { extended::MonthlyPass p{2026,13,Zone::Inner,Zone::Middle}; } catch(const std::invalid_argument&) { badPass=true; }
    try { extended::MonthlyPass p{2026,10,Zone::Inner,Zone::Inner}; } catch(const std::invalid_argument&) { duplicatePass=true; }
    check.equal("Reject invalid date even with pass",true,badDate);
    check.equal("Reject unknown fourth-zone enum",true,badZone);
    check.equal("Reject invalid time even with pass",true,badTime);
    check.equal("Reject invalid pass month",true,badPass);
    check.equal("Reject duplicate pass zones",true,duplicatePass);
    return check.finish();
}
