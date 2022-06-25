#include <stdio.h>
#include <stdint.h>

#include <emmintrin.h>

void* SSE_memset(void *d, int c, uint64_t n)
{
    if (!n && !d)
        return d;
    
    /* Creating a vector data table of four integers values */
    const uint8_t vector_data[] = {c & 0xff, c & 0xff, c & 0xff, c & 0xff}; 
    const uint32_t vector_value[] = {
        *((uint32_t*)vector_data), *((uint32_t*)vector_data), 
        *((uint32_t*)vector_data), *((uint32_t*)vector_data)
    };
    const __m128i fillblock = _mm_loadu_si128((__m128i const*)vector_value);

    for (; n >= sizeof(__m128i); n -= sizeof(__m128i)) {
        _mm_storeu_si128(d, fillblock);
        d += sizeof(__m128i);
    }

    for (; n >= sizeof(uint64_t); n -= sizeof(uint64_t)) {
        uint64_t *d64 = (uint64_t*)d;
        /* Reading 64 bits from the address of vector_value and put into the d */
        *d64 = *((uint64_t*)vector_value);
        d += sizeof(uint64_t);
    }

    /* Complete the remain data need to be writed */
    for (; n > 0; n--) {
        uint8_t *d1 = (uint8_t*)d;
        *d1 = *(uint8_t*)vector_value;
        d++;
    }

    return d;
}

/* The compiler result code!
 // "Type in $": objdump -dM intel --disassemble=SSE_memset ./SSEmemset

00000000000011d0 <SSE_memset>:
    11d0:	48 89 f8             	mov    rax,rdi
    11d3:	48 09 d0             	or     rax,rdx
    11d6:	0f 84 cc 00 00 00    	je     12a8 <SSE_memset+0xd8>
    11dc:	48 c7 44 24 e8 00 00 	mov    QWORD PTR [rsp-0x18],0x0
    11e3:	00 00 
    11e5:	31 c9                	xor    ecx,ecx
    11e7:	89 f0                	mov    eax,esi
    11e9:	49 ba 00 00 00 00 ff 	movabs r10,0xffffffff00000000
    11f0:	ff ff ff 
    11f3:	48 c7 44 24 f0 00 00 	mov    QWORD PTR [rsp-0x10],0x0
    11fa:	00 00 
    11fc:	40 88 f1             	mov    cl,sil
    11ff:	88 c5                	mov    ch,al
    1201:	40 0f b6 c6          	movzx  eax,sil
    1205:	89 c6                	mov    esi,eax
    1207:	0f b7 c9             	movzx  ecx,cx
    120a:	c1 e0 18             	shl    eax,0x18
    120d:	c1 e6 10             	shl    esi,0x10
    1210:	09 f1                	or     ecx,esi
    1212:	48 8b 74 24 e8       	mov    rsi,QWORD PTR [rsp-0x18]
    1217:	09 c1                	or     ecx,eax
    1219:	89 c8                	mov    eax,ecx
    121b:	48 8b 4c 24 f0       	mov    rcx,QWORD PTR [rsp-0x10]
    1220:	4c 21 d6             	and    rsi,r10
    1223:	48 09 c6             	or     rsi,rax
    1226:	49 89 c1             	mov    r9,rax
    1229:	49 89 c0             	mov    r8,rax
    122c:	4c 21 d1             	and    rcx,r10
    122f:	49 c1 e1 20          	shl    r9,0x20
    1233:	89 f6                	mov    esi,esi
    1235:	48 09 c1             	or     rcx,rax
    1238:	4c 09 ce             	or     rsi,r9
    123b:	89 c9                	mov    ecx,ecx
    123d:	48 89 74 24 e8       	mov    QWORD PTR [rsp-0x18],rsi
    1242:	4c 09 c9             	or     rcx,r9
    1245:	48 89 4c 24 f0       	mov    QWORD PTR [rsp-0x10],rcx
    124a:	66 0f 6f 44 24 e8    	movdqa xmm0,XMMWORD PTR [rsp-0x18]
    1250:	48 83 fa 0f          	cmp    rdx,0xf
    1254:	76 72                	jbe    12c8 <SSE_memset+0xf8>
    1256:	48 8d 42 f0          	lea    rax,[rdx-0x10]
    125a:	48 83 e0 f0          	and    rax,0xfffffffffffffff0
    125e:	48 8d 44 07 10       	lea    rax,[rdi+rax*1+0x10]
    1263:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
    1268:	0f 11 07             	movups XMMWORD PTR [rdi],xmm0
    126b:	48 83 c7 10          	add    rdi,0x10
    126f:	48 39 f8             	cmp    rax,rdi
    1272:	75 f4                	jne    1268 <SSE_memset+0x98>
    1274:	83 e2 0f             	and    edx,0xf
    1277:	48 83 fa 07          	cmp    rdx,0x7
    127b:	77 33                	ja     12b0 <SSE_memset+0xe0>
    127d:	48 85 d2             	test   rdx,rdx
    1280:	74 28                	je     12aa <SSE_memset+0xda>
    1282:	41 0f b6 f0          	movzx  esi,r8b
    1286:	41 89 d0             	mov    r8d,edx
    1289:	85 d2                	test   edx,edx
    128b:	74 10                	je     129d <SSE_memset+0xcd>
    128d:	31 c9                	xor    ecx,ecx
    128f:	89 cf                	mov    edi,ecx
    1291:	83 c1 01             	add    ecx,0x1
    1294:	40 88 34 38          	mov    BYTE PTR [rax+rdi*1],sil
    1298:	44 39 c1             	cmp    ecx,r8d
    129b:	72 f2                	jb     128f <SSE_memset+0xbf>
    129d:	48 01 d0             	add    rax,rdx
    12a0:	c3                   	ret    
    12a1:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    12a8:	31 c0                	xor    eax,eax
    12aa:	c3                   	ret    
    12ab:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
    12b0:	48 8b 74 24 e8       	mov    rsi,QWORD PTR [rsp-0x18]
    12b5:	48 83 c0 08          	add    rax,0x8
    12b9:	48 83 ea 08          	sub    rdx,0x8
    12bd:	48 89 70 f8          	mov    QWORD PTR [rax-0x8],rsi
    12c1:	eb ba                	jmp    127d <SSE_memset+0xad>
    12c3:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
    12c8:	48 89 f8             	mov    rax,rdi
    12cb:	eb aa                	jmp    1277 <SSE_memset+0xa7>
*/


int main()
{
    char somedata_a1[100];
    for (int i = 0; i < sizeof(somedata_a1)/sizeof(char); i++)
        somedata_a1[i] = i * i;
    
    SSE_memset(somedata_a1, 0xff, sizeof(somedata_a1));

    puts("Dumping all data");
    for (int i = 0; i < sizeof(somedata_a1)/sizeof(char); i++)
        printf("%d: %x ", i, somedata_a1[i]);
    puts("");
}

