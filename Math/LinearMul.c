#include <stdio.h>

#include <emmintrin.h>

float* SSE_linearmulf(const float *a1, const float *a2, float *r)
{
    /* Loading 4 packed single float point values from memory into a XMM register */
    
    // First array
    __m128 arr_one = _mm_loadr_ps(a1);
    // Second array
    __m128 arr_two = _mm_loadr_ps(a2);

    // Multiply both array values (4 floats)
    // mulps xmm, xmm
    const register __m128 result_value = _mm_mul_ps(arr_one, arr_two);

    /*
        [0] [1] [2] [3]
        *
        [0] [1] [2] [3]
        =
        [[3] * [3]] [[2] * [2]] [[1] * [1]] * [[0] * [0]]   
    */

    // Store the result in reverse order for preserver the semantic
    _mm_storer_ps(r, result_value);

    return r;
}

/* The compiler result code:

00000000000011f0 <SSE_linearmulf>:
    11f0:	0f 28 07             	movaps xmm0,XMMWORD PTR [rdi]
    11f3:	48 89 d0             	mov    rax,rdx
    11f6:	0f 28 c8             	movaps xmm1,xmm0
    11f9:	0f c6 c8 1b          	shufps xmm1,xmm0,0x1b
    11fd:	0f 28 06             	movaps xmm0,XMMWORD PTR [rsi]
    1200:	0f c6 c0 1b          	shufps xmm0,xmm0,0x1b
    1204:	0f 59 c1             	mulps  xmm0,xmm1
    1207:	0f c6 c0 1b          	shufps xmm0,xmm0,0x1b
    120b:	0f 29 02             	movaps XMMWORD PTR [rdx],xmm0
    120e:	c3                   	ret    
*/

int main()
{
    const float vec_first[]  = {10.34f, 3.145f,   32.43f, 122.f};
    const float vec_second[] = {2.4f,   12.2343f, 12.f,   1.000121f};

    printf("1: %f * %f; 2: %f * %f; 3: %f * %f; 4: %f * %f\n",
        vec_first[0], vec_second[0], vec_first[1], vec_second[1],
        vec_first[2], vec_second[2], vec_first[3], vec_second[3]
    );

    float vec_result[4];

    SSE_linearmulf(vec_first, vec_second, vec_result);

    printf("1: %f; 2: %f; 3: %f; 4: %f\n", 
        vec_result[0], vec_result[1], vec_result[2], vec_result[3]
    );

    return 0;
}