#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <cstdint>
#include <string>
#include <map>
#include <variant>

#include "Definitions.hpp"
#include "Exceptions.hpp"

struct Constant;
using Registers = std::vector<Constant>;

struct Constant
{
    RendorConst       Const{};
    RendorNum         Num{};
    RendorReference   Ref{};
    RendorVec         Vec{};
    ConstType         Type{};
    
    Constant(Constant&&) = default;
    Constant &operator=(Constant&&) = default;
    Constant(const Constant&) = delete;
    Constant&operator=(const Constant&) = delete;
    explicit Constant(const RendorConst& t_Const, const ConstType& t_Type) noexcept     : Const(t_Const), Type(t_Type){}
    explicit Constant(const RendorNum& t_Num, const ConstType& t_Type) noexcept       : Num(t_Num),   Type(t_Type){}
    explicit Constant(const RendorReference& t_Ref, const ConstType& t_Type) noexcept : Ref(t_Ref),   Type(t_Type){}
    explicit Constant(const RendorConst& t_Const, const RendorNum& t_Num, const RendorReference& t_Ref, const ConstType& t_Type) noexcept : 
    Const(t_Const), 
    Num(t_Num), 
    Ref(t_Ref), 
    Type(t_Type){}

    /* ------------------------------ Clone method ------------------------------ */
    Constant clone() const noexcept
    {
        return Constant{Const, Num, Ref, Type};
    }

    /* ----------------------------------- add ---------------------------------- */
    friend Constant operator+(const Constant& lhs, const Constant& rhs)
    {
        Constant Ret{0, ConstType::CONST_NUM};
        Ret.Type = ConstType::CONST_NUM;
        Ret.Num = std::visit([&](auto lhs_val, auto rhs_val)
                {
                    using ResultType = decltype(lhs_val + rhs_val);
                    return RendorNum{static_cast<ResultType>(lhs_val) + static_cast<ResultType>(rhs_val)};
                }, lhs.Num, rhs.Num);
        return Ret;
    }

    /* ----------------------------------- sub ---------------------------------- */
    friend Constant operator-(const Constant& lhs, const Constant& rhs)
    {
        Constant Ret{0, ConstType::CONST_NUM};
        Ret.Type = ConstType::CONST_NUM;
        Ret.Num = std::visit([&](auto lhs_val, auto rhs_val)
                {
                    using ResultType = decltype(lhs_val - rhs_val);
                    return RendorNum{static_cast<ResultType>(lhs_val) - static_cast<ResultType>(rhs_val)};
                }, lhs.Num, rhs.Num);
        return Ret;
    }

    /* ----------------------------------- mul ---------------------------------- */
    friend Constant operator*(const Constant& lhs, const Constant& rhs)
    {
        Constant Ret{0, ConstType::CONST_NUM};
        Ret.Type = ConstType::CONST_NUM;
        Ret.Num = std::visit([&](auto lhs_val, auto rhs_val)
                {
                    using ResultType = decltype(lhs_val * rhs_val);
                    return RendorNum{static_cast<ResultType>(lhs_val) * static_cast<ResultType>(rhs_val)};
                }, lhs.Num, rhs.Num);
        return Ret;
    }
    
    /* ----------------------------------- div ---------------------------------- */
    friend Constant operator/(const Constant& lhs, const Constant& rhs)
    {
        Constant Ret{0, ConstType::CONST_NUM};
        Ret.Type = ConstType::CONST_NUM;
        Ret.Num = std::visit([&](auto lhs_val, auto rhs_val)
                {
                    using ResultType = decltype(lhs_val / rhs_val);
                    return RendorNum{static_cast<ResultType>(lhs_val) / static_cast<ResultType>(rhs_val)};
                }, lhs.Num, rhs.Num);
        return Ret;
    }

    /* ----------------------------------- pow ---------------------------------- */
    Constant pow(Constant& rhs)
    {
        Constant Ret{0, ConstType::CONST_NUM};
        Ret.Type = ConstType::CONST_NUM;

        auto rhs_intVal = std::get_if<RendorInt>(&rhs.Num);
        auto Main_intVal = std::get_if<RendorInt>(&Num);
        if 
        ((Main_intVal != nullptr) && 
        (rhs_intVal   != nullptr))
        {
            Ret.Num = Rythm_RendorPow(*Main_intVal, *rhs_intVal);
        }
        else 
        {
            Ret.Num = std::visit([&](auto Num1v, auto Num2v) 
                        { 
                            return std::pow(Num1v, Num2v); 
                        }, Num, rhs.Num);
        }
        return Ret;
    }

    /* ----------------------------------- eq ----------------------------------- */
    friend constexpr bool operator==(const Constant& lhs, const Constant& rhs)
    {
        if 
        ((lhs.Type == ConstType::CONST_NUM) &&
        (rhs.Type  == ConstType::CONST_NUM))
        {
            return lhs.Num == lhs.Num;
        }
        else if 
        ((lhs.Type == ConstType::CONST_REF) &&
        (rhs.Type  == ConstType::CONST_REF))
        {
            return *lhs.Ref == *rhs.Ref;
        }
        else if 
        ((lhs.Type == ConstType::CONST_STR) &&
        (rhs.Type  == ConstType::CONST_STR))
        {
            return *std::get_if<std::string>(&lhs.Const) == *std::get_if<std::string>(&lhs.Const);
        }
        else if 
        ((lhs.Type == ConstType::CONST_BOOL) &&
        (rhs.Type  == ConstType::CONST_BOOL))
        {
            return *std::get_if<bool>(&lhs.Const) == *std::get_if<bool>(&lhs.Const);
        }
        else 
        {
            throw RENDOR_SEG_FAULT;
        }
        return false;
    }

    /* --------------------------------- eq_not --------------------------------- */
    friend constexpr bool operator!=(const Constant& lhs, const Constant& rhs)
    {
        if 
        ((lhs.Type == ConstType::CONST_NUM) &&
        (rhs.Type  == ConstType::CONST_NUM))
        {
            return lhs.Num != lhs.Num;
        }
        else if 
        ((lhs.Type == ConstType::CONST_REF) &&
        (rhs.Type  == ConstType::CONST_REF))
        {
            return *lhs.Ref != *rhs.Ref;
        }
        else if 
        ((lhs.Type == ConstType::CONST_STR) &&
        (rhs.Type  == ConstType::CONST_STR))
        {
            return *std::get_if<std::string>(&lhs.Const) != *std::get_if<std::string>(&lhs.Const);
        }
        else if 
        ((lhs.Type == ConstType::CONST_BOOL) &&
        (rhs.Type  == ConstType::CONST_BOOL))
        {
            return *std::get_if<bool>(&lhs.Const) != *std::get_if<bool>(&lhs.Const);
        }
        else 
        {
            throw RENDOR_SEG_FAULT;
        }
        return false;
    }

    /* ----------------------------------- gr ----------------------------------- */
    friend constexpr bool operator>(const Constant& lhs, const Constant& rhs)
    {
        if 
        ((lhs.Type == ConstType::CONST_NUM) &&
        (rhs.Type == ConstType::CONST_NUM))
        {
            return lhs.Num > rhs.Num;
        }
        else
        {
            throw RENDOR_SEG_FAULT;
        }
        return false;
    }

    /* ---------------------------------- less ---------------------------------- */
    friend constexpr bool operator<(const Constant& lhs, const Constant& rhs)
    {
        if 
        ((lhs.Type == ConstType::CONST_NUM) &&
        (rhs.Type == ConstType::CONST_NUM))
        {
            return lhs.Num < rhs.Num;
        }
        else
        {
            throw RENDOR_SEG_FAULT;
        }
        return false;
    } 

    /* ---------------------------------- gr_eq --------------------------------- */
    friend constexpr bool operator>=(const Constant& lhs, const Constant& rhs)
    {
        if 
        ((lhs.Type == ConstType::CONST_NUM) &&
        (rhs.Type == ConstType::CONST_NUM))
        {
            return lhs.Num >= rhs.Num;
        }
        else
        {
            throw RENDOR_SEG_FAULT;
        }
        return false;
    }

    /* --------------------------------- less_eq -------------------------------- */
    friend constexpr bool operator<=(const Constant& lhs, const Constant& rhs)
    {
        if 
        ((lhs.Type == ConstType::CONST_NUM) &&
        (rhs.Type == ConstType::CONST_NUM))
        {
            return lhs.Num <= rhs.Num;
        }
        else
        {
            throw RENDOR_SEG_FAULT;
        }
        return false;
    }
};

#endif // CONSTANT_HPP