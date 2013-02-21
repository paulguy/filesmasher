#include "arand.h"

int32_t __arand_randbits;
uint32_t __arand_randtest;

int32_t arand_init(unsigned int seed) {
	int32_t i;

	__arand_randtest = 0xFFFFFFFF;
	for(i = 32; i > 0; i--) {
		if((__arand_randtest & RAND_MAX) == __arand_randtest) {
			__arand_randbits = i;
			break;
		}
		__arand_randtest >>= 1;
	}

	srandom(seed);

	return(__arand_randbits);
}

int arand_random64(uint64_t *value, int32_t bits) {
	int32_t bitsleft;

	if(bits < 1 || bits > 64) {
		return(-1);
	}

	bitsleft = bits;

	*value = random();
	bitsleft -= __arand_randbits;
	while(bitsleft > 0) {
		if(bitsleft < __arand_randbits) {
			*value |= (random() & (__arand_randtest >> (__arand_randbits - bitsleft))) << (bits - bitsleft);
		} else {
			*value |= random() << (bits - bitsleft);
		}
		bitsleft -= __arand_randbits;
	}

	return(0);
}

int arand_random32(uint32_t *value, int32_t bits) {
	int32_t bitsleft;

	if(bits < 1 || bits > 32) {
		return(-1);
	}

	bitsleft = bits;

	*value = random();
	bitsleft -= __arand_randbits;
	while(bitsleft > 0) {
		if(bitsleft < __arand_randbits) {
			*value |= (random() & (__arand_randtest >> (__arand_randbits - bitsleft))) << (bits - bitsleft);
		} else {
			*value |= random() << (bits - bitsleft);
		}
		bitsleft -= __arand_randbits;
	}

	return(0);
}

int arand_random16(uint16_t *value, int32_t bits) {
	int32_t bitsleft;

	if(bits < 1 || bits > 16) {
		return(-1);
	}

	bitsleft = bits;

	*value = random();
	bitsleft -= __arand_randbits;
	while(bitsleft > 0) {
		if(bitsleft < __arand_randbits) {
			*value |= (random() & (__arand_randtest >> (__arand_randbits - bitsleft))) << (bits - bitsleft);
		} else {
			*value |= random() << (bits - bitsleft);
		}
		bitsleft -= __arand_randbits;
	}

	return(0);
}

int arand_random8(uint8_t *value, int32_t bits) {
	int32_t bitsleft;

	if(bits < 1 || bits > 8) {
		return(-1);
	}

	bitsleft = bits;

	*value = random();
	bitsleft -= __arand_randbits;
	while(bitsleft > 0) {
		if(bitsleft < __arand_randbits) {
			*value |= (random() & (__arand_randtest >> (__arand_randbits - bitsleft))) << (bits - bitsleft);
		} else {
			*value |= random() << (bits - bitsleft);
		}
		bitsleft -= __arand_randbits;
	}

	return(0);
}

