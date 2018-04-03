#include<fstream>

#ifndef ENCRYPT_H
#define ENCRYPT_H

class encrypt: public AES {
	private:
		ofstream output;				// output file stream
		void encrypt_n_write();				// encrypt the block and write to file
	public:
		void add_to_encrypt(byte byte_to_encrypt);		// add the next byte to encrypt
		bool finished();					// return true if block is empty
		encrypt(const char* password, const char* output_file)	// constructor
};

#endif
