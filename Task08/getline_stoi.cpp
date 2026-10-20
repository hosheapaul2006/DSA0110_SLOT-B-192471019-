#include <iostream>
#include <stdexcept>
#include <string>
int main() {
    std::string line;
    while (std::getline(std::cin,line)) {
        try {
            std::size_t used=0;
            const int hour=std::stoi(line,&used);
            if (line.find_first_not_of(" \t\r",used)!=std::string::npos)
                throw std::invalid_argument("Trailing characters");
            if (hour<0 || hour>23) throw std::out_of_range("Hour range");
            std::cout << "Accepted hour " << hour << '\n';
            return 0;
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid integer line\n";
        } catch (const std::out_of_range&) {
            std::cout << "Integer/hour out of range\n";
        }
    }
    std::cout << "End of input\n";
    return 2;
}
