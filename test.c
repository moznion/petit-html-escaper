#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "picotest.h"
#include "petithtmlescaper.h"

void ok_escape (const char *src, const char *expected);

void chars (void);
void mixed (void);

int main (void) {
    subtest("chars", chars);
    subtest("mixed", mixed);
    return done_testing();
}

void ok_escape (const char *src, const char *expected) {
    char *res;
    int size_of_res = sizeof(char) * 6 + 1;

    res = (char *)malloc(size_of_res);
    phe_escape_html(res, size_of_res, src, strlen(src));

    bool pass = strncmp(res, expected, strlen(expected)) == 0;
    ok(pass);

    free(res);
}

void chars (void) {
    ok_escape("&",  "&amp;");
    ok_escape(">",  "&gt;");
    ok_escape("<",  "&lt;");
    ok_escape("\"", "&quot;");
    ok_escape("'",  "&#39;");
    ok_escape("`",  "&#96;");
    ok_escape("{",  "&#123;");
    ok_escape("}",  "&#125;");
    ok_escape("no included html special chars", "no included html special chars");
}

void mixed (void) {
    ok_escape(
            "'`><script type=\"text/javascript\">{{alert(1)}}</script>&",
            "&#39;&#96;&gt;&lt;script type=&quot;text/javascript&quot;&gt;&#123;&#123;alert(1)&#125;&#125;&lt;/script&gt;&amp;"
            );
}

