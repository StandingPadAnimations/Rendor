#include "RendorCompiler/lexer.hpp"
using namespace Lex;

// TODO: Improve readability
std::vector<std::pair<Token, std::string>> Lexer::Tokenize(const std::string& Code, const std::string& ParentPath){ // ? Perhaps string_view be used over const std::string&
    std::stringstream ss(Code);
    std::string LineofCode;
    std::vector<std::pair<Token, std::string>> Tokens;
    std::vector<std::pair<Token, std::string>> MainFunction;
    bool IsMainFunction = false;
    
    unsigned int LineNumber = 0;

    while(std::getline(ss, LineofCode)){ // TODO: This could definitely be improved. stringstreams take a lot of memory
        ++LineNumber;
        std::vector<ID> TempIDList {ID::None};
        SpecificID SpecificTempID = SpecificID::None; 
        std::string TempString = "";
        ID TempID = TempIDList.back();

        for(size_t i = 0; i < LineofCode.size(); ++i){
            TempID = TempIDList.back();

            // Processing for strings
            if(TempID == ID::Char){ // TODO: Add variable strings. Syntax could be v"blah blah {random_variable} blah blah"
                switch(LineofCode[i]){
                    case '\"': // When it encounters a " char
                        switch(SpecificTempID){
                            case SpecificID::CharDouble:
                                Tokens.emplace_back(Token::String, TempString);
                                TempIDList.pop_back();
                                TempString = "";
                                break;
                            
                            default:
                                TempString.push_back(LineofCode[i]);
                        }
                        break;

                    case '\'': // When it encounters a ' char
                        switch(SpecificTempID){
                            case SpecificID::CharSingle:
                                Tokens.emplace_back(Token::String, TempString);
                                TempIDList.pop_back();
                                TempString = "";
                                break;
                            
                            default:
                                TempString.push_back(LineofCode[i]);
                        }
                        break;

                    case '`': // When it encounters a ` char
                        switch(SpecificTempID){
                            case SpecificID::CharTilda:
                                Tokens.emplace_back(Token::String, TempString);
                                TempIDList.pop_back();
                                TempString = "";
                                break;
                            
                            default:
                                TempString.push_back(LineofCode[i]);
                        }
                        break;

                    default: // Otherwise
                        TempString.push_back(LineofCode[i]);
                }
            }
            
            // Checks for beginning and end of a string
            else if(
                ((LineofCode[i] == '\"') || 
                (LineofCode[i] == '\'') || 
                (LineofCode[i] == '`')) && 
                ((TempID == ID::None) || 
                (TempID == ID::KeywordArgs))
            ){
                TempIDList.emplace_back(ID::Char);
                TempString = "";

                // * Defines what should end string
                switch(LineofCode[i]){
                    case '\'': 
                        SpecificTempID = SpecificID::CharSingle; // * SpecificTempID prevents strings from closing with the wrong char 
                        break;

                    case '\"':
                        SpecificTempID = SpecificID::CharDouble;
                        break;

                    case '`':
                        SpecificTempID = SpecificID::CharTilda;
                        break;
                }
            }

            // Spaces in places other then strings 
            else if((LineofCode[i] == ' ') && (TempID != ID::Char)){
                continue;
            }

            // Checks for operators
            else if(
            (std::find(std::begin(Operators), std::end(Operators), TempString) != std::end(Operators)) && 
            (TempID == ID::KeywordArgs)
            ){
                Tokens.emplace_back(Token::Operator, TempString);
                TempString = "";

                // * To account for chars not being added when a operator is found
                TempString.push_back(LineofCode[i]);
            }

            // Comments and division
            // TODO: Add multiline comments
            else if(
            (LineofCode[i] == '/') && 
            (TempID != ID::Char)
            ){
                if(LineofCode[i+1] == '/'){ // * Comments
                    break;
                } else{ // * Division 
                    if(TempString.find_first_not_of("1234567890.") != std::string::npos){ // * this checks TempString to see if there's a variable or number 
                        Tokens.emplace_back(Token::BopVariableRef, TempString);
                    } else{
                        if(TempString.find_first_of(".") != std::string::npos){ // * to check if it's an int or float
                            Tokens.emplace_back(Token::Float, TempString);
                        } else{
                            Tokens.emplace_back(Token::Int, TempString);
                        }
                    }

                    TempString = ""; 
                    TempString.push_back(LineofCode[i]); // to get the operator
                    Tokens.emplace_back(Token::Bop, TempString);
                    TempString = ""; 
                    SpecificTempID = SpecificID::None; // * To avoid issues with ints at the end of lines being assigned as floats and vice versa
                }
            }

            // Checks for parens
            // ! Parens don't operate well for math operations
            // TODO: Fix paren code for math 
            else if(
            (LineofCode[i] == '(') && 
            (TempID != ID::Char)
            ){
                if(std::find(std::begin(Keywords), std::end(Keywords), TempString) != std::end(Keywords)){ // keywords 
                    Tokens.emplace_back(Token::Keyword, TempString);
                    TempIDList.emplace_back(ID::KeywordArgs);
                    TempString = "";
                } else {
                    if(TempID == ID::Rdef){ // rdef
                        if(TempString != "rdefmain"){ // if the main function isn't #rdef main
                            throw error::RendorException((boost::format("rdef defined without the name main on line %s") % LineNumber).str());
                        } else{ // clear TempString since we've used it
                            TempString = "";
                        }
                    } else{
                        if(TempString.find_first_not_of("1234567890.") != std::string::npos){ // this checks TempString to see if there's a variable or number 
                            Tokens.emplace_back(Token::BopVariableRef, TempString);
                        } else{
                            if(TempString.find_first_of(".") != std::string::npos){ // to check if it's an int or float
                                Tokens.emplace_back(Token::Float, TempString);
                            } else{
                                Tokens.emplace_back(Token::Int, TempString);
                            }
                        }
                        TempString = "";
                    }
                }
                Tokens.emplace_back(Token::Paren, "(");
            }

            else if(
            (LineofCode[i] == ')') && 
            (TempID != ID::Char)
            ){
                // TODO: Make this better
                switch(TempID){
                    case ID::KeywordArgs:
                        Tokens.emplace_back(Token::ArgumentObjects, TempString);
                        Tokens.emplace_back(Token::Paren, ")");
                        break;

                    default:
                        switch(SpecificTempID){ // To check for ints and floats that already exist in the code
                            case SpecificID::Int:
                                Tokens.emplace_back(Token::Int, TempString);
                                TempIDList.pop_back();
                                break;

                            case SpecificID::Float:
                                Tokens.emplace_back(Token::Float, TempString);
                                TempIDList.pop_back();
                                break;

                            default:
                                break;
                        }
                        TempString = "";
                        Tokens.emplace_back(Token::Paren, ")");
                        SpecificTempID = SpecificID::None; // We already handle ints and floats here
                        break;
                }
            }

            // Brackets
            // TODO: Make sure this works with new Paren code when it's done 
            else if(
                ((LineofCode[i] == '{') || 
                (LineofCode[i] == '}')) && 
            (TempID != ID::Char)
            ){
                switch(LineofCode[i]){
                    case '{':
                        switch(TempID){
                            // * Whenever { is used outside of a string, TempID will always be ID::KeywordArgs or ID::Rdef
                            case ID::Rdef:
                            case ID::KeywordArgs:
                                Tokens.emplace_back(Token::Bracket, "{");
                                TempIDList.pop_back();
                                TempString = "";
                                break;
                            
                            default:
                                throw error::RendorException((boost::format("Random { found on line %s") % LineNumber).str());
                        }
                        break;

                    case '}':
                        Tokens.emplace_back(Token::Bracket, "}");
                        break;
                }
            }

            // * Rendor -> Cpp Compiler Hints
            else if(
            (LineofCode[i] == '~') &&
            (TempID != ID::Char)
            ){
                if(LexerCompileMode){
                    Tokens.emplace_back(Token::CppCompileTypeHint, LineofCode);
                    break;
                } else {
                    break;
                }
            }

            // Preprocessor definitions 
            // TODO: Make this easier to read for the love of God
            // ? Things here could be optimized better, esspecially for import paths 
            else if(
            (LineofCode[i] == '#') && 
            (TempID != ID::Char)
            ){
                size_t IndexofSpace = LineofCode.find_first_of(' ');
                std::string PreProcessFunctionName = LineofCode.substr(0, IndexofSpace);

                if(PreProcessFunctionName == "#rdef"){
                    IsMainFunction = true;
                    Tokens.emplace_back(Token::EntryFunction, "MainStart");
                    TempIDList.emplace_back(ID::KeywordArgs);
                    TempIDList.emplace_back(ID::Rdef);
                }
                
                else if(PreProcessFunctionName == "#rdef_end"){
                    if(IsMainFunction){ 
                        IsMainFunction = false;
                    } else{
                        throw error::RendorException((boost::format("End of main function found without declaration of the beggining; Line %s") % LineNumber).str());
                    }
                    Tokens.emplace_back(Token::EndOfProgram, "MainEnd");
                }

                else if(PreProcessFunctionName == "#import"){
                    // TODO: Improve getting the import, this sucks
                    // ! This sucks so much I have to add this. Do better next time future me
                    size_t IndexOfImportString = LineofCode.find_first_of("\""); 
                    size_t IndexOfImportStringEnd = LineofCode.find_last_of("\"");
                    std::string FileImport = LineofCode.substr(IndexOfImportString, IndexOfImportStringEnd);

                    FileImport.erase(std::remove(FileImport.begin(), FileImport.end(), '\"'), FileImport.end());
                    std::string ImportPath = (boost::format("%s/%s.ren") % ParentPath % FileImport).str();
                    
                    if(boost::filesystem::exists(ImportPath)){
                        boost::filesystem::path PathofImport(ImportPath); // ? Is there a way to get the file path without creating a new boost::filesystem::path object?
                        Tokens.emplace_back(Token::Import, PathofImport.string());
                        Imports.emplace_back(PathofImport);
                    } else{
                        throw error::RendorException((boost::format("Can't import non-existent file; Line %s") % LineNumber).str());
                    }
                    break;
                }
            }
            
            // TODO: Improve this to make operators work better. It would also be nice to have stuff like if(!random_booL) as an option in Rendor
            // For if statement operators 
            else if( 
                ((LineofCode[i] == '=') || 
                (LineofCode[i] == '!') || 
                (LineofCode[i] == '>') ||
                (LineofCode[i] == '<')) && 
            (TempID == ID::KeywordArgs)
            ){
                if(TempString.find_first_not_of("=!><") == std::string::npos){
                    TempString.push_back(LineofCode[i]);
                    if(std::find(std::begin(Operators), std::end(Operators), TempString) != std::end(Operators)){
                        Tokens.emplace_back(Token::Operator, TempString);
                        TempString = "";
                    }
                } else{
                    Tokens.emplace_back(Token::ComparisonObject, TempString);
                    TempString = "";
                    TempString.push_back(LineofCode[i]);
                }
            }

            // For variable assignment. It's more readable like this. 
            else if(
            (LineofCode[i] == '=') && 
            (TempID == ID::None)
            ){ 
                Tokens.emplace_back(Token::Variable, TempString);
                TempString = "";
            }

            // ints and floats
            else if( 
                ((std::isdigit(LineofCode[i])) || 
                (
                    (LineofCode[i] == '.') && // * TempID should be ID::Number when it encounters a . in a float
                    (TempID == ID::Number) // ? this is to avoid future chain function calls from being assigned as floats but I feel like I could do better
                )) && 
            (TempID != ID::Char)
            ){
                TempIDList.emplace_back(ID::Number);
                TempID = TempIDList.back();
                TempString.push_back(LineofCode[i]);

                switch(LineofCode[i]){
                    case '.':
                        SpecificTempID = SpecificID::Float;
                        break;
                    
                    default:
                        switch(SpecificTempID){
                            case SpecificID::Int:
                            case SpecificID::Float:
                                break;
                            
                            default:
                                SpecificTempID = SpecificID::Int;
                        }
                }
            }
            
            // Binary operators
            else if(
                    ((LineofCode[i] == '^') ||
                    (LineofCode[i] == '*') || 
                    (LineofCode[i] == '+') ||
                    (LineofCode[i] == '-'))
                ){
                    if(
                    (SpecificTempID == SpecificID::Increment) ||
                    (SpecificTempID == SpecificID::Decrement)
                    ){
                        continue; // * We don't want to handle increment/decrement symbols but we also don't want to add it to TempString
                    }

                    if(TempString.find_first_not_of("1234567890.") != std::string::npos){ // this checks TempString to see if there's a variable or number 
                        Tokens.emplace_back(Token::BopVariableRef, TempString);
                    } else{
                        if(TempString.find_first_of(".") != std::string::npos){ // to check if it's an int or float
                            Tokens.emplace_back(Token::Float, TempString);
                        } else{
                            if(TempString.size() > 0){ // this should be obvious
                                Tokens.emplace_back(Token::Int, TempString);
                            }
                        }
                    }

                    if(
                    (LineofCode[i] == '+') && 
                    (LineofCode[i+1] == '+')
                    ){
                        SpecificTempID = SpecificID::Increment;
                        continue; // We don't want to assign tokens yet 
                    } 
                    else if(
                    (LineofCode[i] == '-') && 
                    (LineofCode[i+1] == '-')
                    ){
                        SpecificTempID = SpecificID::Decrement;
                        continue; // We don't want to assign tokens yet 
                    }
                    TempString = ""; 
                    TempString.push_back(LineofCode[i]); // to get the operator
                    Tokens.emplace_back(Token::Bop, TempString);
                    TempString = ""; 
                    SpecificTempID = SpecificID::None; // * To avoid issues with ints at the end of lines being assigned as floats and vice versa
                }

            else{
                // * Push chars in a temporary string
                TempString.push_back(LineofCode[i]);
            }
        }

        // Checks for leftover numbers at the end of lines 
        // ? I feel like this could be made better 
        switch(TempID){
            case ID::Number:
                if(TempString.size() > 0){ // if there actually is anything worth tokenizing 
                    switch(SpecificTempID){
                        case SpecificID::Int:
                            Tokens.emplace_back(Token::Int, TempString);
                            break;

                        case SpecificID::Float:
                            Tokens.emplace_back(Token::Float, TempString);
                            break;

                        default:
                            break;
                    }
                }
            default: // Increment amd Decrement handling
                switch(SpecificTempID){
                    case SpecificID::Increment:
                        Tokens.emplace_back(Token::Increment, TempString);
                        break;

                    case SpecificID::Decrement:
                        Tokens.emplace_back(Token::Decrement, TempString);
                        break;

                    default:
                        break;
                }
                break;
        }
    }
    return Tokens;
}