#include"AES.h"
#include<fstream>

#ifndef DECRYPT_H
#define DECRYPT_H

class decrypt: public AES {
	private:
		std::ifstream input;						// input file stream
		void read_n_decrypt();						// read a block from file and decrypt
	public:
		byte get_next_decrypted();					// get the next decrypted byte
		void set_decrypt(const char*, const char*);	// set up the variables
		decrypt() {}								// constructor
};

void decrypt::read_n_decrypt() {
	char tmp[16];
	input.read(tmp, 16);
	for (int i = 0; i < 16; i++)
		block.push_back(tmp[i]);
	// do the decryption here
	for (int i = 14; i > 0; i--){
		r_add_round_key(i);
		if (i < 14)				// do not run at round 14
			r_mix_columns();
		r_shift_rows();
		r_sub_bytes();
	}
	r_add_round_key(0); // xor with cipher key
	// finish decryption
}

byte decrypt::get_next_decrypted() {
	if (block.size() == 0)
		read_n_decrypt();
	byte next_byte = block[0];
	block.erase(block.begin());
	return next_byte;
}

void decrypt::set_decrypt(const char* password, const char* input_file) {
	input.open(input_file, std::ios::in | std::ios::binary);
	cipher_key = SHA256(password);
	key_schedule();
}


#endif
