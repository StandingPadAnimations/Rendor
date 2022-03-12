#include "RendorInterpreter/Interpreter.hpp"

/* -------------------------------------------------------------------------- */
/*                          Memory related functions                          */
/* -------------------------------------------------------------------------- */
/* ---------------------------- Garbage Collector --------------------------- */
void Interpreter::GarbageCollector()
{
    for (auto Constant : GreyObjects)
    {
        BlackObjects.push_back(std::move(Constant));
        break;
    }
    WhiteObjects.clear();
}

/* ------------------------ Function to add constants ----------------------- */
void Interpreter::CreateConstant(std::string_view Constant)
{
    size_t CommaIndex = Constant.find_first_of(",");

    if (CommaIndex != std::string::npos)
    {
        std::string Type  (Constant.begin(), Constant.begin() + CommaIndex);
        std::string Const (Constant.begin() + (CommaIndex + 2), Constant.end());

        switch (TypeMapping.at(Type))
        {
            case ConstType::INT64:
            {
                TypeObject_U Object = std::make_unique<Int64>(Const);
                RendorStack.emplace(Object.get(), std::move(Object));
                break;
            }
            case ConstType::DOUBLE:
            {
                TypeObject_U Object = std::make_unique<Double>(Const);
                RendorStack.emplace(Object.get(), std::move(Object));
                break;
            }
            case ConstType::STRING:
            {
                TypeObject_U Object = std::make_unique<String>(Const);
                RendorStack.emplace(Object.get(), std::move(Object));
                break;
            }
            case ConstType::BOOL:
            {
                TypeObject_U Object = std::make_unique<Bool>(Const);
                RendorStack.emplace(Object.get(), std::move(Object));
                break;
            }
            case ConstType::ARITHMETHIC:
            {
                PostFixEval(Const);
                break;
            }
            case ConstType::REFERENCE:
            {
                GetConstFromVariable(std::string{Const});
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else 
    {
        if (Constant.find_first_not_of("1234567890.") == std::string::npos)
        {
            if (Constant.find_first_of(".") != std::string::npos)
            {
                TypeObject_U Object = std::make_unique<Double>(std::string{Constant});
                RendorStack.emplace(Object.get(), std::move(Object));
            }
            else
            {
                TypeObject_U Object = std::make_unique<Int64>(std::string{Constant});
                RendorStack.emplace(Object.get(), std::move(Object));
            }
        }
    }
}

void Interpreter::FindConst(std::string_view Const)
{
    auto [Index, List] = Objects[std::string{Const}];
    std::vector<TypeObject>* VectorToSearch = VectorMapping.at(List);

    /* ----------------- if the vector is smaller then the index ---------------- */
    if (Index + 1 > VectorToSearch->size())
    {
        auto FindIterator = std::find_if(
        VectorToSearch->begin(), 
        VectorToSearch->end(),
        [&Const] (TypeObjectPtr Ptr) 
        {
            if (!Ptr.expired())
            {
                return Ptr.lock()->m_Value == Const;
            } 
            else 
            {
                return false;
            }
        }
        );
        RendorStack.emplace(FindIterator->get(), TypeObject_U());
        return;
    }

    /* -------------------------------- otherwise ------------------------------- */
    for (size_t i = Index; Index > 0; --Index)
    {
        Type* Ptr = VectorToSearch->at(i).get();
        if (Ptr != nullptr)
        {
            if (Ptr->m_Value == Const)
            {
                RendorStack.emplace(Ptr, TypeObject_U());
                return;
            }
        } 
    }
}

void Interpreter::MarkConstantBlack(TypeObject Const)
{
    Const->ColorOfObj = GCColor::BLACK;
    BlackObjects.push_back(std::move(Const));
}

void Interpreter::GetConstFromVariable(const std::string& Variable)
{
    if (CurrentScopeVariables->contains(Variable))
    {
        RendorStack.emplace((*CurrentScopeVariables)[Variable]->m_ValueClass.get(), TypeObject_U());
    }
    else if (GlobalVariables->contains(Variable))
    {
        RendorStack.emplace((*GlobalVariables)[Variable]->m_ValueClass.get(), TypeObject_U());
    }
    else 
    {
        throw error::RendorException("Undefined variable: " + Variable);
    }
}
