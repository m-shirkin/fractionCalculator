#pragma once

#include "std.h"

#ifdef __wasm__
  #define WASM_EXPORT(name) __attribute__((used, export_name(#name)))
  #define WASM_IMPORT(name) __attribute__((import_module("env"), import_name(#name))) extern "C"
#else
  #define WASM_EXPORT(name)
  #define WASM_IMPORT(name)
#endif

static char input[64];
WASM_EXPORT(input)
char* input_();

static char output[1024];

using u64 = unsigned long long;
static_assert(sizeof(u64) == 8);
using u32 = unsigned;
static_assert(sizeof(u32) == 4);
using i64 = long long;

WASM_IMPORT(log)
void js_log(u64 x);

template<typename T>
T ptoT(char* p) {
    T res;
    char* r = reinterpret_cast<char*>(&res);
    char* rend = r + sizeof(T);
    for (; r != rend; ++p, ++r) {
        *r = *p;
    }
    return res;
}

template<typename T>
char* Ttop(char* p, T const& v) {
    char const* r = reinterpret_cast<char const*>(&v);
    char const* rend = r + sizeof(T);
    for (; r != rend; ++p, ++r) {
        *p = *r;
    }
    return p;
}

struct div64 {
    u64 quo;
    u64 rem;
};

// Return a*b/m and a*b%m, avoiding overflow
// a, b < m
div64 multiply_div(u64 a, u64 b, u64 m);

// Return (a+b)/m and (a+b)%m, avoiding overflow
// a, b < m
div64 add_div(u64 a, u64 b, u64 m);

u64 power_mod(u64 a, u64 pow, u64 m);