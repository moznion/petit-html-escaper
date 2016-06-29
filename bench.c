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
"aaa&bbb'ccc`ddd{eee}\n" \
"</body>"

static void simple_escape_html(char *dst, const char *input, size_t input_size);

int main(void) {
    const long input_size = strlen(INPUT);

    char *res;
    int size_of_res = sizeof(char) * strlen(INPUT) * 6 + 1;
    res = (char *)malloc(size_of_res);

    float phe_elapsed;
    {
        time_t begin = clock();
        for (int i = 0; i < 10000000; i++) {
            phe_escape_html(res, INPUT, input_size);
        }
        time_t end = clock();
        phe_elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
        printf("petit-html-escaper: %f [sec]\n", phe_elapsed);
    }

    float simple_elapsed;
    {
        time_t begin = clock();
        for (int i = 0; i < 10000000; i++) {
            simple_escape_html(res, INPUT, input_size);
        }
        time_t end = clock();
        simple_elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
        printf("simple-impl: %f [sec]\n", simple_elapsed);
    }

    printf("petit-html-escaper is faster \x1b[1m%f%%\x1b[0m than simple implementation\n", simple_elapsed / phe_elapsed * 100);

    free(res);

    return 0;
}

// Author: @mattn
static void simple_escape_html(char *dst, const char *input, size_t input_size) {
    const char *ptr = input, *end = input + input_size;
#define STRCPY(d,s) memcpy(d,s,sizeof(s)-1); d += sizeof(s)-1;
    while (ptr < end) {
        if (*ptr < 48) {
            switch (*ptr) {
                case '&': STRCPY(dst,"&amp;"); break;
                case '>': STRCPY(dst,"&gt;"); break;
                case '<': STRCPY(dst,"&lt;"); break;
                case '"': STRCPY(dst,"&quot;"); break;
                case '\'': STRCPY(dst,"&#39;"); break;
                case '`': STRCPY(dst,"&#96;"); break;
                case '{': STRCPY(dst,"&#123;"); break;
                case '}': STRCPY(dst,"&#125;"); break;
                default: *dst++ = *ptr; break;
            }
            ptr++;
        } else
            *dst++ = *ptr++;
    }
#undef STRCPY
    *dst++ = 0;
}

