/* school.hpp - Educational services */
#ifndef _SCHOOL_
#define _SCHOOL_

#include <vector>
#include <string>
#include <map>

#include "interface.hpp"

enum class UniversityClassType {
    Science,
    Computing,
    Education,
    General,
    Medicine,
    Veterinary
};

class UniversityClass {
    public:
    std::string class_name;
    UniversityClassType type;
};

class LifeEducation {
    public:
    EducationLevel current_schooling;
    EducationLevel level;
    std::vector<UniversityClass> university_classes_taken;
    std::map<EducationLevel, int> grades;
    std::map<UniversityClass, int> university_grades;

    std::string GenerateCV();
    int GetQualificationMetric();
};

#endif /* _SCHOOL_ */
