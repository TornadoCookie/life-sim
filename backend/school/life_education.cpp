#include "school.hpp"

std::string LifeEducation::GenerateCV()
{
    std::string ret;
    ret.append("7 Secondary Diploma Qualification(s) at B grade.");
    return ret;
}

int LifeEducation::GetQualificationMetric()
{
    return (int)level + university_classes_taken.size();
}
