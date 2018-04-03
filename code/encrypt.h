#include<fstream>
#include"AES.h"

#ifndef ENCRYPT_H
#define ENCRYPT_H

class encrypt: public AES {
	private:
		std::ofstream output;						// output file stream
		void encrypt_n_write();						// encrypt the block and write to file
	public:
		void add_to_encrypt(byte);					// add the next byte to encrypt
		bool finished();							// return true if block is empty
		void set_encrypt(const char*, const char*);	// set up the variables
		encrypt() {}								// constructor
};

void encrypt::encrypt_n_write() {
	// do the encryption here
	key_schedule(); // not yet decided, generate key?
	add_round_key(0); // xor with cipher key
	for (int i=0;i<10;i++){
		sub_bytes(); 
		shift_rows();
		if (i<9) 				// do not run at round 10
			mix_columns();
		add_round_key(bytes(i+1));
	}
	// finish encryption
	for (auto next_byte: block) {
		char tmp = char(next_byte);
		output.write(&tmp, 1);
	}
	block.clear();
}

void encrypt::add_to_encrypt(byte byte_to_encrypt) {
	block.push_back(byte_to_encrypt);
	if (block.size() == 16)
		encrypt_n_write();
}

bool encrypt::finished() {
	return block.empty();
}

void encrypt::set_encrypt(const char* password, const char* output_file) {
	output.open(output_file, std::ios::out | std::ios::binary);
	cipher_key = SHA256(password);
	key_schedule();
}

#endif
