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

void phe_escape_html(char *dst, size_t dst_size, const char *input, size_t input_size) {
    const __m128i ranges = _mm_loadu_si128((const __m128i*) RANGES);

    int dst_index = 0;
    int cursor = 0;

    __m128i v;
    do {
        v = _mm_loadu_si128((const __m128i*) input);
        cursor = _mm_cmpestri(ranges, RANGE_SIZE, v, 16, _SIDD_LEAST_SIGNIFICANT | _SIDD_CMP_RANGES | _SIDD_UBYTE_OPS);
        if (cursor != 16) {
            memcpy(&dst[dst_index], input, cursor);
            dst_index += cursor;
            const char c = input[cursor];
            switch (c) {
                case '&':
                    memcpy(&dst[dst_index], "&amp;", 5);
                    dst_index += 5;
                    break;
                case '>':
                    memcpy(&dst[dst_index], "&gt;", 4);
                    dst_index += 4;
                    break;
                case '<':
                    memcpy(&dst[dst_index], "&lt;", 4);
                    dst_index += 4;
                    break;
                case '"':
                    memcpy(&dst[dst_index], "&quot;", 6);
                    dst_index += 6;
                    break;
                case '\'':
                    memcpy(&dst[dst_index], "&#39;", 5);
                    dst_index += 5;
                    break;
                case '`':
                    // For IE. IE interprets back-quote as valid quoting characters
                    // ref: https://rt.cpan.org/Public/Bug/Display.html?id=84971
                    memcpy(&dst[dst_index], "&#96;", 5);
                    dst_index += 5;
                    break;
                case '{':
                    // For javascript templates (e.g. AngularJS and such javascript frameworks)
                    // ref: https://github.com/angular/angular.js/issues/5601
                    memcpy(&dst[dst_index], "&#123;", 6);
                    dst_index += 6;
                    break;
                case '}':
                    // For javascript templates (e.g. AngularJS and such javascript frameworks)
                    // ref: https://github.com/angular/angular.js/issues/5601
                    memcpy(&dst[dst_index], "&#125;", 6);
                    dst_index += 6;
                    break;
                default:
                    memcpy(&dst[dst_index], &c, 1);
                    dst_index += 1;
            }

            const int next = cursor + 1;
            input += next;
            input_size -= next;
            continue;
        }

        memcpy(&dst[dst_index], input, 16);
        dst_index += 16;
        input += 16;
        input_size -= 16;
    } while((int) input_size > 0);
}

