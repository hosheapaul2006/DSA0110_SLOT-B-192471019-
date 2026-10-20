#include "../include/metro.hpp"
#include <fstream>
#include <iostream>
#include <vector>
class ConfigFareTable {
    std::vector<metro::Money> fares_;
public:
    explicit ConfigFareTable(const char* path) {
        std::ifstream in{path};
        int count=0;
        if (!(in>>count) || count<1 || count>32) throw std::invalid_argument("Invalid zone count or unreadable file");
        fares_.reserve(static_cast<std::size_t>(count));
        for (int i=0;i<count;++i) {
            metro::Money fare=0;
            if (!(in>>fare) || fare<=0 || fare>1000000) throw std::invalid_argument("Invalid/missing fare");
            fares_.push_back(fare);
        }
        std::string extra;
        if (in>>extra) throw std::invalid_argument("Unexpected trailing configuration data");
    }
    std::size_t size() const { return fares_.size(); }
    metro::Money quote(std::size_t index,int hour) const {
        if (index>=fares_.size()) throw std::invalid_argument("Unknown configured zone");
        return metro::roundHalfUp(fares_[index]*(metro::isPeak(hour)?120:100),100);
    }
};
int main(int argc,char** argv) {
    try {
        const ConfigFareTable configured{argc>1 ? argv[1] : "fares.txt"};
        if (configured.size()<3) throw std::invalid_argument("Demo requires at least three zones");
        const auto fare=configured.quote(2,8);
        metro::SmartCard card{10000};
        const bool allowed=card.deduct(fare);
        std::cout << "Configured zones: " << configured.size() << '\n';
        std::cout << "Outer peak fare Rs " << metro::money(fare) << '\n';
        std::cout << "Allowed=" << std::boolalpha << allowed << " balance Rs " << metro::money(card.balance()) << '\n';
    } catch(const std::exception& error) {
        std::cout << "Configuration rejected: " << error.what() << '\n';
        return 2;
    }
    return 0;
}
