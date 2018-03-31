#include<dictionary.h>
#include<ifstream>
#include<ofstream>

class encode {
	private:
		std::ifstream input;				// input file stream
		std::ofstream output;				// output file stream
		bits buffer;						// store the bits that have not been output
		dictionary dict;					// the dictionary
		void write(byte) const;				// write a specific byte
		void write(bits);					// write to output file
		byte read() const;					// read from input file
	public:
		void start_encode();				// start encode the file
		encode(const char*, const char*);	// constructor
}

void encode::write(byte output_byte) const {
	output.write(&output_byte, 1);
}

void encode::write(bits output_bits) {
	if (buffer.size() + output_bits.size() < 8)
		for (auto a_bit: output_bits)
			buffer.push_back(a_bit);
	else {
		int cur_len = 0;
		byte output_byte = 0;
		for (auto a_bit: buffer) {
			output_byte = output_byte * 2 + a_bit;
			cur_len++;
		}
		buffer.clear();
		for (auto a_bit: output_bits) {
			buffer.push_back(a_bit);
			output_byte = output_byte * 2 + a_bit;
			cur_len++;
			if (cur_len == 8) {
				write(output_byte);
				cur_len = 0;
				output_byte = 0;
				buffer.clear();
			}
		}
	}
}

byte encode::read() const {
	byte input_byte;
	input.read(&input_byte, 1);
	return input_byte;
}

void encode::start_encode() {
	while (!input.eof())
		write(dict.encode(read()));
}

encode::encode (const char *input_file, const char *output_file) {
	input.open(input_file, std::ios::in | std::ios::binary);
	output.open(output_file, std::ios::out | std::ios::binary);
	buffer = bits(0);
	dict = dictionary();
}
