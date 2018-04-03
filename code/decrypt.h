#include"AES.h"
#include<fstream>

#ifndef DECRYPT_H
#define DECRYPT_H

class decrypt: public AES {
	private:
		ifstream input;					// input file stream
		void read_n_decrypt();				// read a block from file and decrypt
	public:
		bytes get_next_decrypted();				// get the next decrypted byte
		bool finished();					// return true if block is empty
		decrypt();						// constructor
		decrypt(const char* password, const char* input_file);	// constructor
};
		
#endif
