#pragma once

#include"common.h"

u64 GCD(u64 a, u64 b);
u64 LCM(u64 a, u64 b);
std::pair<u64, u64> modular_inverse(u64 x, u64 m);
std::pair<u64, u64> chinese_remainder(u64 p, u64 a, u64 q, u64 b);
