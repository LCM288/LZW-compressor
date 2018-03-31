#include<vector>

typedef bool bit;
typedef unsigned char byte;

typedef std::vector<bit> bits;
typedef std::vector<byte> bytes;

class code {
	int val;
	code() { val = 0; }
	explicit code(int x) : val(x) {};
	operator int() { return val; }
}
