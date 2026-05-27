#pragma once

#include"common.h"

struct DigitStream {
    u64 num, den, rem;
    u64 pos;
    bool dir; // dir = 0 is before the dot, dir = 1 is after the dot
};

WASM_EXPORT(streamSeek)
void stream_seek(DigitStream* stream, u64 pos, bool dir);

// Creates the stream and seeks to start
WASM_EXPORT(streamCreate)
DigitStream* stream_create(u64 num, u64 den);

WASM_EXPORT(streamRead)
char* stream_read(DigitStream* stream, int num);
