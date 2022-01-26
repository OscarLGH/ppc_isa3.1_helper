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

void load_fpscr(uint64_t val)
{
	asm (
		"li %%r0, 0 \n"
		"lfiwax %%f9, %%r0, %0 \n"
		"mtfsf 0, %%f9, 1, 0 \n"
		:
		:"r"(&val)
		:"r0"
	);
}

void load_scalar_dst(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs3, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs3"
	);
}

void load_scalar_src1(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs0, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs0"
	);
}

void load_scalar_src2(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs1, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs1"
	);
}

void load_scalar_src3(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs2, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs2"
	);
}

void load_scalar_src4(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs4, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs4"
	);
}

void load_fp_dst(void *addr)
{
	asm (
		"lfd %%f3, 0(%0) \n"
		:
		:"r"(addr)
		:"f3"
	);
}

void load_fp_src1(void *addr)
{
	asm (
		"lfd %%f0, 0(%0) \n"
		:
		:"r"(addr)
		:"f0"
	);
}

void load_fp_src2(void *addr)
{
	asm (
		"lfd %%f1, 0(%0) \n"
		:
		:"r"(addr)
		:"f1"
	);
}

void load_fp_src3(void *addr)
{
	asm (
		"lfd %%f2, 0(%0) \n"
		:
		:"r"(addr)
		:"f2"
	);
}

void load_fp_src4(void *addr)
{
	asm (
		"lfd %%f4, 0(%0) \n"
		:
		:"r"(addr)
		:"f4"
	);
}

void operations()
{
	asm (
		"{###place_holder###} \n"
		:
		:
		:"f0","f1","f2","f3","f4"
	);
}

void operations_same_operand()
{
	asm (
		"{###place_holder_same_operand###} \n"
		:
		:
		:"f4"
	);
}

void store_scalar_dst1(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"stxvd2x %%vs3, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs3"
	);
}

void store_fp_dst(void *addr)
{
	asm (
		"stfd %%f3, 0(%0) \n"
		:
		:"r"(addr)
		:"f3"
	);
}

void store_fp_dst4(void *addr)
{
	asm (
		"stfd %%f4, 0(%0) \n"
		:
		:"r"(addr)
		:"f3"
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
		:"r0"
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
	//load_fpscr(0x0);
	for (i = 0; i < ARRAY_SIZE(vector_array_src1) / 2; i++) {
		for (j = 0; j < ARRAY_SIZE(vector_array_src1) / 2; j++) {
			for (k = 3; k < 4; k++) {
				load_fp_dst(&vector_array_src1[8 * 2]);
				load_fp_src1(&vector_array_src1[i * 2]);
				load_fp_src2(&vector_array_src1[j * 2]);
				load_fp_src3(&vector_array_src1[k * 2]);
				load_vscr(0x0);
				load_fpscr(0x0);
				load_cr(0x0);
				operations();
				cr = store_cr();
				store_vscr((__uint128_t *)&vscr[0]);
				fpscr = store_fpscr();
				//fpscr |= (2 << 17);
				//fpscr = 0;
				store_fp_dst(&vector_array_dst[0]);
				store_scalar_dst1(&vector_array_dst[1]);
				printf("####output1 %04d(%02d %02d %02d)####{DST 0x%016llx%016llx CR:%llx VSCR:%llx FPSCR:%llx}\n", cnt, i, j, k, vector_array_dst[1], vector_array_dst[0], cr, vscr[0], fpscr);
				//store_scalar_dst2(&vector_array_dst[0]);
				//printf("####output2 %04d(%02d %02d %02d)####{DST 0x%016llx%016llx CR:%llx VSCR:%llx FPSCR:%llx}\n", cnt, i, j, k, vector_array_dst[1], vector_array_dst[0], cr, vscr[0], fpscr);
				cnt++;
			}
		}
		load_fp_src4(&vector_array_src1[i * 2]);
		load_fpscr(0x0);
		load_cr(0x0);
		operations_same_operand();
		cr = store_cr();
		store_vscr((__uint128_t *)&vscr[0]);
		fpscr = store_fpscr();
		//fpscr |= (2 << 17);
		//fpscr = 0;
		store_fp_dst4(&vector_array_dst[0]);
		store_scalar_dst4(&vector_array_dst[1]);
		printf("####output1 %04d(%02d %02d %02d)####{DST 0x%016llx%016llx CR:%llx VSCR:%llx FPSCR:%llx}\n", cnt++, i, i, i, vector_array_dst[1], vector_array_dst[0], cr, vscr[0], fpscr);
	}
	return 0;
}
