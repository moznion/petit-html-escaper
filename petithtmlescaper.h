#ifndef petithtmlparser_h
#define petithtmlparser_h

#ifdef __cplusplus
extern "C" {
#endif

void phe_escape_html(char *dst, size_t dst_size, const char *input, size_t input_size);

#ifdef __cplusplus
}
#endif
#endif

