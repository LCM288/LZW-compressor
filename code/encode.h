#include"dictionary.h"
#include<fstream>

#ifndef ENCODE_H
#define ENCODE_H

class encode {
	private:
		std::ifstream input;				// input file stream
		std::ofstream output;				// output file stream
		std::streampos begin, end;			// begin and end position of the input file
		long long input_size, output_size;	// size of input and output file
		bits buffer;						// store the bits that have not been output
		dictionary dict;					// the dictionary
		void write(byte);					// write a specific byte
		void write(bits);					// write to output file
		byte read();						// read from input file 
	public:
		void start_encode();				// start encode the file
		encode(const char*, const char*);	// constructor
};

void encode::write(byte output_byte) {
	char tmp = output_byte;
	output.write(&tmp, 1);
	output_size += 1;
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

byte encode::read() {
	char tmp;
	input.read(&tmp, 1);
	input_size += 1;
	return (unsigned char)(tmp);
}

void encode::start_encode() {
	while (!input.eof()) {
		write(dict.encode(read()));
		if (!rand())
			printf("Compressed %.2lf%% (%lld / %lld) Compression rate %.2lf%%\n", 
																				100. * input_size / ((long long) (end - begin)),
																				input_size, (long long) (end - begin), 
																				100. * output_size / input_size);
	}
	if (buffer.size()) {
		while (buffer.size() < 8) buffer.push_back(0);
		byte output_byte = 0;
		for (auto a_bit: buffer)
			output_byte = output_byte * 2 + a_bit;
		write(output_byte);
		buffer.clear();
	}
	printf("Compressed 100.00%% (%lld / %lld) Compression rate %.2lf%%\n",
																			(long long) (end - begin), (long long) (end - begin), 
																			100. * output_size / (end - begin));
}

encode::encode (const char *input_file, const char *output_file) {
	input.open(input_file, std::ios::in | std::ios::binary);
	output.open(output_file, std::ios::out | std::ios::binary);
	input_size = output_size = 0;
	begin = input.tellg();
	input.seekg (0, std::ios::end);
	end = input.tellg();
	input.seekg (0, std::ios::beg);
}

#endif
