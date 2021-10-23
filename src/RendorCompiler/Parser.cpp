#include "RendorCompiler/Parser.hpp"

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass);

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens){
    Main Script;
    Lex::Token LastToken;
    bool InParen = false; // Allows us to do arguments
    CurrentScope Scope = CurrentScope::Global;
    std::vector<std::string> ByteCode;
    
    for(auto const& [token, value] : Tokens){
        std::cout << "Token: " << static_cast<std::underlying_type<Lex::ID>::type>(token) << " " << value << std::endl;
        // Main Function 
        if(token == Lex::Token::EntryFunction){
            Scope = CurrentScope::Main;
            Script.GlobalBody->push_back(std::make_unique<MarkRdef>()); // Marks the beginning of the main function
            continue;
        }

        else if(token == Lex::Token::EndOfProgram){
            Scope = CurrentScope::Global;
            Script.MainBody->push_back(std::make_unique<MarkGlobal>()); // To generate the command for the end of the main function
            continue;
        }

        // Parens
        else if(token == Lex::Token::Paren){
            if(Scope == CurrentScope::Main){
                if(value == "("){
                    InParen = true;
                    continue;
                } 
                else if(")"){
                    InParen = false;
                    continue;
                }
            }
        }
        
        // Variables
        else if(token == Lex::Token::Variable){
            if(Scope == CurrentScope::Global){
                Script.GlobalBody->push_back(std::make_unique<AssignVariable>(value));
            }
            else if(Scope == CurrentScope::Main){
                Script.MainBody->push_back(std::make_unique<AssignVariable>(value));
            }
        }
        
        // Values for variables
        else if(
        (token == Lex::Token::Int) ||
        (token == Lex::Token::Float) ||
        (token == Lex::Token::String) ||
        (token == Lex::Token::Bool)
        ){
            if(LastToken == Lex::Token::Variable){
                if(Scope == CurrentScope::Global){
                    // Editing the actual object
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Script.GlobalBody->back());
                    AssignmentNode.Value = value;

                    // Set the type of the variable
                    if(token == Lex::Token::Int){
                        AssignmentNode.VariableType = VariableTypes::Int;
                    }
                    else if(token == Lex::Token::Float){
                        AssignmentNode.VariableType = VariableTypes::Float;
                    }
                    else if(token == Lex::Token::String){
                        AssignmentNode.VariableType = VariableTypes::String;
                    }
                    else if(token == Lex::Token::Bool){
                        AssignmentNode.VariableType = VariableTypes::Bool;
                    }
                }
                else if(Scope == CurrentScope::Main){
                    // Editing the actual object
                    auto& AssignmentNode = dynamic_cast<AssignVariable&>(*Script.MainBody->back());
                    AssignmentNode.Value = value;

                    // Set the type of the variable
                    if(token == Lex::Token::Int){
                        AssignmentNode.VariableType = VariableTypes::Int;
                    }
                    else if(token == Lex::Token::Float){
                        AssignmentNode.VariableType = VariableTypes::Float;
                    }
                    else if(token == Lex::Token::String){
                        AssignmentNode.VariableType = VariableTypes::String;
                    }
                    else if(token == Lex::Token::Bool){
                        AssignmentNode.VariableType = VariableTypes::Bool;
                    }
                }
            }
        }
        
        // Keywords
        else if(token == Lex::Token::Keyword){
            if(Scope == CurrentScope::Global){
                Script.GlobalBody->push_back(std::make_unique<RendorKeyWord>(value));
            }
            else if(Scope == CurrentScope::Main){
                Script.MainBody->push_back(std::make_unique<RendorKeyWord>(value));
            }
        }

        // Arguments
        else if(token == Lex::Token::ArgumentObjects){
            if(InParen){
                if(Scope == CurrentScope::Global){
                    auto& RendorKeyWordNode = dynamic_cast<RendorKeyWord&>(*Script.GlobalBody->back());
                    RendorKeyWordNode.Args = value;
                }
                else if(Scope == CurrentScope::Main){
                    auto& RendorKeyWordNode = dynamic_cast<RendorKeyWord&>(*Script.MainBody->back());
                    RendorKeyWordNode.Args = value;
                }
            } else{
                throw error::RendorException("Arguments can only be used in ()");
            }
        }
        LastToken = token;
    }
    
    ByteCode.emplace_back("LOAD 0"); // For Global Scope
    for(const auto& Node : (*Script.GlobalBody)){
        if(Node->Type() == NodeType::MarkRdef){
            ByteCode.emplace_back("LOAD 1"); // For Main Scope
            // Start looping in main
            for(const auto& Node : (*Script.MainBody)){
                ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node)); // Generate bytecode
            }
            continue; // Move on to next node
        }
        ByteCode.emplace_back(ByteCodeGen(Node->Type(), Node)); // Generate bytecode
    }
    return ByteCode;
}

std::string ByteCodeGen(const NodeType& ClassType, const std::unique_ptr<Node>& NodeClass){
    if(ClassType == NodeType::MarkGlobal){
        return ("END 1");
    }
    else if(ClassType == NodeType::AssignVariable){
        auto& AssignmentNode = dynamic_cast<AssignVariable&>(*NodeClass); // if we reach here, it should be a AssignVariable object
        return (boost::format("ASSIGN %s, %s, %s") % static_cast<std::underlying_type<VariableTypes>::type>(AssignmentNode.VariableType) % AssignmentNode.VariableName % AssignmentNode.Value).str();
    } 
    else if(ClassType == NodeType::RendorKeyWord){
        auto& RendorKeyWordNode = dynamic_cast<RendorKeyWord&>(*NodeClass); // if we reach here, it should be a RendorKeyWord object
        if(RendorKeyWordNode.KeyWord == "echo"){
            return (boost::format("ECHO %s") % RendorKeyWordNode.Args).str();
        } else {
            throw error::RendorException((boost::format("Fatal Error: %s is not a keyword") % RendorKeyWordNode.KeyWord).str());
        }
    }
    else{
        throw error::RendorException((boost::format("Fatal Error: rendorc can't generate bytecode for %s node") % static_cast<std::underlying_type<NodeType>::type>(ClassType)).str());
    }
}