#pragma once

#include <iostream>
#include <vector>
#include <algorithm>


template <typename const_iterator>
class IteratorRange
{

public:

    IteratorRange(const_iterator begin, const_iterator end) : step1(begin), step2(end) {}

    const_iterator begin() const { return step1; }

    const_iterator end() const { return step2; }

    size_t size() const { return (distance(step1, step2)); }

private:
    const_iterator step1; /////можно поправить на begin и end. Шаг тк шагаем по страницам
    const_iterator step2;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& span) {
    for (Iterator it = span.begin(); it != span.end(); ++it) {
        out << *it;
    }
    return out;
}


template <typename const_iterator>
class Paginator {
public:
    Paginator(const_iterator begin, const_iterator end, size_t page_size)
    {
        size_t match = distance(begin, end);

        do {
            const size_t page_step = std::min(page_size, match);
            const const_iterator page_end = next(begin, page_step);
            pages.push_back({ begin, page_end });
            match -= page_step;
            begin = page_end;
        }

        while (match > 0);

    }

    auto begin() const { return pages.begin(); }

    auto end() const { return pages.end(); }

    size_t size() const { return pages.size(); }

private:
    std::vector<IteratorRange<const_iterator>> pages;

};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(c.cbegin(), c.cend(), page_size);
}