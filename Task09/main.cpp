#include "../include/metro.hpp"
#include "../include/checks.hpp"
#include <climits>
#include <iostream>
#include <sstream>
#include <string>
std::string outcome(long long fare,bool allowed,long long balance) {
    return metro::money(fare)+","+(allowed?"allow":"deny")+","+metro::money(balance);
}
int main() {
    using metro::Category; using metro::Zone;
    Checks check;
    const metro::FareTable table;
    struct Case { const char* label; Zone zone; int hour,minute; Category category; long long balance,fare; bool allowed; long long after; };
    const Case cases[]={
        {"Inner offpeak",Zone::Inner,11,0,Category::Regular,10000,1000,true,9000},
        {"Middle offpeak",Zone::Middle,11,0,Category::Regular,10000,2000,true,8000},
        {"Outer offpeak",Zone::Outer,11,0,Category::Regular,10000,3000,true,7000},
        {"Inner peak",Zone::Inner,9,0,Category::Regular,10000,1200,true,8800},
        {"Middle peak",Zone::Middle,9,0,Category::Regular,10000,2400,true,7600},
        {"Outer peak",Zone::Outer,9,0,Category::Regular,10000,3600,true,6400},
        {"07:59",Zone::Inner,7,59,Category::Regular,10000,1000,true,9000},
        {"08:00",Zone::Inner,8,0,Category::Regular,10000,1200,true,8800},
        {"09:59",Zone::Inner,9,59,Category::Regular,10000,1200,true,8800},
        {"10:00",Zone::Inner,10,0,Category::Regular,10000,1000,true,9000},
        {"16:59",Zone::Inner,16,59,Category::Regular,10000,1000,true,9000},
        {"17:00",Zone::Inner,17,0,Category::Regular,10000,1200,true,8800},
        {"19:59",Zone::Inner,19,59,Category::Regular,10000,1200,true,8800},
        {"20:00",Zone::Inner,20,0,Category::Regular,10000,1000,true,9000},
        {"Exactly Rs50 remains",Zone::Inner,10,0,Category::Regular,6000,1000,true,5000},
        {"One paisa below reserve",Zone::Inner,10,0,Category::Regular,5999,1000,false,5999},
        {"Student Inner peak",Zone::Inner,8,0,Category::Student,10000,1080,true,8920},
        {"Student Middle offpeak",Zone::Middle,10,0,Category::Student,10000,1800,true,8200},
        {"Senior Outer peak",Zone::Outer,17,0,Category::Senior,10000,2880,true,7120},
        {"Senior Outer offpeak",Zone::Outer,20,0,Category::Senior,10000,2400,true,7600}
    };
    for (const auto& c:cases) {
        metro::SmartCard card{c.balance};
        const auto f=table.quote(c.zone,c.hour,c.category,c.minute);
        const bool allowed=card.deduct(f);
        check.equal(c.label,outcome(c.fare,c.allowed,c.after),outcome(f,allowed,card.balance()));
    }
    for (const int invalidZone : {-1,3,99}) {
        bool rejected=false;
        try { (void)table.quote(static_cast<Zone>(invalidZone),10); } catch(const std::invalid_argument&) { rejected=true; }
        check.equal("Reject zone "+std::to_string(invalidZone),true,rejected);
    }
    for (const int hour : {-1,24}) {
        bool rejected=false;
        try { (void)table.quote(Zone::Inner,hour); } catch(const std::invalid_argument&) { rejected=true; }
        check.equal("Reject hour "+std::to_string(hour),true,rejected);
    }
    bool badMinute=false,badCategory=false,badInitial=false;
    try { (void)table.quote(Zone::Inner,8,Category::Regular,60); } catch(const std::invalid_argument&) { badMinute=true; }
    try { (void)table.quote(Zone::Inner,8,static_cast<Category>(99)); } catch(const std::invalid_argument&) { badCategory=true; }
    try { metro::SmartCard invalid{-1}; } catch(const std::invalid_argument&) { badInitial=true; }
    check.equal("Reject minute 60",true,badMinute);
    check.equal("Reject category 99",true,badCategory);
    check.equal("Reject negative initial balance",true,badInitial);
    metro::SmartCard card{10000};
    check.equal("Reject negative recharge",false,card.recharge(-1));
    check.equal("Reject zero recharge",false,card.recharge(0));
    check.equal("Rejected recharges leave balance",10000LL,card.balance());
    check.equal("Accept positive recharge",true,card.recharge(1));
    check.equal("Positive recharge new balance",10001LL,card.balance());
    metro::SmartCard full{metro::MAX_BALANCE_PAISE};
    check.equal("Reject recharge above application cap",false,full.recharge(1));
    check.equal("Reject huge recharge",false,card.recharge(LLONG_MAX));
    check.equal("Reject negative fare",false,card.deduct(-1));
    check.equal("Reject huge fare safely",false,card.deduct(LLONG_MAX));
    std::istringstream letters{"letters\n8\n"}; std::ostringstream messages; long long hour=0;
    const bool recovered=metro::readInteger(letters,messages,"hour: ",0,23,hour);
    check.equal("Letters then valid hour recovery",true,recovered && hour==8 && messages.str().find("Invalid number")!=std::string::npos);
    check.equal("Half-paisa rounds up",101LL,metro::roundHalfUp(10050,100));
    check.equal("Below half-paisa rounds down",100LL,metro::roundHalfUp(10049,100));
    const metro::Journey journeys[]={
        {Zone::Inner,11,0,Category::Regular},{Zone::Middle,8,0,Category::Regular},
        {Zone::Outer,10,0,Category::Regular},{Zone::Inner,17,0,Category::Student},
        {Zone::Outer,19,0,Category::Senior}
    };
    const long long expected[]={1000,2400,3000,1080,2880};
    metro::SmartCard dayCard{20000}; long long total=0; int row=0;
    for (const auto& j:journeys) {
        const auto f=table.quote(j.zone,j.hour,j.category,j.minute);
        check.equal("Five-journey fare "+std::to_string(row+1),expected[row],f);
        check.equal("Five-journey payment "+std::to_string(row+1),true,dayCard.deduct(f));
        total+=f; ++row;
    }
    check.equal("Five-journey hand total paise",10360LL,total);
    check.equal("Five-journey remaining paise",9640LL,dayCard.balance());
    return check.finish();
}
