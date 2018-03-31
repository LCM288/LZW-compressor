#include<iostream>
#include<cstring>
#include"encode.h"
#include"decode.h"

char a[100], b[100], c[100];

int main() {
	scanf("%s", a);
	if (!strcmp("encode", a)) {
		scanf("%s %s", b, c);
		encode x(b, c);
		x.start_encode();
	}
	else if (!strcmp("decode", a)) {
		scanf("%s %s", b, c);
		decode x(b, c);
		x.start_decode();
	}
}
