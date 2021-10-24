#include "ExtraFunctions.hpp"

/*
Source code of ExtraFuctions.
*/

std::vector<std::string> ex::Extra::Split(std::string String, char Seperator){
    std::stringstream ss(String);
    std::string line;
    std::vector<std::string> SeparatedSentance;

    // Splits lines and pushes them in vector
    while(getline(ss, line, Seperator)) {
        SeparatedSentance.emplace_back(line);
    }
    return SeparatedSentance;
}

std::vector<std::string> ex::Extra::MultiSplit(std::string String, std::string Separator){
    size_t pos_start = 0, pos_end, delim_len = Separator.length();
    std::string token;
    std::vector<std::string> SeparatedSentance;

    // Splits lines where a multi-char separator is needed
    while ((pos_end = String.find (Separator, pos_start)) != std::string::npos) {
        token = String.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        SeparatedSentance.emplace_back(token);
    }

    SeparatedSentance.emplace_back(String.substr (pos_start));
    return SeparatedSentance;
}

std::string ex::Extra::RemoveSpace(std::string StringWithSpace){
    // The name suggests
    auto space = StringWithSpace.find_first_not_of(' ');
    auto StringClean = StringWithSpace.substr(space != std::string::npos ? space : 0);
    return StringClean;
}

/*
I took these from stackoverflow(https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string)
*/

std::string ex::Extra::trim(const std::string& str, const std::string& whitespace = " \t"){
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


std::string ex::Extra::reduce(const std::string& str, const std::string& fill = " ", const std::string& whitespace = " \t"){
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}
