#include "RendorCompiler/Parser.hpp"

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::map<std::string, char>& Variables);
void SetVariable(std::map<std::string, char>& Variables, const std::string& value, AssignVariable& AssignmentNode, std::string& VariableName);

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens){
    Main Script;
    Lex::Token LastToken;
    bool InParen = false; // Allows us to do arguments
    std::vector<std::unique_ptr<Node>> *Scope = &Script.Global.ConnectedNodes;
    std::vector<std::string> ByteCode;
    std::map<std::string, char> Variables; // Defines variable and it's type. Used for references and copying
    
    for(auto const& [token, value] : Tokens){
        std::cout << "Token: " << static_cast<std::underlying_type<Lex::ID>::type>(token) << " " << value << std::endl;
        // Main Function 
        if(token == Lex::Token::EntryFunction){
            Scope->push_back(std::make_unique<MarkRdef>()); // Marks the beginning of the main function
            Scope = &Script.MainFunction.MainFunctionBody.ConnectedNodes;
            continue;
        }

        else if(token == Lex::Token::EndOfProgram){
            Scope->push_back(std::make_unique<MarkGlobal>()); // To generate the command for the end of the main function
            Scope = &Script.Global.ConnectedNodes;
            continue;
        }

        // Parens
        else if(token == Lex::Token::Paren){
            if(value == "("){
                InParen = true;
                continue;
            } 
            else if(")"){
                InParen = false;
                continue;
            }
        }
        
        // Variables
        else if(token == Lex::Token::Variable){
            Scope->push_back(std::make_unique<AssignVariable>(value));
            Variables[value] = ' ';
        }
        
        // Values for variables
        else if(
        (token == Lex::Token::Int) ||
        (token == Lex::Token::Float) ||
        (token == Lex::Token::String) ||
        (token == Lex::Token::Bool) ||
        (token == Lex::Token::VariableReference)
        ){
            if(LastToken == Lex::Token::Variable){
                // Editing the actual object
                auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Scope->back());
                std::string VariableName = AssignmentNode.VariableName;
                AssignmentNode.Value = value;

                // Set the type of the variable
                switch(token){
                    case Lex::Token::Int:
                        AssignmentNode.VariableType = VariableTypes::Int;
                        Variables[VariableName] = 'N';
                        break;

                    case Lex::Token::Float:
                        AssignmentNode.VariableType = VariableTypes::Float;
                        Variables[VariableName] = 'F';
                        break;

                    case Lex::Token::String:
                        AssignmentNode.VariableType = VariableTypes::String;
                        Variables[VariableName] = 'S';
                        break;

                    case Lex::Token::Bool:
                        AssignmentNode.VariableType = VariableTypes::Bool;
                        Variables[VariableName] = 'B';
                        break;

                    case Lex::Token::VariableReference: 
                        if(value[0] == '&'){
                            std::string VariableBeingReferenced(value.c_str()+1, value.size()-1);
                            SetVariable(Variables, VariableBeingReferenced, AssignmentNode, VariableName);
                            AssignmentNode.Value = (boost::format("_&&%s") % VariableBeingReferenced).str(); // to let the interpreter know that it's refering to another variable
                        }
                        else{
                            SetVariable(Variables, value, AssignmentNode, VariableName);
                            AssignmentNode.Value = (boost::format("_&%s") % value).str(); // to let the interpreter know that it's copying to another variable
                        }
                        break;
                    
                    default:
                        throw error::RendorException("Invalid token type; Assignment Variable Fail");
                }
            }
        }
        
        // Keywords
        else if(token == Lex::Token::Keyword){
            Scope->push_back(std::make_unique<RendorKeyWord>(value));
        }

        // Arguments
        else if(token == Lex::Token::ArgumentObjects){
            if(InParen){
                auto& RendorKeyWordNode = dynamic_cast<RendorKeyWord&>(*Scope->back());
                RendorKeyWordNode.Args = value;
            } else{
                throw error::RendorException("Arguments can only be used in ()");
            }
        }
        
        else if(token == Lex::Token::Increment){
            Scope->push_back(std::make_unique<Increment>(value));
        }

        else if(token == Lex::Token::Decrement){
            Scope->push_back(std::make_unique<Decrement>(value));
        }
        LastToken = token;
    }
    
    ByteCode.emplace_back("LOAD 0"); // For Global Scope
    for(const auto& Node : (*Script.GlobalBody)){
        if(Node->Type() == NodeType::MarkRdef){
            ByteCode.emplace_back("LOAD 1"); // For Main Scope
            // Start looping in main
            for(const auto& Node : (*Script.MainBody)){
                ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node, Variables)); // Generate bytecode
            }
            continue; // Move on to next node
        }
        ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node, Variables)); // Generate bytecode
    }
    ByteCode.emplace_back("END 0"); // End Global Scope
    return ByteCode;
}

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass, std::map<std::string, char>& Variables){
    if(ClassType == NodeType::MarkGlobal){
        return ("END 1");
    }
    else if(ClassType == NodeType::AssignVariable){
        auto& AssignmentNode = dynamic_cast<AssignVariable&>(*NodeClass); // if we reach here, it should be a AssignVariable object
        std::string Type;

        // is needed to make sure that variables are assigned correctly at runtime 
        switch(AssignmentNode.VariableType){
            case VariableTypes::Int:
                Type = "0";
                break;
            
            case VariableTypes::Float:
                Type = "1";
                break;

            case VariableTypes::String:
                Type = "2";
                break;

            case VariableTypes::Bool:
                Type = "3";
                break;
            
            default:
                throw error::RendorException("Invalid node type; Assignment Variable Fail");
        }
        return (boost::format("CONST %s %s\nASSIGN %s") % Type % AssignmentNode.Value % AssignmentNode.VariableName).str();
    } 

    else if(ClassType == NodeType::RendorKeyWord){
        auto& RendorKeyWordNode = dynamic_cast<RendorKeyWord&>(*NodeClass); // if we reach here, it should be a RendorKeyWord object

        if(RendorKeyWordNode.KeyWord == "echo"){ // if keyword is echo 
            if(Variables.find(RendorKeyWordNode.Args) == Variables.end()){ // CONSTANT is a variable defined in rendorvm
                return (boost::format("CONST 2 %s\nECHO CONSTANT") % RendorKeyWordNode.Args).str(); // we pretend it's a string cause ECHO will only use strings anyway
            } else{
                return (boost::format("ECHO %s") % RendorKeyWordNode.Args).str();
            }
        } else { // in case it's not a keyword somehow
            throw error::RendorException((boost::format("WTH Error; %s is not a keyword. This error should not appear so please post an issue on the GitHub") % RendorKeyWordNode.KeyWord).str());
        }
    }

    else if(ClassType == NodeType::Increment){
        auto& IncrementNode = dynamic_cast<Increment&>(*NodeClass); // if we reach here, it should be an Increment object

        if(Variables.find(IncrementNode.Args) == Variables.end()){ // You can only increment variables that have been defined
            throw error::RendorException((boost::format("Fatal Error; %s is not defined") % IncrementNode.Args).str());
        } else{
            if(Variables[IncrementNode.Args] != 'N'){
                throw error::RendorException((boost::format("Fatal Error; %s is not an interger; only integers can be incremented") % IncrementNode.Args).str());
            }
            return (boost::format("INCREMENT %s") % IncrementNode.Args).str();
        }
    }

    else if(ClassType == NodeType::Decrement){
        auto& DecrementNode = dynamic_cast<Decrement&>(*NodeClass); // if we reach here, it should be an Increment object

        if(Variables.find(DecrementNode.Args) == Variables.end()){ // You can only increment variables that have been defined
            throw error::RendorException((boost::format("Fatal Error; %s is not defined") % DecrementNode.Args).str());
        } else{
            if(Variables[DecrementNode.Args] != 'N'){
                throw error::RendorException((boost::format("Fatal Error; %s is not an interger; only integers can be decremented") % DecrementNode.Args).str());
            }
            return (boost::format("DECREMENT %s") % DecrementNode.Args).str();
        }
    }
    
    else{
        throw error::RendorException((boost::format("Fatal Error; rendorc can't generate bytecode for %s node") % static_cast<std::underlying_type<NodeType>::type>(ClassType)).str());
    }
}

// This is made as a separate function to make the code easier to navagate
void SetVariable(std::map<std::string, char>& Variables, const std::string& value, AssignVariable& AssignmentNode, std::string& VariableName){
    switch(Variables[value]){
        case 'N':
            AssignmentNode.VariableType = VariableTypes::Int;
            Variables[VariableName] = 'N';
            break;
        
        case 'F':
            AssignmentNode.VariableType = VariableTypes::Float;
            Variables[VariableName] = 'F';
            break;

        case 'S':
            AssignmentNode.VariableType = VariableTypes::String;
            Variables[VariableName] = 'S';
            break;

        case 'B':
            AssignmentNode.VariableType = VariableTypes::Bool;
            Variables[VariableName] = 'B';
            break;
    }
}