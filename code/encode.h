#include"dictionary.h"
#include"encrypt.h"
#include"timer.h"
#include<fstream>

#ifndef ENCODE_H
#define ENCODE_H

class encode {
	private:
		timer encode_timer;					// encode timer
		encrypt encryption;					// encryption
		std::ifstream input;				// input file stream
		long long input_size, output_size;	// size of input and output file
		long long raw_size;					// size of raw file
		bits buffer;						// store the bits that have not been output
		dictionary dict;					// the dictionary
		void write(byte);					// add a specific byte to be encrypted
		void write(bits);					// write to output file
		byte read();						// read from input file 
	public:
		void start_encode();							// start encode the file
		encode(const char*, const char*, const char*);	// constructor
};

void encode::write(byte output_byte) {
	encryption.add_to_encrypt(output_byte);
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
		if (!rand()) {
			printf("Compressed %.2lf%% (%lld / %lld) Compression rate %.2lf%%\n", 100. * input_size / raw_size, input_size, raw_size, 
																				  100. * output_size / input_size);
			long long time_passed = encode_timer.time_passed();
			long long expected_remaining_time = 1. * time_passed * (raw_size - input_size) / input_size;
			printf("Time used: %02lld:%02lld:%02lld Expected remaining time: %02lld:%02lld:%02lld\n",
					time_passed / 3600000, time_passed % 3600000 / 60000, time_passed % 60000 / 1000,  
					expected_remaining_time / 3600000, expected_remaining_time % 3600000 / 60000, expected_remaining_time % 60000 / 1000);
		}
	}
	if (buffer.size()) {
		while (buffer.size() < 8) buffer.push_back(0);
		byte output_byte = 0;
		for (auto a_bit: buffer)
			output_byte = output_byte * 2 + a_bit;
		write(output_byte);
		buffer.clear();
	}
	while (!encryption.finished()) {
		write(byte(0));
	}
	printf("Compressed 100.00%% (%lld / %lld) Compression rate %.2lf%%\n", raw_size, raw_size, 100. * output_size / raw_size);
	long long time_passed = encode_timer.time_passed();
	printf("Time used: %02lld:%02lld:%02lld\n", time_passed / 3600000, time_passed % 3600000 / 60000, time_passed % 60000 / 1000);
}

encode::encode (const char *input_file, const char *output_file, const char *password) {
	input.open(input_file, std::ios::in | std::ios::binary);
	input_size = output_size = 0;
	std::streampos begin, end;
	begin = input.tellg();
	input.seekg(0, std::ios::end);
	end = input.tellg();
	input.seekg(0, std::ios::beg);
	raw_size = (long long) (end - begin);
	long long tmp = raw_size;
	byte next_byte[8];
	for (int i = 7; i >= 0; i--) {
		next_byte[i] = byte(tmp % 256);
		tmp /= 256;
	}
	std::ofstream output(output_file, std::ios::out | std::ios::binary);
	for (int i = 0; i < 8; i++) {
		char tmp = char(next_byte[i]);
		output.write(&tmp, 1);
	}
	output.close();
	encryption.set_encrypt(password, output_file);
	encode_timer.start();
	for (int i = 0; i < 8; i++)
		write(next_byte[i]);
}

#endif
