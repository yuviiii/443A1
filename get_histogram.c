#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <assert.h>
#include <math.h>

/**
 * file_ptr : the file pointer, ready to be read from.
 * hist: an array to hold 26 long integers.  hist[0] is the
 *       number of 'A', and hist[1] is the number of 'B', etc.
 * block_size: the buffer size to be used.
 * milliseconds: time it took to complete the file scan
 * total_bytes_read: the amount data in bytes read
 *
 * returns: -1 if there is an error.
 */
int get_histogram(
    FILE *file_ptr, 
    long hist[], 
    int block_size, 
    long *milliseconds, 
    long *total_bytes_read){

	long min;
    fseek(file_ptr, 0, SEEK_END);
    long bytes = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

	struct timeb t;
	long start_t;
	long end_t;
	char *buf = malloc(block_size * sizeof(char));

	ftime(&t);
	start_t = t.time * 1000 + t.millitm;
	*total_bytes_read = 0;

	while (bytes > 0){
		min = (long)fmin((double)block_size, (double)bytes);
		bzero(buf,block_size);
		fread(buf, 1, min, file_ptr);
		for (int i = 0; i < min; i++){
			hist[buf[i] - 'A'] ++;
		}
		*total_bytes_read += min;
		bytes -= min;
	}
	ftime(&t);
	end_t = t.time*1000 + t.millitm;
	*milliseconds = end_t - start_t;
	fclose(file_ptr);
	return 0;
}

int main(int argc, char *argv[]){
	char *filename = argv[1];
	long block_size = atol(argv[2]);
	long hist[26];
	long milliseconds;
	long filelen;
	FILE *file_ptr = fopen(filename, "r");

	for (int i = 0; i < 26; i++) hist[i] = 0;
	int ret = get_histogram(file_ptr,
							hist,
							block_size,
							&milliseconds,
							&filelen);
	assert(!(ret < 0));

	printf("Computed the histogram in %ld ms.\n", milliseconds);
	for (int i = 0; i < 26; i++){
		printf("%c : %ld\n", 'A' + i, hist[i]);
	}
	printf("Data rate: %f Bps\n", (double)filelen/milliseconds*1000);
	fclose(file_ptr);
	return 0;
}
