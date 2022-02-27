#include <string_view>
using std::string_view;

#include "RendorInterpreter/Resources/RendorGCData.hpp"
using rengc::RendorGCData;
using rengc::RendorStack;


/* -------------------------------------------------------------------------- */
/*                           RendorGCData Functions                           */
/* -------------------------------------------------------------------------- */
template <typename T>
bool RendorGCData<T>::black_empty()
{
    return m_BlackObjects.empty();
}

template <typename T>
bool RendorGCData<T>::grey_empty()
{
    return m_GreyObjects.empty();
}

template <typename T>
bool RendorGCData<T>::white_empty()
{
    return m_WhiteObjects.empty();
}

template <typename T>
void RendorGCData<T>::promote(std::string_view Object)
{
    if (m_BlackObjects.contains(Object))
    {
        return;
    }
    else if (m_WhiteObjects.contains(Object))
    {
        m_BlackObjects.insert(Object, std::move(m_WhiteObjects[Object]));
        m_WhiteObjects.erase(Object);
    }
    else if (m_GreyObjects.contains(Object))
    {
        m_BlackObjects.insert(Object, std::move(m_GreyObjects[Object]));
        m_GreyObjects.erase(Object);
    }
}

template <typename T>
void RendorGCData<T>::demote(std::string_view Object)
{
    if (m_WhiteObjects.contains(Object))
    {
        return;
    }
    if (m_BlackObjects.contains(Object))
    {
        m_WhiteObjects.insert(Object, std::move(m_BlackObjects[Object]));
        m_BlackObjects.erase(Object);
    }
    else if (m_GreyObjects.contains(Object))
    {
        m_WhiteObjects.insert(Object, std::move(m_GreyObjects[Object]));
        m_GreyObjects.erase(Object);
    }
}

template <typename T>
void RendorGCData<T>::add(std::string_view ObjectName, T Object)
{
    if 
    ((m_BlackObjects.contains(ObjectName)) ||
    (m_WhiteObjects.contains(ObjectName))  ||
    (m_GreyObjects.contains(ObjectName)))
    {
        return;
    }
    else
    {
        m_WhiteObjects.insert(ObjectName, Object);
    }
}

template <typename T>
void RendorGCData<T>::clean()
{
    m_WhiteObjects.clear();
}

template <typename T>
void RendorGCData<T>::promote_grey()
{
    for ([Name, Object] : m_GreyObjects)
    {
        m_BlackObjects.insert(Object, std::move(Object));
    }
}

/* -------------------------------------------------------------------------- */
/*                                Rendor Stack                                */
/* -------------------------------------------------------------------------- */

template <typename T, size_t m_size>
size_t RendorStack<T, m_size>::size()
{
    return m_Size;
}

template <typename T, size_t m_size>
bool RendorStack<T, m_size>::empty()
{
    return m_Size == 0;
}

template <typename T, size_t m_size>
void RendorStack<T, m_size>::push(T Object)
{
    
}

