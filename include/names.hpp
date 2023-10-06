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
    bool can_use_cjk;
};

#endif /* _NAMES_ */
