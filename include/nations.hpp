/* nations.hpp - Nationalities */
#ifndef _NATION_
#define _NATION_

#include <vector>
#include <string>

class Nation {
    public:
    std::string name;
    std::string demonym;
    std::string api_name;
    bool shouldHaveTheBefore;
};

class NationGenerator {
    public:
    NationGenerator();
    Nation GetRandomNation();
    std::vector<Nation> GetNationList();

    private:
    std::vector<Nation> loaded_nations;
};

#endif /* _NATION_ */
