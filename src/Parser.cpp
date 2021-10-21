#include "Parser.hpp"

std::string ByteCodeGen(const std::string& NodeType, const std::unique_ptr<Node>& NodeClass);

std::vector<std::string> Parser(const std::vector<std::pair<Lex::Token, std::string>>& Tokens){
    Main Script;
    Lex::Token LastToken;
    CurrentScope Scope = CurrentScope::Global;
    std::vector<std::string> ByteCode;
    
    for(auto const& [token, value] : Tokens){
        std::cout << "Token: " << static_cast<std::underlying_type<Lex::ID>::type>(token) << " " << value << std::endl;
        // Main Function 
        if(token == Lex::Token::EntryFunction){
            Scope = CurrentScope::Main;
            Script.GlobalBody->emplace_back(std::make_unique<MarkRdef>());
            continue;
        }

        else if(token == Lex::Token::EndOfProgram){
            Scope = CurrentScope::Global;
            continue;
        }

        // Parens
        else if(token == Lex::Token::Paren){
            if(Scope == CurrentScope::Main){
                if(value == "("){
                    continue;
                } 
                else if(")"){
                    continue;
                }
            }
        }
        
        // Variables
        else if(token == Lex::Token::Variable){
            if(Scope == CurrentScope::Global){
                Script.GlobalBody->emplace_back(std::make_unique<AssignVariable>(value));
            }
            else if(Scope == CurrentScope::Main){
                Script.MainBody->emplace_back(std::make_unique<AssignVariable>(value));
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
        
        LastToken = token;
    }

    ByteCode.emplace_back("Load 0"); // For Global Scope
    for(const auto& Node : (*Script.GlobalBody)){
        if(Node->Type() == "mark_rdef"){
            ByteCode.emplace_back("Load 1"); // For Main Scope
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

std::string ByteCodeGen(const std::string& NodeType, const std::unique_ptr<Node>& NodeClass){
    if(NodeType == "mark_global"){
        return "End 1";
    }
    else if(NodeType == "assignment"){
        auto& AssignmentNode = dynamic_cast<AssignVariable&>(*NodeClass); // if we reach here, it should be a AssignVariable object

        return (boost::format("Assign %s, %s, %s") % static_cast<std::underlying_type<VariableTypes>::type>(AssignmentNode.VariableType) % AssignmentNode.VariableName % AssignmentNode.Value).str();
    } 
    else{
        throw error::RendorException((boost::format("Fatal Error: rendorc can't generate bytecode for %s") % NodeType).str());
    }
}