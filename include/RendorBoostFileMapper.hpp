#ifndef RENNDOR_BOOST_FILE_MAPPING_HPP
#define RENNDOR_BOOST_FILE_MAPPING_HPP
#include <ios>
#include <iostream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>

// Code is from answer to this question: https://stackoverflow.com/questions/52699244/c-fast-way-to-load-large-txt-file-in-vectorstring
class LineIterator
    : public boost::iterator_facade<
            LineIterator,
            boost::iterator_range<char const*>,
            boost::iterators::forward_traversal_tag,
            boost::iterator_range<char const*>
            >
{
    char const *p_, *q_;
    boost::iterator_range<char const*> dereference() const { return {p_, this->next()}; }
    bool equal(LineIterator b) const { return p_ == b.p_; }
    void increment() { p_ = this->next(); }
    char const* next() const { auto p = std::find(p_, q_, '\n'); return p + (p != q_); }
    friend class boost::iterator_core_access;

public:
    LineIterator(char const* begin, char const* end) : p_(begin), q_(end) {}
};

namespace RendorMapping
{
    inline boost::iterator_range<LineIterator> crange(boost::interprocess::mapped_region const& r) {
        auto p = static_cast<char const*>(r.get_address());
        auto q = p + r.get_size();
        return {LineIterator{p, q}, LineIterator{q, q}};
    }
}


inline std::ostream& operator<<(std::ostream& s, boost::iterator_range<char const*> const& line) {
    return s.write(line.begin(), static_cast<std::streamsize>(line.size()));
}

#endif // RENNDOR_BOOST_FILE_MAPPING_HPP
