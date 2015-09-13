#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "petithtmlescaper.h"

#define INPUT \
"<body>\n" \
"<div>\n" \
"    <h1>Example Domain</h1>\n" \
"    <p>This domain is established to be used for illustrative examples in documents. You may use this\n" \
"    domain in examples without prior coordination or asking for permission.</p>\n" \
"    <p><a href=\"http://www.iana.org/domains/example\">More information...</a></p>\n" \
"</div>\n" \
"</body>"

static void simple_escape_html(char *dst, size_t dst_size, const char *input, size_t input_size);

int main(void) {
    const long input_size = strlen(INPUT);

    char *res;
    int size_of_res = sizeof(char) * strlen(INPUT) * 6 + 1;
    res = (char *)malloc(size_of_res);

    float phe_elapsed;
    {
        time_t begin = clock();
        for (int i = 0; i < 10000000; i++) {
            phe_escape_html(res, size_of_res, INPUT, input_size);
        }
        time_t end = clock();
        phe_elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
        printf("petit-html-escaper: %f [sec]\n", phe_elapsed);
    }

    float simple_elapsed;
    {
        time_t begin = clock();
        for (int i = 0; i < 10000000; i++) {
            simple_escape_html(res, size_of_res, INPUT, input_size);
        }
        time_t end = clock();
        simple_elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
        printf("simple-impl: %f [sec]\n", simple_elapsed);
    }

    printf("petit-html-escaper is faster \x1b[1m%f%%\x1b[0m than simple implementation\n", simple_elapsed / phe_elapsed * 100);

    free(res);

    return 0;
}

static void simple_escape_html(char *dst, size_t dst_size, const char *input, size_t input_size) {
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
}

