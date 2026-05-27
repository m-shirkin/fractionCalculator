#include "stream.h"

static DigitStream g_Stream;

void stream_seek(DigitStream* stream, u64 pos, bool dir) {
    stream->pos = pos;
    stream->dir = dir;
    stream->rem = stream->num % stream->den;
    if (dir) {
        stream->rem = multiply_div(stream->rem, power_mod(10, pos, stream->den), stream->den).rem;
    }
}

DigitStream* stream_create(u64 num, u64 den) {
    g_Stream = DigitStream{num, den, num % den, 1, 0};
    u64 m = 10;
    while (g_Stream.num / m >= g_Stream.den) {
        ++g_Stream.pos;
        m *= 10;
    }
    return &g_Stream;
}

char* stream_read(DigitStream* stream, int num) {
    char* o = output;
    char* e = o;
    if (!stream->dir) {
        u64 quo = stream->num / stream->den;
        do {
            *o++ = char(quo % 10) + '0';
            quo /= 10;
        } while (quo > 0);
        for (char* o1 = output, *o2 = o - 1; o1 < o2; ++o1, --o2) {
            std::swap(*o1, *o2);
        }
        o -= stream->pos;
        if (num < stream->pos) {
            stream->pos -= num;
            num = 0;
        } else {
            num -= stream->pos;
            e += stream->pos;
            stream->pos = 0;
            stream->dir = 1;
        }
    }
    stream->pos += num;
    for (int i = 0; i < num; ++i) {
        auto div = multiply_div(stream->rem, 10ull, stream->den);
        *e++ = char(div.quo) + '0';
        stream->rem = div.rem;
    }
    return o;
}