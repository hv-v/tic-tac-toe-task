#ifndef UTILS_H
#define UTILS_H

#include <list>
#include <optional>
#include <memory>

template<typename T>
class PlayerList
{
    using ImplType = typename std::list<T>;
    using ImplTypeIt = typename ImplType::iterator;
    using ImplTypeReference = typename ImplType::const_reference;

public:
    template<typename ...Args>
    ImplTypeReference emplaceBack(Args &&...args) {
        const auto ref = m_impl.emplace_back(std::move(args)...);
        m_current = m_impl.end();
        return ref;
    }

    void push_back(T value) {
        m_impl.push_back(value);
        reset();
    }

    ImplTypeIt next() {
        ++m_current;
        return current();
    }

    ImplTypeIt current() {
        assert(!m_impl.empty());

        if (m_current == m_impl.end()) {
            m_current = m_impl.begin();
        }

        return m_current;
    }

    ImplTypeIt erase(int index) {
        const auto it = std::next(m_impl.begin(), index);
        const auto res = m_impl.erase(it);
        reset();

        return res;
    }

    void reset() {
        m_current = m_impl.end();
    }

    size_t size() const {
        return m_impl.size();
    }

private:

    ImplType m_impl;
    ImplTypeIt m_current;
};

#endif
