#define USE_MAP
#include<iostream>
#include<cstring> 
#include"encode.h"
#include"decode.h"

char a[100], b[100], c[100], d[10000];

int main() {
	printf("encode or decode: ");
	scanf("%s", a);
	if (!strcmp("encode", a)) {
		printf("Please input the name of the file to be encoded: ");
		scanf("%s", b);
		printf("Please input the name of the desire output file name: ");
		scanf("%s", c);
		printf("Do you want to use a password to protect the file? (Y/N) ");
		scanf("%s", d);
		if (d[0] == 'Y' || d[0] == 'y') {
			printf("Please input the password: ");
			scanf("%s", d);
		}
		else d[0] = 0;
		encode x(b, c, d);
		x.start_encode();
	}
	else if (!strcmp("decode", a)) {
		printf("Please input the name of the file to be decoded: ");
		scanf("%s", b);
		printf("Please input the name of the desire output file name: ");
		scanf("%s", c);
		printf("Is the file protected by any password? (Y/N) ");
		scanf("%s", d);
		if (d[0] == 'Y' || d[0] == 'y') {
			printf("Please input the password: ");
			scanf("%s", d);
		}
		else d[0] = 0;
		decode x(b, c, d);
		x.start_decode();
	}
}
