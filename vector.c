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
	0xff800000ff800000, 0xff800000ff800000,  /* [0] float -inf */
	0xfff0000000000000, 0xfff0000000000000,  /* [1] double -inf */
	0xff7fffffff7fffff, 0xff7fffffff7fffff,  /* [2] float -NZF -MAX */
	0xffefffffffffffff, 0xffefffffffffffff,  /* [3] double -NZF -MAX */
	0x8080000080800000, 0x8080000080800000,  /* [4] float -NZF -MIN */
	0x8010000000000000, 0x8010000000000000,  /* [5] double -NZF -MIN */
	0x8000000080000000, 0x8000000080000000,  /* [6] float -0 */
	0x8000000000000000, 0x8000000000000000,  /* [7] double -0 */
	0x0,0x0,								 /* [8] float/double +0 */
	0x7f7fffff7f7fffff, 0x7f7fffff7fffffff,  /* [9] float +MAX +NZF */
	0x7fefffffffffffff, 0x7fefffffffffffff,  /* [10] double +MAX +NZF */
	0x0080000000800000, 0x0080000000800000,  /* [11] float +MIN +NZF*/
	0x0010000000000000, 0x0010000000000000,  /* [12] double +MIN +NZF*/
	0x7f8000007f800000, 0x7f8000007f800000,  /* [13] float +inf */
	0x7ff0000000000000, 0x7ff0000000000000,  /* [14] double +inf */
	0xffffffffffffffff, 0xffffffffffffffff,  /* [15] float/double QNaN1 */
	0xfffffffffffffffe, 0xfffffffffffffffe,  /* [16] float/double QNaN2 */
	0xffbfffffffbfffff, 0xffbfffffffbfffff,  /* [17] float SNaN1 */
	0xffbffffeffbffffe, 0xffbffffeffbffffe,  /* [18] float SNaN2 */
	0xfff7ffffffffffff, 0xfff7ffffffffffff,  /* [19] double SNaN1 */
	0xfff7fffffffffffe, 0xfff7fffffffffffe,  /* [20] double SNaN2 */
	0x08090a0b0c0d0e0f, 0x0001020304050607,  /* [21] integer vector */
	0x3F8E38E33F8E38E3, 0x3F8E38E33F8E38E3,  /* [22] float 1.1111111 */
	0x3FF1C71C6ECB8FB6, 0x3FF1C71C6ECB8FB6,  /* [23] double 1.1111111 */
	0x3FF1C71C71C71C72, 0x3FF1C71C71C71C72,  /* [24] double 1.1111111111111111 */
	0xBF8E38E3BF8E38E3, 0xBF8E38E3BF8E38E3,  /* [25] float -1.1111111 */
	0xBFF1C71C6ECB8FB6, 0xBFF1C71C6ECB8FB6,  /* [26] double -1.1111111 */
	0xBFF1C71C71C71C72, 0xBFF1C71C71C71C72,  /* [27] double -1.1111111111111111 */
	0x3F7FFFFF3F7FFFFF, 0x3F7FFFFF3F7FFFFF,  /* [28] float 0.99999994 */
	0x3FEFFFFFDFC9A9AD, 0x3FEFFFFFDFC9A9AD,  /* [29] double 0.99999994 */
	0x3FEFFFFFFFFFFFFF, 0x3FEFFFFFFFFFFFFF,  /* [30] double 0.9999999999999999 */
	0xBF7FFFFFBF7FFFFF, 0xBF7FFFFFBF7FFFFF,  /* [31] float -0.99999994 */
	0xBFEFFFFFDFC9A9AD, 0xBFEFFFFFDFC9A9AD,  /* [32] double -0.99999994 */
	0xBFEFFFFFFFFFFFFF, 0xBFEFFFFFFFFFFFFF,  /* [33] double -0.9999999999999999 */
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

void load_vector_dst(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lvx %%v3, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v2"
	);
}

void load_vector_src4(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lvx %%v4, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v4"
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

void operations_same_operand()
{
	asm (
		"{###place_holder_same_operand###} \n"
		:
		:
		:"v4"
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

void store_vector_dst_1(__uint128_t *vector)
{
	asm (
		"li %%r0, 0 \n"
		"stvx %%v4, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","v4"
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

int main()
{
	int i, j, k;
	int cnt = 0;
	uint64_t cr = 0, fpscr = 0;
	
	for (i = 0; i < ARRAY_SIZE(vector_array_src1) / 2; i++) {
		for (j = 0; j < ARRAY_SIZE(vector_array_src1) / 2; j++) {
			for (k = 3; k < 4; k++) {
				load_vector_src1(&vector_array_src1[i * 2]);
				load_vector_src2(&vector_array_src1[j * 2]);
				load_vector_src3(&vector_array_src1[k * 2]);
				load_cr(0x0);
				load_vscr(0x0);
				operations();
				cr = store_cr();
				store_vscr((__uint128_t *)&vscr[0]);
				store_vector_dst(&vector_array_dst[0]);
				printf("####output %04d(%02d %02d %02d)####{DST 0x%016llx%016llx CR:%llx VSCR:%llx}\n", cnt, i, j, k, vector_array_dst[1], vector_array_dst[0], cr, vscr[0]);
				cnt++;
			}
		}
		load_vector_src4(&vector_array_src1[i * 2]);
		load_cr(0x0);
		load_vscr(0x0);
		operations_same_operand();
		cr = store_cr();
		store_vscr((__uint128_t *)&vscr[0]);
		store_vector_dst_1(&vector_array_dst[0]);
		printf("####output %04d(%02d %02d %02d)####{DST 0x%016llx%016llx CR:%llx VSCR:%llx}\n", cnt++, i, i, i, vector_array_dst[1], vector_array_dst[0], cr, vscr[0]);
	}
	return 0;
}
