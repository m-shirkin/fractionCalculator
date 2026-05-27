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
    if (a < b) return multiply_div(b, a, m);
    auto combine_divs = [](div64 const& a, div64 const& b, u64 m) {
        div64 mt = add_div(a.rem, b.rem, m);
        return div64{a.quo + b.quo + mt.quo, mt.rem};
    };
    if (a >= m) {
        return combine_divs({a/m*b, 0}, multiply_div(a%m, b, m), m);
    }
    if (b == 1) return {0, a};
    auto res = multiply_div(a, b/2, m);
    res = combine_divs(res, res, m);
    if (b&1) {
        res = combine_divs(res, {0, a}, m);
    }
    return res;
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