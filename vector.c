#include <stdio.h>
#include <math.h>

/* special cases:
integer: 
	1. (+a) op (+b)
	2. (-a) op (-b)
	3. a op 0
	4. 0 op b
float/double:
	1. a op 0
	2. 0 op b
	3. +0 op -0
	4. +inf op b
	5. +inf op 0
	6. +inf op -inf
	7. NaN op NaN
	8. deNor op deNor
	9. Max op Max :overflow
	10.Min op Min :underflow
*/
typedef unsigned long long uint64_t;
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))





uint64_t __attribute__((aligned(16))) vector_array_src1[] = {
	0xff800000ff800000, 0xff800000ff800000,  /* float -inf */
	0xfff0000000000000, 0xfff0000000000000,  /* double -inf */
	0xff7fffffff7fffff, 0xff7fffffff7fffff,  /* float -NZF -MAX */
	0xffefffffffffffff, 0xffefffffffffffff,  /* double -NZF -MAX */
	0x8080000080800000, 0x8080000080800000,  /* float -NZF -MIN */
	0x8010000000000000, 0x8010000000000000,  /* double -NZF -MIN */
	0x8000000080000000, 0x8000000080000000,  /* float -0 */
	0x8000000000000000, 0x8000000000000000,  /* double -0 */
	0x0,0x0,				  /* float/double +0 */
	0x7f7fffff7f7fffff, 0x7f7fffff7fffffff,  /* float +MAX +NZF */
	0x7fefffffffffffff, 0x7fefffffffffffff,  /* double +MAX +NZF */
	0x0080000000800000, 0x0080000000800000,  /* float +MIN +NZF*/
	0x0010000000000000, 0x0010000000000000,  /* double +MIN +NZF*/
	0x7f8000007f800000, 0x7f8000007f800000,  /* float +inf */
	0x7ff0000000000000, 0x7ff0000000000000,  /* double +inf */
	0xffffffffffffffff, 0xffffffffffffffff,  /* float/double QNaN1 */
	0xfffffffffffffffe, 0xfffffffffffffffe,  /* float/double QNaN2 */
	0xffbfffffffbfffff, 0xffbfffffffbfffff,  /* float SNaN1 */
	0xffbffffeffbffffe, 0xffbffffeffbffffe,  /* float SNaN2 */
	0xfff7ffffffffffff, 0xfff7ffffffffffff,  /* double SNaN1 */
	0xfff7fffffffffffe, 0xfff7fffffffffffe,  /* double SNaN2 */
	0x08090a0b0c0d0e0f, 0x0001020304050607,  /* integer vector */
};

uint64_t __attribute__((aligned(16))) vector_array_dst[2] = {0};

void load_cr(uint64_t val)
{
	asm (
		"mtcr %0 \n"
		:
		:"r"(val)
		:
	);
}

void load_vscr(__uint128_t val)
{
	asm (
		"li %%r0, 0 \n"
		"lvx %%v9, %%r0, %0 \n"
		"mtvscr %%v9 \n"
		:
		:"r"(&val)
		:"r0","v9"
	);
}

void load_vector_src1(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lvx %%v0, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v0"
	);
}

void load_vector_src2(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lvx %%v1, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v1"
	);
}

void load_vector_src3(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lvx %%v2, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v2"
	);
}

void operations()
{
	asm (
		"{###place_holder###} \n"
		:
		:
		:"v0","v1","v2","v3"
	);
}

void store_vector_dst(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"stvx %%v3, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v3"
	);
}

uint64_t store_cr(void)
{
	uint64_t cr_val;
	asm (
		"mfcr %0 \n"
		:"=r"(cr_val)
		:
		:
	);
	return cr_val;
}

void store_vscr(__uint128_t *ptr)
{
	__uint128_t vscr_val;
	asm (
		"mfvscr %%v9 \n"
		"li %%r0, 0 \n"
		"stvx %%v9, %%r0, %0 \n"
		:
		:"r"(ptr)
		:"r0","v9"
	);
}

uint64_t store_fpscr(void)
{
	uint64_t fpscr_val;
	asm (
		"mffs %%f9 \n"
		"li %%r0, 0 \n"
		"stfdx %%f9, %%r0, %0"
		:
		:"r"(&fpscr_val)
		:
	);
	return fpscr_val;
}

uint64_t __attribute__((aligned(16))) vscr[2] = {0};

void main()
{
	int i, j, k;
	int cnt = 0;
	uint64_t cr = 0, fpscr = 0;
	
	for (i = 0; i < ARRAY_SIZE(vector_array_src1) / 2; i++)
		for (j = 0; j < ARRAY_SIZE(vector_array_src1) / 2; j++)
			for (k = 3; k < 4; k++) {
				load_vector_src1(&vector_array_src1[i * 2]);
				load_vector_src2(&vector_array_src1[j * 2]);
				load_vector_src3(&vector_array_src1[k * 2]);
				load_cr(0x0);
				load_vscr(0x0);
				operations();
				cr = store_cr();
				store_vscr((__uint128_t *)&vscr[0]);
				fpscr = store_fpscr();
				store_vector_dst(&vector_array_dst[0]);
				printf("####output %04d(%02d %02d %02d)####{DST 0x%016llx%016llx CR:%llx VSCR:%llx FPSCR:%llx}\n", cnt, i, j, k, vector_array_dst[1], vector_array_dst[0], cr, vscr[0], fpscr);
				cnt++;
			}
}
