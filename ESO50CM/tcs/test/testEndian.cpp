#include <stdio.h>
#include <ctype.h>

void print_bytes(double x) {
	printf("%.2f\n", x);

	const char *s = (char*)&x;
	for(int i=0; i<8; i++)
		printf("s[%d] = 0x%X\n",i, s[i]);
	printf("\n\n");
}

void print_bytes(unsigned x) {
	printf("%d\n", x);

	const char *s = (char*)&x;
	for(int i=0; i<sizeof(unsigned int); i++)
		printf("s[%d] = 0x%X\n",i, s[i]);
	printf("\n\n");
}


int main() {

	double x;

	//double *a = (double*) malloc(sizeof(double));

//	printf("double represtation size = %d bytes\n", sizeof(double));
	x = 9006104071832581.0;

//	print_bytes(x);
//	
//	x = -9006104071832581.0;
//	print_bytes(x);
//	
//	x = 1.0;
//	print_bytes(x);


//	x = -1.0;
	print_bytes(x);

	unsigned int flo = 0; 
	unsigned int fhi = 0;

	double test;

	char *p = (char*)&x;
	//first byte
	printf("byte[0]=%#x\n", *p);
	flo |= *p;

	//second byte
	p++;
	printf("byte[1]=%#x\n", *p);
	flo |= *p << 8;

	//third byte
	p++;
	printf("byte[2]=%#x\n", *p);
	flo |= *p << 16;

	//fourth byte
	p++;
	printf("byte[3]=%#x\n", *p);
	flo |= *p << 24;

	//fifith byte
	p++;
	printf("byte[4]=%#x\n", *p);
	fhi |= *p;

	//sixth byte
	p++;
	printf("byte[5]=%#x\n", *p);
	fhi |= *p << 8;

	p++;
	printf("byte[6]=%#x\n", *p);
	fhi |= *p << 16;

	//eigth byte
	p++;
	printf("byte[7]=%#x\n", *p);
	fhi |= *p << 24;

	print_bytes(flo);
	print_bytes(fhi);


}
