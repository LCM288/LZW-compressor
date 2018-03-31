#include"dictionary.h"
#include<fstream>

#ifndef DECODE_H
#define DECODE_H

class decode {
	private:
		std::ifstream input;				// input file stream
		std::ofstream output;				// output file stream
		bits buffer;						// store the bits that have not been input
		dictionary dict;					// the dictionary
		void write(byte);					// write to output file
		void write(bytes);					// write to output file
		byte read();						// read a byte
		bits read(int);						// read from input file
	public:
		void start_decode();				// start decode the file
		decode(const char*, const char*);	// constructor
};

void decode::write(byte output_byte) {
	char tmp = output_byte;
	output.write(&tmp, 1);
}

void decode::write(bytes output_bytes) {
	for (auto output_byte: output_bytes)
		write(output_byte);
}

byte decode::read() {
	char tmp;
	input.read(&tmp, 1);
	return (unsigned char)(tmp);
}

bits decode::read(int no_of_bits) {
	bits input_bits = buffer;
	if (int(buffer.size()) > no_of_bits) {									// if buffer is larger than required
		buffer = bits(input_bits.begin() + no_of_bits, input_bits.end());
		while (int(input_bits.size()) > no_of_bits)
			input_bits.pop_back();
		return input_bits;
	}
	buffer.clear();
	no_of_bits -= int(input_bits.size());
	while (no_of_bits >= 8) {
		byte next_byte = read();
		for (int i = 128; i; i /= 2) {
			input_bits.push_back(next_byte / i);
			next_byte %= i;
		}
		no_of_bits -= 8;
	}
	if (no_of_bits) {
		byte next_byte = read();
		for (int i = 128; i; i /= 2) {
			if (no_of_bits) {
				input_bits.push_back(next_byte / i);
				no_of_bits--;
			}
			else
				buffer.push_back(next_byte / i);
			next_byte %= i;
		}
	}
	return input_bits;
}

void decode::start_decode() {
	while(!input.eof())
		write(dict.decode(read(dict.next_length())));
}

decode::decode (const char *input_file, const char *output_file) {
	input.open(input_file, std::ios::in | std::ios::binary);
	output.open(output_file, std::ios::out | std::ios::binary);
}

#endif
