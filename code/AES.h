#ifndef AES_H
#define AES_H

class AES {
	protected:
		static const bytes rcon[10];				// the rcon values
		static const bytes forward_mc, inverse_mc;	// the mixcolumns matrix
		static const bytes forward_S, inverse_S;	// the S tables
		bytes cipher_key, round_keys[10];			// the cipher key and 10 round keys (matrix form)
		bytes block;								// the block being processed
		static unsigned int rightrotate(unsigned int, int);	// right rotate
		static bytes SHA256(const char*);					// hashing password into 256 bit
		void key_schedule();						// the key schedule
		void sub_bytes();							// the sub bytes step
		void shift_rows();							// the shift rows step
		void mix_columns();							// the mix columns step
		void add_round_key(bytes);					// the add round key step
		void r_sub_bytes();							// the reverse sub bytes step
		void r_shift_rows();						// the reverse shift rows step
		void r_mix_columns();						// the reverse mix columns step
		void r_add_round_key(bytes);				// the reverse add round key step
};

unsigned int AES::rightrotate(unsigned int x, int c) {
	return (x >> c) | (x << (32 - c));
}

bytes AES::SHA256(const char* password) {
	bits message;
	long long l = strlen(password);
	for (int i = 0; i < l; i++) {
		byte tmp = byte(password[i]);
		for (int j = 1 << 7; j; j /= 2) {
			message.push_back(tmp / j);
			tmp %= j;
		}
	}
	message.push_back(1);
	while (message.size() % 512 != 448)
		message.push_back(0);
	l *= 8;
	for (unsigned long long i = 1ull << 63; i; i /= 2) {
		message.push_back(l / i);
		l %= i;
	}
	unsigned int H[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
	unsigned int k[] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
						0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
						0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
						0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
						0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
						0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
						0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
						0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
	for (int i = 0; i < message.size(); i += 512) {
		unsigned int w[64];
		for (int j = 0; j < 16; j++) {
			w[j] = 0;
			for (int k = 0; k < 32; k++) 
				w[j] = w[j] * 2 + (message[i + j * 32 + k] ? 1 : 0);
		}
		for (int j = 16; j < 64; j++) {
			unsigned int s0 = rightrotate(w[j - 15], 7) ^ rightrotate(w[j - 15], 18) ^ (w[j - 15] >> 3);
			unsigned int s1 = rightrotate(w[j - 2], 17) ^ rightrotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
			w[j] = w[j - 16] + s0 + w[j - 7] + s1;
		}
		unsigned int a = H[0];
		unsigned int b = H[1];
		unsigned int c = H[2];
		unsigned int d = H[3];
		unsigned int e = H[4];
		unsigned int f = H[5];
		unsigned int g = H[6];
		unsigned int h = H[7];
		for (int j = 0; j < 64; j++) {
			unsigned int s1 = rightrotate(e, 6) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
			unsigned int ch = (e & f) ^ ((~e) & g);
			unsigned int temp1 = h + s1 + ch + k[j] + w[j];
			unsigned int s0 = rightrotate(a, 2) ^ rightrotate(a, 13) ^ rightrotate(a, 22);
			unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
			unsigned int temp2 = s0 + maj;
			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;
	}
	bytes hashed_message;
	for (int i = 0; i < 8; i++) {
		for (int j = 1 << 24; j; j /= 1 << 8) {
			hashed_message.push_back(H[i] / j);
			H[i] %= j;
		}
	}
	return hashed_message;
}

void AES::key_schedule(){
	for (int i=0;i<16;i++){			// generate the round key 1 with cipher key
		if (i==0){
			round_keys[0][i] = cipher_key[i]^forward_S[cipher_key[i+1]]^rcon[0]; 
		}
		else if (0<i && i<4){
			round_keys[0][i] = cipher_key[i]^forward_S[cipher_key[(i+1)%4]]^0;
		}
		else {
			round_keys[0][i] = cipher_key[i]^round_keys[0][i-4];
		}
	}
	for (int i=0;i<9;i++){			// generate the round key 2-10
		for (int j=0;j<16;j++){
			if (j==0){
				round_keys[i+1][j] = round_keys[i][j]^forward_S[round_keys[i][12+j+1]]^rcon[0];
			}
			else if (0<j && j<4){
				round_keys[i+1][j] = round_keys[i][j]^forward_S[round_keys[i][12+(j+1)%4]]^0;
			}
			else {
				round_keys[i+1][j] = round_keys[i][j]^round_keys[i+1][j-4];
			}
		}
	}
}

void AES::sub_bytes(){
	for (int i=0;i<16;i++){
		block[i] = forward_S[block[i]];
	}
}

void AES::shift_rows(){
	bytes tem;
	tem = block[4]; // rotate over 1 byte
	block[4] = block[5];
	block[5] = block[6];
	block[6] = block[7];
	block[7] = tem;
	tem = block[8]; // rotate over 2 byte
	block[8] = block[10];
	block[10] = tem;
	tem = block[9];
	block[9] = block[11];
	block[11] = tem;
	tem = block[12]; // rotate over 3 byte
	block[12] = block[15];
	block[15] = block[14];
	block[14] = block[13];
	block[13] = tem;
}

void AES::mix_columns(){
	bytes tem[4];		// save the new value in tem first
	for (int i =0;i<4;i++){
		tem[0] = 2*block[i*4]^3*block[i*4+1]^1*block[i*4+2]^1*block[i*4+3];	//2 3 1 1
		tem[1] = 1*block[i*4]^2*block[i*4+1]^3*block[i*4+2]^1*block[i*4+3];	//1 2 3 1
		tem[2] = 1*block[i*4]^1*block[i*4+1]^2*block[i*4+2]^3*block[i*4+3];	//1 1 2 3
		tem[3] = 3*block[i*4]^1*block[i*4+1]^1*block[i*4+2]^2*block[i*4+3];	//3 1 1 2
		block[i*4] = tem[0];
		block[i*4+1] = tem[1];
		block[i*4+2] = tem[2];
		block[i*4+3] = tem[3];
	}
}

void AES::add_round_key(bytes turn){
	if (turn == 0){
		block[i] = block[i]^cipher_key[i];	// xor with the cipher key(in the initial round)
	}
	else{
		for (int i = 0; i<16;i++)
			block[i] = block[i]^round_keys[turn-1][i];	// xor with round keys 1-10
	}
}

void AES::r_sub_bytes(){
	for (int i=0;i<16;i++){
		block[i] = inverse_S[block[i]];
	}
}

void AES::r_shift_rows(){
	bytes tem;
	tem = block[12]; // rotate over 1 byte (reverse of rotate over 3 byte)
	block[12] = block[13];
	block[13] = block[14];
	block[14] = block[15];
	block[15] = tem;
	tem = block[8]; // rotate over 2 byte (reverse of rotate over 2 byte)
	block[8] = block[10];
	block[10] = tem;
	tem = block[9];
	block[9] = block[11];
	block[11] = tem;
	tem = block[4]; // rotate over 3 byte (reverse of rotate over 1 byte)
	block[4] = block[7];
	block[7] = block[6];
	block[6] = block[5];
	block[5] = tem;
}

void AES::r_mix_columns(){
	bytes tem[4];
	for (int i =0;i<4;i++){
		tem[0] = 14*block[i*4]^11*block[i*4+1]^13*block[i*4+2]^9*block[i*4+3];	//14 11 13 09
		tem[1] = 9*block[i*4]^14*block[i*4+1]^11*block[i*4+2]^13*block[i*4+3];	//09 14 11 13
		tem[2] = 13*block[i*4]^9*block[i*4+1]^14*block[i*4+2]^11*block[i*4+3];	//13 09 14 11
		tem[3] = 11*block[i*4]^13*block[i*4+1]^9*block[i*4+2]^14*block[i*4+3];	//11 13 09 14
		block[i*4] = tem[0];
		block[i*4+1] = tem[1];
		block[i*4+2] = tem[2];
		block[i*4+3] = tem[3];
	}
}

void AES::r_add_round_key(bytes type){
	add_round_key(turn);
}

#endif
