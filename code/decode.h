#include"dictionary.h"
#include"decrypt.h"
#include<fstream>

#ifndef DECODE_H
#define DECODE_H

class decode {
	private:
		decrypt decryption;					// decryption
		std::ofstream output;				// output file stream
		std::streampos begin, end;			// begin and end position of the input file
		long long input_size, output_size;	// size of input and output file
		long long raw_size;					// size of raw file
		bits buffer;						// store the bits that have not been input
		dictionary dict;					// the dictionary
		void write(byte);					// write to output file
		void write(bytes);					// write to output file
		byte read();						// read a byte
		bits read(int);						// read from input file
	public:
		void start_decode();							// start decode the file
		decode(const char*, const char*, const char*);	// constructor
};

void decode::write(byte output_byte) {
	char tmp = output_byte;
	output.write(&tmp, 1);
	output_size += 1;
}

void decode::write(bytes output_bytes) {
	for (auto output_byte: output_bytes)
		write(output_byte);
}

byte decode::read() {
	byte tmp = decryption.get_next_decrypted();
	input_size += 1;
	return tmp;
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
	while(output_size < raw_size) {
		bits tmp = read(dict.next_length());
		if (!rand())
			printf("Deompressed %.2lf%% (%lld / %lld)\n", 100. * input_size / ((long long) (end - begin)), 
														  input_size, (long long) (end - begin));
		write(dict.decode(tmp));
	}
	printf("Deompressed 100.00%% (%lld / %lld)\n", (long long) (end - begin), (long long) (end - begin));
}

decode::decode (const char *input_file, const char *output_file, const char *password) {
	output.open(output_file, std::ios::out | std::ios::binary);
	decryption.set_decrypt(password, input_file);
	input_size = output_size = 0;
	std::ifstream input(input_file, std::ios::in | std::ios::binary);
	begin = input.tellg();
	input.seekg (0, std::ios::end);
	end = input.tellg();
	input.close();
	raw_size = 0;
	for (int i = 0; i < 8; i++) {
		byte next_byte = read();
		raw_size = raw_size * 256 + next_byte;
	}
}

#endif
