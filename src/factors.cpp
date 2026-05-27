#include"factors.h"
#include"gcd.h"

Factorization MergeFactors(Factorization const& x, Factorization const& y) {
    Factorization res;
    auto itx = x.begin();
    auto ity = y.begin();
    for (;;) {
        if (itx == x.end()) {
            for (; ity != y.end(); ++ity) {
                res.push_back(*ity);
            }
            break;
        }
        if (ity == y.end()) {
            for (; itx != x.end(); ++itx) {
                res.push_back(*itx);
            }
            break;
        }
        if (itx->first < ity->first) {
            res.push_back(*itx);
            ++itx;
        } else if (itx->first == ity->first) {
            res.emplace_back(itx->first, itx->second + ity->second);
            ++itx;
            ++ity;
        } else {
            res.push_back(*ity);
            ++ity;
        }
    }
    return res;
}

constexpr u64 FIRST_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 
    67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 
    139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 
    223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 
    293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 
    383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 
    463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541
};
constexpr auto LAST_PRIME = FIRST_PRIMES[sizeof(FIRST_PRIMES) / sizeof(decltype(FIRST_PRIMES[0])) - 1];

bool MillerRabin(u64 x, int k) {
    auto xm1 = x - 1;
    int s = 0;
    auto d = xm1;
    while ((d & 1) == 0) {
        ++s;
        d >>= 1;
    }
    u64 a = 2;
    for (;k > 0; --k) {
        auto n = power_mod(a, d, x);
        if (n != 1 && n != xm1) {
            for (int i = 0; i < s; ++i) {
                n = multiply_div(n, n, x).rem;
                if (n == 1) return false;
                if (n == xm1) break;
            }
            if (n != xm1) return false;
        }
        do {
            a = a * 0x12345ull % 0x7fffffffull;
        } while ([amod = a%x, &xm1]() {return amod <= 1 || amod == xm1; }());
    }
    return true;
}

u64 Rho(u64 x) {
    for (u64 a = 1;; ++a) {
        u64 v1 = 2, v2 = 2;
        auto iter = [&](u64& v) {
            v = multiply_div(v, v, x).rem;
            v = add_div(v, a, x).rem;
        };
        for (;;) {
            iter(v1);
            iter(v2);
            iter(v2);
            if (v2 == v1) break;
            if (auto g = GCD(v1 >= v2 ? v1 - v2 : x - v2 + v1, x); g > 1) return g;
        }
    }
}

Factorization factorize(u64 x) {
    Factorization res;
    if (x == 1) return res;
    auto CheckDiv = [&](auto p) {
        if (p * p > x) return 1;
        if (x % p == 0) {
            res.emplace_back(p, 1);
            x /= p;
            while (x % p == 0) {
                ++res.back().second;
                x /= p;
            }
            if (p * p > x) return 1;
        }
        return 0;
    };
    for (auto const& p : FIRST_PRIMES) {
        if (CheckDiv(p)) {
            if (x > 1) {
                res.emplace_back(x, 1);
            }
            return res;
        }
    }
    std::vector<u64> stack;
    stack.push_back(x);
    std::vector<u64> ps;
    while (!stack.empty()) {
        auto x_ = stack.back();
        stack.pop_back();
        if (x_ <= LAST_PRIME * LAST_PRIME || MillerRabin(x_, 20)) {
            ps.push_back(x_);
        } else {
            if (x_ >= (1ull << 47)) {
                u64 p = Rho(x_);
                stack.push_back(x_ / p);
                stack.push_back(p);
            } else {
                auto dp = LAST_PRIME % 6 == 1 ? 4 : 2;
                for (auto i = LAST_PRIME + dp; i * i <= x_; i += dp) {
                    while (x_ % i == 0) {
                        ps.push_back(i);
                        x_ /= i;
                    }
                    dp = 6 - dp;
                }
                if (x_ > 1) ps.push_back(x_);
            }
        }
    }
    std::sort(ps.begin(), ps.end());
    for (auto& p : ps) {
        if (res.size() > 0 && res.back().first == p) {
            ++res.back().second;
        } else {
            res.emplace_back(p, 1);
        }
    }
    return res;
}

u64 multiplivative_order(u64 x, u64 p, u64 k) {
    u64 pk = 1;
    for (int i = 1; i < k; ++i) {
        pk *= p;
    }
    u64 ord = p*pk - pk;
    pk *= p;
    x = x % pk;
    auto f = factorize(p - 1);
    if (k >= 2) f.emplace_back(p, k - 1);
    for (auto const& [q, i] : f) {
        for (int j = 1; j <= i; ++j) {
            if (power_mod(x, ord / q, pk) == 1) {
                ord /= q;
            } else {
                break;
            }
        }
    }
    return ord;
}