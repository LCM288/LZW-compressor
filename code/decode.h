#include<dictionary.h>
#include<ifstream>
#include<ofstream>

class decode {
	private:
		std::ifstream input;						// input file stream
		std::ofstream output;						// output file stream
		dictionary dict;						// the dictionary
		void write(byte) const;						// write to output file
		bits read() const;						// read from input file
	public:
		start_decode() const;						// start decode the file
		decode(const char*, const char*);				// constructor
		~decode();							// destructor
}
