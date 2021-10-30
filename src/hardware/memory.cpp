#include <stddef.h>
#include <stdint.h>


void *memcpy(void *des, const void *src, size_t n) {
	uint8_t *source = (uint8_t *)src;
	uint8_t *destination = (uint8_t *)des;

	for (size_t i = 0; i < n; i++) {
		destination[i] = source[i];
	}
	return des;
}



void *memset(void *str, uint8_t c, size_t n) {
	uint8_t *destination = (uint8_t *)str;

	for (size_t i = 0; i < n; i++) {
		destination[i] = c;
	}
	return str;
}



int32_t memcmp(const void *str1, const void *str2, size_t n) {
	uint8_t *stra = (uint8_t *)str1;
	uint8_t *strb = (uint8_t *)str2;

	for (size_t i = 0; i < n; i++) {
		int32_t diff = stra[i] - strb[i];
		if (diff) {
			return diff;
		}
	}
	return 0;
}



/*
Don't feel like writing this unless it actually becomes an issue later on.


void *memmove(void *str1, const void *str2, size_t n) {


}
*/
