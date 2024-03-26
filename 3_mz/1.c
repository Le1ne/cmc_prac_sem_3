#include <stdio.h>

typedef char STYPE;
typedef unsigned char UTYPE;

STYPE bit_reverse(STYPE value) {
	UTYPE size_value = 0;
	UTYPE temp_value = (UTYPE) value | ~((UTYPE) value);

	while (temp_value != 0) {
		size_value++;
		temp_value >>= 1;
	}

	UTYPE mask = (UTYPE) value;
	UTYPE res = 0;

	for (STYPE i = 0; i < size_value; ++i) {
		res <<= 1;
		res |= mask & 1;
		mask >>= 1;
	}

	return (STYPE) res;
}

int main(void) {
	STYPE a;
	scanf("%d", &a);

	printf("%0x\n%d\n", bit_reverse(a), a);

	return 0;
}
