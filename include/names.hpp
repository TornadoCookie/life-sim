/* names.hpp - Name generation services */
#ifndef _NAMES_
#define _NAMES_

#include "nations.hpp"
#include <map>
#include <thread>

enum class Gender {
    Male,
    Female
};

struct FullName {
    public:
    std::string name;
    Nation nation;
    Gender gender;
};

class NameGenerator {
    public:
    NameGenerator(Nation home_nation);
    NameGenerator();
    ~NameGenerator();
    std::string GetRandomFirstName(Nation nation, Gender gender);
    std::string GetRandomLastName(Nation nation, Gender gender);
    void SetCanUseCJK(bool can);
    void SetHomeNation(Nation nation);

    private:
    std::string get_random_full_name(Nation nation, Gender gender);
    std::vector<FullName> loaded_full_names;
    std::thread name_generation_thread;
    bool running;
    bool can_use_cjk;
    Nation home_nation;
};

extern NameGenerator *name_generator;

#endif /* _NAMES_ */
