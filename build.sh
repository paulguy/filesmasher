#!/bin/sh

gcc -W -Wall -Wextra -pedantic -D_LARGEFILE64_SOURCE -O3 -c -o range.o range.c
gcc -W -Wall -Wextra -pedantic -D_LARGEFILE64_SOURCE -O3 -o filesmasher main.c range.o

