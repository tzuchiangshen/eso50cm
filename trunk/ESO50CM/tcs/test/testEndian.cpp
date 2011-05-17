#include <stdio.h>
#include <ctype.h>

void print_bytes(double x) {
	printf("%.2f\n", x);

	const char *s = (char*)&x;
	for(int i=0; i<8; i++)
		printf("s[%d] = 0x%X\n",i, s[i]);
	printf("\n\n");
}

int main() {

	//double *a = (double*) malloc(sizeof(double));

	printf("double represtation size = %d\n bytes", sizeof(double));
	double x = 9006104071832581.0;
	print_bytes(x);
	
	x = -9006104071832581.0;
	print_bytes(x);
	
	x = 1.0;
	print_bytes(x);

	x = -1.0;
	print_bytes(x);

}
