#include <stdio.h>
#include <ctype.h>

int main() {

	//double *a = (double*) malloc(sizeof(double));

	printf("double represtation size = %d\n", sizeof(double));
	double x = 9006104071832581.0;
	//double x = 1.0;
	printf("%.2f\n", x);

	const char *s = (char*)&x;
	for(int i=0; i<8; i++)
		printf("s[%d] = 0x%X\n",i, s[i]);

	//read the first 4 bytes 
	unsigned int fhi = (unsigned int)x;
	const char *l = (char*)&fhi;
	printf("first 4 bytes\n");
	for(int i=0; i<4; i++)
		printf("s[%d] = 0x%X\n",i, l[i]);
	


}
