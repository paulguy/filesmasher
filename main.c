#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "range.h"

int main(int argc, char **argv) {
	int outfile;
	FileRange **ranges;
	int nranges;
	off64_t filesize, randpos, totalarea;
	int i, j;
	long amount, il;
	unsigned int randtest;
	int randbits, bits;
	unsigned char randval;
	time_t lastupdate;

	if(argc < 3) {
		fprintf(stderr, "USAGE: filesmasher <filename> <amount> [ranges]\n");
		return(-1);
	}

	outfile = open(argv[1], O_WRONLY);
	if(outfile < 0) {
		fprintf(stderr, "Couldn't open %s for writing: ", argv[1]);
		perror("open");
		return(-1);
	}
	filesize = lseek64(outfile, 0, SEEK_END);
	if(filesize < 0) {
		perror("lseek64");
		return(-1);
	}

	amount = atol(argv[2]);
	if(amount < 1) {
		fprintf(stderr, "\"amount\" must be a value >0.\n");
		return(-1);
	}

	if(argc == 4) {
		ranges = NULL;
		nranges = range_parse(argv[3], &ranges);
		if(nranges < 0) {
			fprintf(stderr, "range_parse: (%d) %s.\n", nranges, range_error_string(nranges));
			return(-1);
		}

		if(nranges == 0) {
			fprintf(stderr, "No valid ranges found, this is a bug.\n");
			return(-1);
		}

		for(i = 0; i < nranges; i++) {
			if(ranges[i]->start > ranges[i]->end) {
				fprintf(stderr, "Range start %ld is greater than range end %ld.\n", ranges[i]->start, ranges[i]->end);
				return(-1);
			}
			if(ranges[i]->end > filesize - 1) {
				fprintf(stderr, "Range %ld-%ld goes past end of file (%ld).\n", ranges[i]->start, ranges[i]->end, filesize - 1);
				return(-1);
			}
			for(j = 0; j < nranges; j++) {
				if(i != j) {
					if(	ranges[i]->end >= ranges[j]->start &&
						ranges[i]->end <= ranges[j]->end) {
						fprintf(stderr, "Range %ld-%ld overlaps with range %ld-%ld.\n", 
										ranges[i]->start, ranges[i]->end,
										ranges[j]->start, ranges[j]->end);
						return(-1);
					}
				}
			}
		}
	} else {
		ranges = range_new(1);
		nranges = 1;
		ranges[0]->start = 0;
		ranges[0]->end = filesize - 1;
	}

	printf("Filename: %s (%ld bytes)\n", argv[1], filesize);
	printf("Amount of Corruption: %ld\n", amount);
	printf("Ranges: (%d)\n", nranges);
	for(i = 0; i < nranges; i++) {
		printf("%d. %ld-%ld, %ld-%ld\n", i, ranges[i]->start, ranges[i]->end, ranges[i]->totalstart, ranges[i]->totalend);
	}

	randtest = 0xFFFFFFFF;
	for(i = 32; i > 0; i--) {
		if((randtest & RAND_MAX) == randtest) {
			randbits = i;
			printf("srandom found to be %d bits.\n", randbits);
			break;
		}
		randtest /= 16;
	}

	srandom(time(NULL));
	lastupdate = 0;
	totalarea = ranges[nranges - 1]->totalend;
	for(il = 0; il < amount; il++) {
		bits = 62;
		randpos = random();
		bits -= randbits;
		while(bits > 0) {
			randpos |= random() << (62 - bits);
			bits -= randbits;
		}
		randpos %= totalarea;
		for(i = 0; i < nranges; i++) {
			if(randpos > ranges[i]->totalstart && randpos < ranges[i]->totalend) {
				randpos = randpos - ranges[i]->totalstart + ranges[i]->start;
				break;
			}
		}

		lseek64(outfile, randpos, SEEK_SET);
		randval = (char)(random() & 255);
		write(outfile, &randval, 1);
		if(time(NULL) > lastupdate) {
			printf("\r%li/%li", il + 1, amount);
			fflush(stdout);
			lastupdate = time(NULL);
		}
	}
	printf("\r%li/%li\n", il, amount);

	range_delete(ranges, nranges);
	close(outfile);
	return(0);
}
