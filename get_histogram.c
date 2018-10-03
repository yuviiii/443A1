#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <assert.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
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
    long *total_bytes_read);

int get_histogram(FILE *file_ptr, 
    long hist[], int block_size, 
    long *milliseconds, long *total_bytes_read){

	char *buf = malloc(block_size*sizeof(char));
	fseek(file_ptr,0,SEEK_END);
	long total = ftell(file_ptr);
	fseek(file_ptr,0,SEEK_SET);
	printf("%ld\n", total);

	struct timeb t;
	long now_in_ms1,now_in_ms2;
	ftime(&t);
	now_in_ms1 = t.time*1000 + t.millitm;
	while (total>0){
		bzero(buf,block_size);
		fread(buf, 1, MIN(block_size,total), file_ptr);
		for (int i=0; i<MIN(block_size,total); i++){
			char c = buf[i];
			hist[c-'A']+=1;
		}
		*total_bytes_read += MIN(block_size,total);
		total-=block_size;
	}
	ftime(&t);
	now_in_ms2 = t.time*1000 + t.millitm;
	*milliseconds = now_in_ms2 - now_in_ms1;
	return 0;
}

int main(int argc, char *argv[])
{
	char *filename = argv[1];
	long block_size = atol(argv[2]);
	long hist[26],milliseconds,filelen;
	for (int i = 0; i < 26; i++) hist[i] = 0;
	FILE *file_ptr = fopen(filename, "r");
	int ret = get_histogram(file_ptr,hist,block_size,&milliseconds,&filelen);
	assert(!(ret < 0));
	printf("Computed the histogram in %ld ms.\n", milliseconds);
	long total_byte=0;
	for (int i = 0; i < 26; i++)
	{
		total_byte+=hist[i];
		printf("%c : %ld\n", 'A'+i, hist[i]);
	}
	printf("%ld\n", milliseconds);
	printf("Data rate: %f Bps\n", (double)filelen/milliseconds*1000);
	printf("total_byte:%ld\n", filelen);
	fclose(file_ptr);
	return 0;
}
