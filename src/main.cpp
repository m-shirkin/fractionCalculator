#include"common.h"

#include"gcd.h"
#include"factors.h"

char* input_() { return input; }

// div64 multiply_div(u64 a, u64 b, u64 m) {
//     auto mul = __int128(a) * b;
//     return {u64(mul / m), u64(mul % m)};
// }

div64 multiply_div(u64 a, u64 b, u64 m) {
    if (m < (1ull << 32)) {
        return { a * b / m, a * b % m };
    }
    struct {
        union {
            u64 v64[2];
            u32 v32[4];
        };
    } rem = {};
    auto add = []<bool sign>(u32 * s, u32 const* x, int xlen) {
        u32 carry = 0;
        int i;
        for (i = 0; i < xlen; ++i) {
            u64 acc = s[i];
            if constexpr (sign) {
                acc -= x[i];
                acc -= carry;
            } else {
                acc += x[i];
                acc += carry;
            }
            s[i] = u32(acc);
            carry = bool(acc >> 32);
        }
        if (carry) {
            if constexpr (sign) {
                while (s[i] == 0) {
                    s[i++] = u32(-1);
                }
                --s[i];
            } else {
                do {
                    ++s[i];
                } while (s[i++] == 0);
            }
        }
    };
    auto u64tou32 = [](u64 const& x) {
        return reinterpret_cast<u32 const*>(&x);
    };
    auto multiply = [&]<bool sign>(u64 x, u64 y, auto& res) {
        auto x32 = u64tou32(x);
        auto y32 = u64tou32(y);
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                u64 prod = u64(x32[i]) * y32[j];
                auto prod32 = u64tou32(prod);
                add.operator()<sign>(res.v32 + i + j, prod32, 2);
            }
        }
    };
    multiply.operator()<0>(a, b, rem);
    u64 quo = 0;
    while (rem.v64[1] > 0) {
        static constexpr double two64 = double(1ull << 32) * double(1ull << 32);
        static constexpr double two96 = two64 * double(1ull << 32);
        u64 aquo;
        if (rem.v32[3] > 0) {
            aquo = two96 * rem.v32[3] / m;
        } else {
            aquo = two64 * rem.v32[2] / m;
        }
        quo += aquo;
        multiply.operator()<1>(aquo, m, rem);
    }
    quo += rem.v64[0] / m;
    return { quo, rem.v64[0] % m };
}

div64 add_div(u64 a, u64 b, u64 m) {
    if (a < m - b) {
        return {0, a + b};
    } else {
        return {1, a - (m - b)};
    }
}

u64 power_mod(u64 a, u64 pow, u64 m) {
    if (pow == 0) return 1;
    if (pow == 1) return a % m;
    u64 res = power_mod(a, pow/2, m);
    res = multiply_div(res, res, m).rem;
    if (pow & 1) res = multiply_div(res, a % m, m).rem;
    return res;
}

// Returns:
// 1. Number of decimals before the point (at least one)
// 2. Number of non-repeating decimals after the point
// 3. Period length
WASM_EXPORT(fractionInfo)
char* fraction_info(u64 num, u64 den) {
    u64 int_digits = 1;
    for (u64 m = 10; num / m >= den; m *= 10) ++int_digits;
    num %= den;
    u64 gcd = GCD(num, den);
    num /= gcd; den /= gcd;
    u64 period_digits = 0, nonrep_digits = 0;
    if (den > 1) {
        auto f = factorize(den);
        int p2 = 0, p5 = 0;
        for (auto const& [p, k]: f) {
            if (p == 2) p2 = k;
            else if (p == 5) p5 = k;
            else {
                period_digits = LCM(period_digits == 0 ? 1 : period_digits, multiplivative_order(10, p, k));
            }
        }
        nonrep_digits = p2 > p5 ? p2 : p5;
    } else {
        period_digits = 0;
    }
    char* o = output;
    o = Ttop(o, int_digits);
    o = Ttop(o, nonrep_digits);
    o = Ttop(o, period_digits);
    return output;
}

WASM_EXPORT(gcd)
char* GCD_exp(u64 a, u64 b) {
    Ttop(output, GCD(a, b));
    return output;
}