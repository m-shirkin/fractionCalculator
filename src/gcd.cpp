#include"gcd.h"

template<typename Fn>
void pairwise_apply(Fn const& fn) {}

template<typename Fn, typename T1, typename T2, typename... Args>
void pairwise_apply(Fn const& fn, T1& t1, T2& t2, Args&... args) {
    fn(t1, t2);
    pairwise_apply(fn, args...);
}

template<typename... Args>
auto GCD_Impl(u64 a, u64 b, Args... args) {
    if (b == 0 || a < b && a != 0) {
        pairwise_apply([](auto& a, auto& b) {std::swap(a, b);}, a, b, args...);
    }
    if (a != 0) {
        for (;;) {
            if (b > (a >> 1)) {
                if (b == a) break;
                pairwise_apply(
                    [](auto& a, auto& b) {
                        auto t = a;
                        a = b;
                        b = t - b;
                    },
                    a, b, args...
                );
            } else {
                u64 q = a / b;
                if (a - q * b == 0) {
                    break;
                }
                pairwise_apply(
                    [q](auto& a, auto& b) {
                        auto t = a;
                        a = b;
                        b = t - q*b;
                    },
                    a, b, args...
                );
            }
        }
    }
    struct RetT {
        u64 v[1 + sizeof...(Args) / 2];
    };
    RetT res;
    int i = 0;
    pairwise_apply(
        [&res, &i](auto const&, auto const& b) {
            res.v[i++] = b;
        },
        a, b, args...
    );
    return res;
}

u64 GCD(u64 a, u64 b) {
    return GCD_Impl(a, b).v[0];
}

u64 LCM(u64 a, u64 b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    return a / GCD(a, b) * b;
}

using i64 = long long;
static_assert(sizeof(i64) == 8);

std::pair<u64, u64> modular_inverse(u64 x, u64 m) {
    if (x == 0) return {m, 1};
    auto res = GCD_Impl(m, x, i64(0), i64(1));
    if (i64(res.v[1]) < 0) res.v[1] += m;
    return {res.v[0], res.v[1]};
}

std::pair<u64, u64> chinese_remainder(u64 p, u64 a, u64 q, u64 b) {
    auto res = GCD_Impl(p, q, 1ll, 0ll, 0ll, 1ll);
    u64 gcd = res.v[0];
    u64 lcm = p / gcd * q;
    if (a % gcd == b % gcd) {
        if (i64(res.v[1]) < 0) res.v[1] += q;
        if (i64(res.v[2]) < 0) res.v[2] += p;
        u64 ret = add_div(multiply_div(b, res.v[1], q).rem * (p / gcd), multiply_div(a, res.v[2], p).rem * (q / gcd), lcm).rem;
        return { lcm, ret };
    } else {
        return { lcm, lcm };
    }
}