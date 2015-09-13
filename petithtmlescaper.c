#include <string.h>
#ifdef __SSE4_2__
#ifdef _MSC_VER
#include <nmmintrin.h>
#else
#include <x86intrin.h>
#endif
#endif
#include "petithtmlescaper.h"

#define RANGES "\">``{}"
#define RANGE_SIZE 6

static inline void _phe_escape_html(char *dst, const char *input, size_t input_size);

void phe_escape_html(char *dst, const char *input, size_t input_size) {
    _phe_escape_html(dst, input, input_size);
}

static inline void _phe_escape_html(char *dst, const char *input, size_t input_size) {
#if __SSE4_2__
    const __m128i ranges = _mm_loadu_si128((const __m128i*) RANGES);

    int cursor = 0;

    int left = 0;

    __m128i v;
    do {
        v = _mm_loadu_si128((const __m128i*) &input[left]);
        cursor = _mm_cmpestri(ranges, RANGE_SIZE, v, 16, _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);
        if (cursor != 16) {
            const int len = left + cursor;
            if (input_size + len - 17 == 0) {
                break;
            }

            memcpy(dst, input, len);
            dst += len;
            input += left;
            const char c = input[cursor];
            switch (c) {
                case '&':
                    memcpy(dst, "&amp;", 5);
                    dst += 5;
                    break;
                case '>':
                    memcpy(dst, "&gt;", 4);
                    dst += 4;
                    break;
                case '<':
                    memcpy(dst, "&lt;", 4);
                    dst += 4;
                    break;
                case '"':
                    memcpy(dst, "&quot;", 6);
                    dst += 6;
                    break;
                case '\'':
                    memcpy(dst, "&#39;", 5);
                    dst += 5;
                    break;
                case '`':
                    // For IE. IE interprets back-quote as valid quoting characters
                    // ref: https://rt.cpan.org/Public/Bug/Display.html?id=84971
                    memcpy(dst, "&#96;", 5);
                    dst += 5;
                    break;
                case '{':
                    // For javascript templates (e.g. AngularJS and such javascript frameworks)
                    // ref: https://github.com/angular/angular.js/issues/5601
                    memcpy(dst, "&#123;", 6);
                    dst += 6;
                    break;
                case '}':
                    // For javascript templates (e.g. AngularJS and such javascript frameworks)
                    // ref: https://github.com/angular/angular.js/issues/5601
                    memcpy(dst, "&#125;", 6);
                    dst += 6;
                    break;
                default:
                    memcpy(dst, &c, 1);
                    dst += 1;
            }

            const int next = cursor + 1;
            input += next;
            input_size -= next;
            left = 0;
            continue;
        }

        left += 16;
        input_size -= 16;
    } while((int) input_size > 0);

    if (left > 0) {
        const int len = left + cursor;
        memcpy(dst, input, len);
        dst += len;
    }

    *dst++ = *"\0";
#else
    for (int i = 0; i < input_size; i++) {
        const char c = *(input++);
        switch (c) {
            case '&':
                memcpy(dst, "&amp;", 5);
                dst += 5;
                break;
            case '>':
                memcpy(dst, "&gt;", 4);
                dst += 4;
                break;
            case '<':
                memcpy(dst, "&lt;", 4);
                dst += 4;
                break;
            case '"':
                memcpy(dst, "&quot;", 6);
                dst += 6;
                break;
            case '\'':
                memcpy(dst, "&#39;", 5);
                dst += 5;
                break;
            case '`':
                // For IE. IE interprets back-quote as valid quoting characters
                // ref: https://rt.cpan.org/Public/Bug/Display.html?id=84971
                memcpy(dst, "&#96;", 5);
                dst += 5;
                break;
            case '{':
                // For javascript templates (e.g. AngularJS and such javascript frameworks)
                // ref: https://github.com/angular/angular.js/issues/5601
                memcpy(dst, "&#123;", 6);
                dst += 6;
                break;
            case '}':
                // For javascript templates (e.g. AngularJS and such javascript frameworks)
                // ref: https://github.com/angular/angular.js/issues/5601
                memcpy(dst, "&#125;", 6);
                dst += 6;
                break;
            default:
                memcpy(dst, &c, 1);
                dst += 1;
        }
    }
    *dst++ = *"\0";
#endif
}
