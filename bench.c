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

    int i;
    char *res;
    int size_of_res = sizeof(char) * strlen(INPUT) * 6 + 1;
    res = (char *)malloc(size_of_res);

    float phe_elapsed;
    {
        time_t begin = clock();
        for (i = 0; i < 10000000; i++) {
            phe_escape_html(res, INPUT, input_size);
        }
        time_t end = clock();
        phe_elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
        printf("petit-html-escaper: %f [sec]\n", phe_elapsed);
    }

    float simple_elapsed;
    {
        time_t begin = clock();
        for (i = 0; i < 10000000; i++) {
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

static void simple_escape_html(char *dst, const char *input, size_t input_size) {
    const char *ptr = input, *end = input + input_size;
    static int pp[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,8};
    static char* dd[] = {NULL, "&amp;", "&gt;", "&lt;", "&quot;", "&#39;", "&#96;", "&#123;", "&#125;"};
    static int dl[] = {0, 5, 4, 4, 6, 5, 5, 6, 6 };
#define _ESC_AND_COPY(d,s,n) { memcpy(d,s,n); d += n; }
    while (ptr < end) {
        unsigned char c = *ptr;
        int i = c < 126 ? pp[c] : 0;
        if (i == 0) *dst++ = c;
        else _ESC_AND_COPY(dst, dd[i], dl[i]);
        ptr++;
    }
#undef _ESC_AND_COPY
    *dst++ = 0;
}

