#include "lexer.hpp"
using namespace Lex;


std::vector<std::pair<Token, std::string>> Lexer::Tokenize(std::string& Code){
    std::stringstream __ss__(Code);
    std::string __LineofCode__;
    std::vector<std::pair<Token, std::string>> __Tokens__;
    unsigned int __LineNumber__ = 0;

    while(std::getline(__ss__, __LineofCode__)){
        ++__LineNumber__;
        // Check for beginning of string
        std::vector<ID> __TempIDList__ {ID::None};
        SpecificID __SpecificTempID__ = SpecificID::None; 
        std::string __TempString__ = "";

        for(size_t i = 0; i < __LineofCode__.size(); ++i){
            ID __TempID__ = __TempIDList__.back();
            // Checks for beginning and end of a string
            if((__LineofCode__[i] == '\"') || (__LineofCode__[i] == '\'') || (__LineofCode__[i] == '`')){
                // beggining of a string
                if((__TempID__ == ID::None) || (__TempID__ == ID::KeywordArgs)){
                    // std::cout << "StringStart " << __TempString__ << std::endl;
                    __TempIDList__.emplace_back(ID::Char);
                    __TempString__ = "";

                    // defines what should end string
                    switch(__LineofCode__[i]){
                        case '\'': 
                            __SpecificTempID__ = SpecificID::CharSingle;
                            break;

                        case '\"':
                            __SpecificTempID__ = SpecificID::CharDouble;
                            break;

                        case '`':
                            __SpecificTempID__ = SpecificID::CharTilda;
                            break;
                    }
                    
                }

                // end of a string
                else if(__TempID__ == ID::Char){
                    // std::cout << __TempString__ << std::endl;

                    if(((__LineofCode__[i] == '\'') && (__SpecificTempID__ == SpecificID::CharSingle)) || 
                    ((__LineofCode__[i] == '\"') && (__SpecificTempID__ == SpecificID::CharDouble)) || 
                    ((__LineofCode__[i] == '`') && (__SpecificTempID__ == SpecificID::CharTilda))){
                        __Tokens__.emplace_back(Token::String, __TempString__);
                        __TempIDList__.pop_back();
                        __TempString__ = "";
                    }
                    else{
                        __TempString__.push_back(__LineofCode__[i]);
                    }
                    
                }

                else{
                    // std::cout << "StringEOL Error " << __TempString__  << " " << static_cast<std::underlying_type<ID>::type>(__TempID__) << std::endl;
                    throw error::RendorException("Invalid Syntax: EOL while scanning string on line " + std::to_string(__LineNumber__));
                }
            }

            else if((__LineofCode__[i] == ' ') && (__TempID__ != ID::Char)){

            }

            // Comments
            else if((__LineofCode__[i] == '/') && (__TempID__ != ID::Char)){
                if(__TempID__ != ID::Comment){
                    // std::cout << "Encounted Comment" << std::endl;
                    __TempIDList__.emplace_back(ID::Comment);
                    __TempString__ = "";
                }
                
                else if(__TempID__ == ID::Comment){
                    // std::cout << "Comment " << __TempString__ << std::endl; 
                    break;
                }
            }

            // Checks for operators
            else if((std::find(std::begin(Operators), std::end(Operators), __TempString__) != std::end(Operators)) && (__TempID__ == ID::KeywordArgs)){
                __Tokens__.emplace_back(Token::Operator, __TempString__);
                __TempString__ = "";

                // to account for chars not being added when a operator is found
                __TempString__.push_back(__LineofCode__[i]);
            }

            //Checks for parens
            else if(__LineofCode__[i] == '('){
                // Sometimes the ( is part of a keyword
                if((std::find(std::begin(Keywords), std::end(Keywords), __TempString__) != std::end(Keywords)) && (__TempID__ == ID::None)){
                    __Tokens__.emplace_back(Token::Keyword, __TempString__);
                    __TempIDList__.emplace_back(ID::KeywordArgs);
                    __TempString__ = "";
                }
                __Tokens__.emplace_back(Token::Paren, "PAREN");
            }

            else if(__LineofCode__[i] == ')'){
                __Tokens__.emplace_back(Token::Paren, "PAREN");
            }

            else if((__LineofCode__[i] == '{') || (__LineofCode__[i] == '}')){
                switch(__LineofCode__[i]){
                    case '{':
                        switch(__TempID__){
                            // Whenever { is used outside of a string, __TempID__ will always be ID::KeywordArgs
                            case ID::KeywordArgs:
                                __Tokens__.emplace_back(Token::Bracket, "BRACK");
                                __TempIDList__.pop_back();
                                __TempString__ = "";
                                break;
                            
                            default:
                                throw error::RendorException("Random { found on line " + std::to_string(__LineNumber__));
                        }
                        break;

                    case '}':
                        __Tokens__.emplace_back(Token::Bracket, "BRACK");
                        break;
                }
            }

            // Preprocessor definitions 
            else if((__LineofCode__[i] == '#') && (__TempID__ != ID::Char)){
                std::string __PreProcessFunctionName__ = __LineofCode__.substr(0, __LineofCode__.find_first_of(' '));
                if(__PreProcessFunctionName__ == "#rdef"){
                    __Tokens__.emplace_back(Token::EntryFunction, "ENTRY");
                    __TempIDList__.emplace_back(ID::KeywordArgs);
                }
                else if(__PreProcessFunctionName__ == "#rdef_end"){
                    __Tokens__.emplace_back(Token::EndOfProgram, "END");
                }
            }

            else if(__LineofCode__[i] == '='){
                switch(__TempID__){
                    case ID::None:
                        __Tokens__.emplace_back(Token::Variable, __TempString__);
                        __TempString__ = "";
                        break;
                    
                    case ID::KeywordArgs:
                        if(__SpecificTempID__ != SpecificID::EqualOperator){
                            __SpecificTempID__ = SpecificID::EqualOperator;
                            break;
                        }
                        __Tokens__.emplace_back(Token::ComparisonObject, __TempString__);
                        __Tokens__.emplace_back(Token::Operator, "EQUAL");
                        __TempString__ = "";
                        break;

                    default:
                        throw error::RendorException("Random = found on line " + std::to_string(__LineNumber__));
                }
            }

            else if(__LineofCode__[i] == '^' ||
            __LineofCode__[i] == '*' || __LineofCode__[i] == '/' ||
            __LineofCode__[i] == '+' || __LineofCode__[i] == '-'){
                switch(__LineofCode__[i]){
                    case '^':
                        __Tokens__.emplace_back(Token::Arithmic, "^");
                        break;

                    case '*':
                        __Tokens__.emplace_back(Token::Arithmic, "*");
                        break;

                    case '/':
                        __Tokens__.emplace_back(Token::Arithmic, "/");
                        break;

                    case '+':
                        switch(__SpecificTempID__){
                            case SpecificID::IncrementDecrementCheck:
                                __Tokens__.pop_back();
                                __Tokens__.emplace_back(Token::Increment, "++");
                                break;

                            default:
                                __Tokens__.emplace_back(Token::Arithmic, "+");
                                __SpecificTempID__ = SpecificID::IncrementDecrementCheck;
                                break;
                        }
                        break;

                    case '-':
                        switch(__SpecificTempID__){
                            case SpecificID::IncrementDecrementCheck:
                                __Tokens__.pop_back();
                                __Tokens__.emplace_back(Token::Decrement, "--");
                                break;

                            default:
                                __Tokens__.emplace_back(Token::Arithmic, "-");
                                __SpecificTempID__ = SpecificID::IncrementDecrementCheck;
                                break;
                        }
                        break;
                }
            }

            // Push chars in a temporary string
            else{
                __TempString__.push_back(__LineofCode__[i]);
                // std::cout << static_cast<std::underlying_type<ID>::type>(__TempID__) << std::endl;
            }
        }
    }
    return __Tokens__;
}