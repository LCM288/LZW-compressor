#include<dictionary.h>
#include<ifstream>
#include<ofstream>

class encode {
	private:
		std::ifstream input;						// input file stream
		std::ofstream output;						// output file stream
		dictionary dict;							// the dictionary
		void write(bits) const;						// write to output file
		byte read() const;							// read from input file
	public:
		start_encode() const;						// start encode the file
		encode(const char*, const char*);			// constructor
		~encode();									// destructor
}
