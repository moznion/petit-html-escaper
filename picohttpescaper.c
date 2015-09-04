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

const char *escape(const char *p, size_t size) {
    const __m128i range = _mm_loadu_si128((const __m128i*)"\">``{}");
    const size_t range_size = 6;

    char *buf;
    buf = (char *)malloc(sizeof(char) * size * 6);

    int begin = 0;
    int cursor = 0;

    __m128i v;
    do {
        v = _mm_loadu_si128((const __m128i*)p);
        if (!_mm_cmpestra(range, range_size, v, size, 4)) {
            if (_mm_cmpestrc(range, range_size, v, size, 4)) {
                cursor = _mm_cmpestri(range, range_size, v, size, 4);
                memcpy(&buf[begin], p, cursor);
                begin = cursor;
                const char c = p[cursor];
                switch (c) {
                    case '&':
                        memcpy(&buf[begin], "&amp;", 5);
                        begin += 5;
                        break;
                    case '>':
                        memcpy(&buf[begin], "&gt;", 4);
                        begin += 4;
                        break;
                    case '<':
                        memcpy(&buf[begin], "&lt;", 4);
                        begin += 4;
                        break;
                    case '"':
                        memcpy(&buf[begin], "&quot;", 6);
                        begin += 6;
                        break;
                    case '\'':
                        memcpy(&buf[begin], "&#39;", 5);
                        begin += 5;
                        break;
                    case '`':
                        // For IE. IE interprets back-quote as valid quoting characters
                        // ref: https://rt.cpan.org/Public/Bug/Display.html?id=84971
                        memcpy(&buf[begin], "&#96;", 5);
                        begin += 5;
                        break;
                    case '{':
                        // For javascript templates (e.g. AngularJS and such javascript frameworks)
                        // ref: https://github.com/angular/angular.js/issues/5601
                        memcpy(&buf[begin], "&#123;", 6);
                        begin += 6;
                        break;
                    case '}':
                        // For javascript templates (e.g. AngularJS and such javascript frameworks)
                        // ref: https://github.com/angular/angular.js/issues/5601
                        memcpy(&buf[begin], "&#125;", 6);
                        begin += 6;
                        break;
                    default:
                        memcpy(&buf[begin], &c, 1);
                        begin += 1;

                }
                const int next = cursor + 1;
                p += (next + 1);
                size -= (next + 1);
                continue;
            }
        }
        memcpy(&buf[begin], p, 16);
        p += 16;
        size -= 16;
    } while((int)size > 0);

    return buf;
}

int main () {
    printf("%s\n", escape("saf<asdfad<ffffff", 17));
    return 1;
}
