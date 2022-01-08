#ifndef RENDOR_ID_COMPARISON_HPP
#define RENDOR_ID_COMPARISON_HPP

#include <string>

#include "boost/lexical_cast.hpp"

typedef std::unique_ptr<Type> TypeObject;
inline bool CompareRendorIDs(TypeObject& ID1, TypeObject& ID2)
{
    size_t ID1UnderScore = ID1->m_ID.find_first_of("_");
    size_t ID2UnderScore = ID2->m_ID.find_first_of("_");

    std::string IntID1String{ID1->m_ID.begin() + ID1UnderScore, ID1->m_ID.end()};
    std::string IntID2String{ID2->m_ID.begin() + ID2UnderScore, ID2->m_ID.end()};

    int64_t IntOfID1 = 0;
    int64_t IntOfID2 = 0;

    IntOfID1 = boost::lexical_cast<int64_t>(IntID1String);
    IntOfID2 = boost::lexical_cast<int64_t>(IntID2String);

    return IntOfID1 > IntOfID2;
}

#endif // RENDOR_ID_COMPARISON_HPP