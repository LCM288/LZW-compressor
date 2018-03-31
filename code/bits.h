#include<bitset>
#include<vector>

typedef std::bitset<1> bit;
typedef std::bitset<8> byte;

typedef std::vector<bit> bits;
typedef std::vector<byte> bytes;

class code {
	int val;
	code() { val = 0; }
	explicit code(int x) : val(x) {};
	operator int() { return val; }
}
