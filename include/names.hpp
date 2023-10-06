/* names.hpp - Name generation services */
#ifndef _NAMES_
#define _NAMES_

#include "nations.hpp"
#include <map>

enum class Gender {
    Male,
    Female
};

class NameGenerator {
    public:
    NameGenerator();
    ~NameGenerator();
    std::string GetRandomFirstName(Nation nation, Gender gender);
    std::string GetRandomLastName(Nation nation, Gender gender);

    private:
    std::map<Gender, std::map<std::string, std::vector<std::string>>> name_lists;
};

#endif /* _NAMES_ */
