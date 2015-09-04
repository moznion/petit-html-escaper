#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef __SSE4_2__
#ifdef _MSC_VER
#include <nmmintrin.h>
#else
#include <x86intrin.h>
#endif
#endif

#define RANGES "\">``{}"
#define RANGE_SIZE 6

char *phe_escape_html(const char *p, size_t size) {
    const __m128i ranges = _mm_loadu_si128((const __m128i*) RANGES);

    char *buf;
    buf = (char *)malloc(sizeof(char) * size * 6 + 1);

    int buf_index = 0;
    int cursor = 0;

    __m128i v;
    do {
        v = _mm_loadu_si128((const __m128i*) p);
        cursor = _mm_cmpestri(ranges, RANGE_SIZE, v, 16, _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);
        if (cursor != 16) {
            memcpy(&buf[buf_index], p, cursor);
            buf_index += cursor;
            const char c = p[cursor];
            switch (c) {
                case '&':
                    memcpy(&buf[buf_index], "&amp;", 5);
                    buf_index += 5;
                    break;
                case '>':
                    memcpy(&buf[buf_index], "&gt;", 4);
                    buf_index += 4;
                    break;
                case '<':
                    memcpy(&buf[buf_index], "&lt;", 4);
                    buf_index += 4;
                    break;
                case '"':
                    memcpy(&buf[buf_index], "&quot;", 6);
                    buf_index += 6;
                    break;
                case '\'':
                    memcpy(&buf[buf_index], "&#39;", 5);
                    buf_index += 5;
                    break;
                case '`':
                    // For IE. IE interprets back-quote as valid quoting characters
                    // ref: https://rt.cpan.org/Public/Bug/Display.html?id=84971
                    memcpy(&buf[buf_index], "&#96;", 5);
                    buf_index += 5;
                    break;
                case '{':
                    // For javascript templates (e.g. AngularJS and such javascript frameworks)
                    // ref: https://github.com/angular/angular.js/issues/5601
                    memcpy(&buf[buf_index], "&#123;", 6);
                    buf_index += 6;
                    break;
                case '}':
                    // For javascript templates (e.g. AngularJS and such javascript frameworks)
                    // ref: https://github.com/angular/angular.js/issues/5601
                    memcpy(&buf[buf_index], "&#125;", 6);
                    buf_index += 6;
                    break;
                default:
                    memcpy(&buf[buf_index], &c, 1);
                    buf_index += 1;
            }

            const int next = cursor + 1;
            p += next;
            size -= next;
            continue;
        }

        memcpy(&buf[buf_index], p, 16);
        buf_index += 16;
        p += 16;
        size -= 16;
    } while((int) size > 0);

    return buf;
}

