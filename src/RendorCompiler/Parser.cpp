#include "RendorCompiler/Parser.hpp"

/*----------------------------------------------------------------
In the parser loop(which streams tokens), please avoid using the continue keyword. In the past, it has caused unexpected behavior. The exception(s) are:
    * When you have to move on to the next token but know you may execute unwanted code(though using if-else is preferable here)

If you do use continue, please do:
    LastToken = token;
    LastValue = value;
    ++TokenIndex;
    continue;

This way, the parser works as expected
----------------------------------------------------------------*/

std::string ByteCodeGen (const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::map<std::string, char>& Variables);
void SetVariable // ! This is way too many arguments
    ( 
    std::map<std::string, char>& Variables, 
    const std::string& value, AssignVariable& AssignmentNode, 
    const std::string& VariableName, 
    const std::vector<std::pair<Lex::Token, std::string>>& Tokens, 
    const unsigned int& TokenIndex, 
    TempID& ParserTempID
    );

std::vector<std::string> Parser (const std::vector<std::pair<Lex::Token, std::string>>& Tokens)
{
    Main Script;
    TempID ParserTempID = TempID::None;
    Lex::Token LastToken;
    std::string LastValue;
    bool InParen = false; // Allows us to do arguments
    std::vector<std::unique_ptr<Node>> *Scope = &Script.Global.ConnectedNodes;
    std::vector<std::string> ByteCode;
    std::map<std::string, char> Variables; // Defines variable and it's type. Used for references and copying
    unsigned int LineNumber = 1;
    unsigned int TokenIndex = 0;
    
    for (auto const& [token, value] : Tokens)
    {
        std::cout << "Token: " << static_cast<std::underlying_type<Lex::Token>::type>(token) << " " << value << std::endl;

        LastToken = token;
        LastValue = value;
        ++TokenIndex;
    }
    

    return ByteCode;
}

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::map<std::string, char>& Variables)
{
    
}

// This is made as a separate function to make the code easier to navagate
// ! Arguments could be better handled 
void SetVariable 
    (
    std::map<std::string, char>& Variables, 
    const std::string& value, AssignVariable& AssignmentNode, 
    const std::string& VariableName, 
    const std::vector<std::pair<Lex::Token, std::string>>& Tokens, 
    const unsigned int& TokenIndex,
    TempID& ParserTempID)
{

}