#pragma once

#include"common.h"

using Factorization = std::vector<std::pair<u64, int>>;

Factorization merge_factors(Factorization const& x, Factorization const& y);
Factorization factorize(u64 x);
// Multiplicative order of x mod p^k
u64 multiplivative_order(u64 x, u64 p, u64 k);