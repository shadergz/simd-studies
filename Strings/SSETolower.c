#include <stdio.h>
#include <emmintrin.h>
#include <string.h>

/* This function doesn't support spaces or numbers, otherwise undefined behaviours will happens */
size_t SSE_tolower(char *lower, char *upper)
{
    size_t n;
    unsigned long lowerlen = strlen(upper);

    const register __m128i ascii_lower = _mm_set1_epi8(32);

    for (n = 0; lowerlen >= sizeof(__m128i); lowerlen -= sizeof(__m128i)) {
        _mm_storeu_si128((__m128i*)lower, _mm_add_epi8(*((__m128i*)upper), ascii_lower));
        upper += sizeof(__m128i);
        lower += sizeof(__m128i);

        n += sizeof(__m128i);
    }

    n += lowerlen;
    for (; lowerlen > 0; lowerlen--)
        *lower++ = (*upper++) + 32;

    *lower = '\0';

    return n;
}

int main()
{
    char upper_str[] = "AGSDHBABSDBAUSBDYUASDUHDUIAODHQASDASDASDASDASDASDSDAYDUBHSABHJASYGUDBHEGYDEGYUDSBHEWGYEWGFYSEWGFYEGSDBHUKESFYIDSFYUGDSSOMERANDOMSTRINGKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKAUDGSDBHSBDHAHDBSHDBAHDBHASBDHBSAHDBAHSDBASBHDHASBDHBSAHDBASBDHABSHDBSJDHASBDHBASKHDBSBDASBGDHASVGDYTASFDFSDFAASDSFDASFDFASFDTFTFASFFAFTTFATYDATFADFTAS";
    char lower_str[sizeof(upper_str)];
    printf("Upper string: %s\n", upper_str);
    SSE_tolower(lower_str, upper_str);
    printf("After tolower: %s\n", lower_str);


    return 0;
}
