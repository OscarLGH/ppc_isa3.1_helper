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
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x0,0x0,				 /* integer 0, float 0 */
	0x7f7f7f7f7f7f7f7f, 0x7f7f7f7f7f7f7f7f,  /* integer +big, float +normal */
	0xffffffffffffffff, 0xffffffffffffffff,  /* integer -1, float NaN */
	0xffffffffffffffff, 0xffffffffffffffff,  /* integer -1, float NaN */
	0x0,0x0,				 /* integer 0, float 0 */
	0x8000000080000000, 0x8000000080000000,  /* integer -Max, float -0 */
	0x8000000000000000, 0x8000000000000000,  /* integer -Max, double -0 */
	0x7f800000ff800000, 0x7f800000ff800000,  /* integer +a, float inf/-inf */
	0x7f800000ff800000, 0x7f800000ff800000,  /* integer +a, float inf/-inf */
	0x7f80000000000000, 0xff80000000000000,  /* integer +a, double inf/-inf */
	0x7f80000000000000, 0xff80000000000000,  /* integer +a, double inf/-inf */
	0x0000000000000001, 0x0000000000000001,	 /* integer 1, double deNormal */
	0x0000000000000001, 0x0000000000000001,	 /* integer 1, double deNormal */
	0x7f7fffffffff7f7f, 0xffffffffffffffff,  /* integer +a, double +MAX */
	0x0080000000000001, 0x0080000000000001,  /* integer +a, double +MIN */
	0xff7fffffffffff7f, 0xffffffffffffffff,  /* integer -a, double -MAX */
	0x8080000000000001, 0x8080000000000001,  /* integer -a, double -MIN */
	0x7f7fffffffff7f7f, 0xffffffffffffffff,  /* integer +a, double +MAX */
	0x0080000000000001, 0x0080000000000001,  /* integer +a, double +MIN */
	0xff7fffffffffff7f, 0xffffffffffffffff,  /* integer -a, double -MAX */
	0x8080000000000001, 0x8080000000000001,  /* integer -a, double -MIN */
};
uint64_t __attribute__((aligned(16))) vector_array_src2[] = {
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x0,0x0,				 /* integer 0, float 0 */
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x7f7f7f7f7f7f7f7f, 0x7f7f7f7f7f7f7f7f,  /* integer +big, float +normal */
	0xffffffffffffffff, 0xffffffffffffffff,  /* integer -1, float NaN */
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x0,0x0,				 /* integer 0, float 0 */
	0x8000000080000000, 0x8000000080000000,  /* integer -Max, float -0 */
	0x8000000000000000, 0x8000000000000000,  /* integer -Max, double -0 */
	0x7f800000ff800000, 0x7f800000ff800000,  /* integer +a, float inf/-inf */
	0x7f80000000000000, 0xff80000000000000,  /* integer +a, double inf/-inf */
	0x7f80000000000000, 0xff80000000000000,  /* integer +a, double inf/-inf */
	0x0,0x0,        			 /* integer 0, float 0 */
	0x0000000000000001, 0x0000000000000001,	 /* integer 1, double deNormal */
	0x7f7fffffffff7f7f, 0xffffffffffffffff,  /* integer +a, double +MAX */
	0x0080000000000001, 0x0080000000000001,  /* integer +a, double +MIN */
	0xff7fffffffffff7f, 0xffffffffffffffff,  /* integer -a, double -MAX */
	0x8080000000000001, 0x8080000000000001,  /* integer -a, double -MIN */
	0x8080000000000001, 0x8080000000000001,  /* integer -a, double -MIN */
	0xff7fffffffffff7f, 0xffffffffffffffff,  /* integer -a, double -MAX */
	0x0080000000000001, 0x0080000000000001,  /* integer +a, double +MIN */
	0x7f7fffffffff7f7f, 0xffffffffffffffff,  /* integer +a, double +MAX */
};
uint64_t __attribute__((aligned(16))) vector_array_src3[] = {
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x0,0x0,				 /* integer 0, float 0 */
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x7f7f7f7f7f7f7f7f, 0x7f7f7f7f7f7f7f7f,  /* integer +big, float +normal */
	0xffffffffffffffff, 0xffffffffffffffff,  /* integer -1, float NaN */
	0x0001020304050607, 0x08090a0b0c0d0e0f,  /* integer normal, float normal */
	0x0,0x0,				 /* integer 0, float 0 */
	0x8000000080000000, 0x8000000080000000,  /* integer -Max, float -0 */
	0x8000000000000000, 0x8000000000000000,  /* integer -Max, double -0 */
	0x7f800000ff800000, 0x7f800000ff800000,  /* integer +a, float inf/-inf */
	0x7f80000000000000, 0xff80000000000000,  /* integer +a, double inf/-inf */
	0x7f80000000000000, 0xff80000000000000,  /* integer +a, double inf/-inf */
	0x0,0x0,				 /* integer 0, float 0 */
	0x0000000000000001, 0x0000000000000001,	 /* integer 1, double deNormal */
	0x7f7fffffffff7f7f, 0xffffffffffffffff,  /* integer +a, double +MAX */
	0x0080000000000001, 0x0080000000000001,  /* integer +a, double +MIN */
	0xff7fffffffffff7f, 0xffffffffffffffff,  /* integer -a, double -MAX */
	0x8080000000000001, 0x8080000000000001,  /* integer -a, double -MIN */
	0x8080000000000001, 0x8080000000000001,  /* integer -a, double -MIN */
	0xff7fffffffffff7f, 0xffffffffffffffff,  /* integer -a, double -MAX */
	0x0080000000000001, 0x0080000000000001,  /* integer +a, double +MIN */
	0x7f7fffffffff7f7f, 0xffffffffffffffff,  /* integer +a, double +MAX */
};
uint64_t __attribute__((aligned(16))) vector_array_dst[2] = {0};


void load_scalar_src1(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs0, %%r0, %0 \n"
		"lxvd2x %%vs32, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs0","vs32"
	);
}

void load_scalar_src2(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs1, %%r0, %0 \n"
		"lxvd2x %%vs33, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs1","vs33"
	);
}

void load_scalar_src3(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"lxvd2x %%vs2, %%r0, %0 \n"
		"lxvd2x %%vs34, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs2","vs34"
	);
}

void operations()
{
	asm (
		"{###place_holder###} \n"
		:
		:
		:"vs0","vs1","vs2","vs3","vs32","vs33","vs34","vs35"
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

void store_scalar_dst2(void *vector)
{
	asm (
		"li %%r0, 0 \n"
		"stxvd2x %%vs35, %%r0, %0 \n"
		:
		:"r"(vector)
		:"r0","vs35"
	);
}

void main()
{
	int i;
	for (i = 0; i < ARRAY_SIZE(vector_array_src1); i++) {
		//printf("loading vector:%llx %llx\n", vector_array_ld[0], vector_array_ld[1]);
		load_scalar_src1(&vector_array_src1[i * 2]);
		load_scalar_src2(&vector_array_src2[i * 2]);
		load_scalar_src3(&vector_array_src3[i * 2]);
		operations_1();
		store_scalar_dst1(&vector_array_dst[0]);
		printf("####output####{%016llx %016llx}\n",((uint64_t *)&vector_array_dst[0])[0], ((uint64_t *)&vector_array_dst[0])[1]);
		operations_2();
		store_scalar_dst2(&vector_array_dst[0]);
		printf("####output####{%016llx %016llx}\n",((uint64_t *)&vector_array_dst[0])[0], ((uint64_t *)&vector_array_dst[0])[1]);
	}
}