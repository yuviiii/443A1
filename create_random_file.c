#include <stdlib.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
* populate a random array (which is already
* allocated with enough memory to hold n bytes.
*/

void random_array(char *array, long bytes){
	for (int i = 0; i < bytes; i++)
		array[i] = 'A' + (rand()%26);
}

int main(int argc, char **argv){
	struct timeb t;
	char *filename =argv[1];
	long total_bytes = atol(argv[2]);
	long block_size = atol(argv[3]);
	char *buf = malloc(block_size * sizeof(char));
	
	FILE *fp = fopen(filename, "w");
	long left_bytes = total_bytes;
	long start_t;
	long end_t;
	long writting_time = 0;
	long min;

	while (left_bytes > 0){
		min = (long)fmin((double)block_size, (double)left_bytes);
		random_array(buf, min);
		ftime(&t);
		start_t = t.time * 1000 + t.millitm;
		fwrite(buf, 1, min, fp);
		fflush(fp);
		ftime(&t);
		end_t = t.time * 1000 + t.millitm;
		writting_time += (end_t - start_t);
		left_bytes -= block_size;
	}

	printf("write %ld bytes into %s, data rate: %f, time:%ld\n", total_bytes, filename, (double)total_bytes/writting_time*1000, writting_time);
	fclose(fp);
	free(buf);
	return 0;
}

