#include"dictionary.h"
#include"decrypt.h"
#include"timer.h"
#include<fstream>

#ifndef DECODE_H
#define DECODE_H

class decode {
	private:
		timer decode_timer;					// decode timer
		decrypt decryption;					// decryption
		std::ofstream output;				// output file stream
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
		if (!rand()) {
			printf("Deompressed %.2lf%% (%lld / %lld)\n", 100. * output_size / raw_size, output_size, raw_size);
			long long time_passed = decode_timer.time_passed();
			long long expected_remaining_time = 1. * time_passed * (raw_size - output_size) / output_size;
			printf("Time used: %02lld:%02lld:%02lld Expected remaining time: %02lld:%02lld:%02lld\n",
					time_passed / 3600000, time_passed % 3600000 / 60000, time_passed % 60000 / 1000, 
					expected_remaining_time / 3600000, expected_remaining_time % 3600000 / 60000, expected_remaining_time % 60000 / 1000);
		}
		write(dict.decode(tmp));
	}
	printf("Deompressed 100.00%% (%lld / %lld)\n", raw_size, raw_size);
	long long time_passed = decode_timer.time_passed();
	printf("Time used: %02lld:%02lld:%02lld\n", time_passed / 3600000, time_passed % 3600000 / 60000, time_passed % 60000 / 1000);
}

decode::decode (const char *input_file, const char *output_file, const char *password) {
	output.open(output_file, std::ios::out | std::ios::binary);
	input_size = output_size = raw_size = 0;
	std::ifstream input(input_file, std::ios::in | std::ios::binary);
	for (int i = 0; i < 8; i++) {
		char tmp;
		input.read(&tmp, 1);
		raw_size = raw_size * 256 + byte(tmp);
	}
	input.close();
	decryption.set_decrypt(password, input_file);
	long long raw_size_check = 0;
	decode_timer.start();
	for (int i = 0; i < 8; i++)
		raw_size_check = raw_size_check * 256 + read();
	if (raw_size != raw_size_check) {
		printf("Error 01\n");
		printf("The file maybe corrupted or the password is wrong.");
		exit(1);
	}
}

#endif
