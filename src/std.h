#pragma once

namespace std {
    template<typename T1, typename T2>
    struct pair {
        T1 first;
        T2 second;
    };

    template<typename T, int max_sz = 64>
    struct vector {
        T v[max_sz];
        int sz = 0;

        void push_back(T const& t) {
            v[sz++] = t;
        }

        template<typename... Args>
        void emplace_back(Args const&... args) {
            push_back(T(args...));
        }

        T* begin() {
            return v;
        }
        T* end() {
            return v + sz;
        }
        T& back() {
            return v[sz - 1];
        }

        T const* begin() const {
            return v;
        }
        T const* end() const {
            return v + sz;
        }
        T const& back() const {
            return v[sz - 1];
        }

        bool empty() const {
            return sz == 0;
        }
        int size() const {
            return sz;
        }

        void pop_back() {
            --sz;
        }
    };

    template<typename T>
    void swap(T& x, T& y) {
        T t = x;
        x = y;
        y = t;
    }

    template<typename TIt1, typename TIt2>
    void sort(TIt1 it1, TIt2 it2) {
        for (auto it = it1; it != it2; ++it) {
            for (auto jt = it;;) {
                if (++jt == it2) break;
                if (*it > *jt) {
                    std::swap(*it, *jt);
                }
            }
        }
    }
}
