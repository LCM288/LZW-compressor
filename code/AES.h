#ifndef AES_H
#define AES_H

class AES {
	protected:
		static const bytes rcon[10];				// the rcon values
		static const bytes forward_mc, inverse_mc;	// the mixcolumns matrix
		static const bytes forward_S, inverse_S;	// the S tables
		bytes cipher_key, round_keys[10];			// the cipher key and 10 round keys (matrix form)
		bytes block;								// the block being processed
		void key_schedule();						// the key schedule
		static bytes MD5(const char* password);		// hashing password into 128 bit
		void sub_bytes();							// the sub bytes step
		void shift_rows();							// the shift rows step
		void mix_columns();							// the mix columns step
		void add_round_key(bytes);					// the add round key step
		void r_sub_bytes();							// the reverse sub bytes step
		void r_shift_rows();						// the reverse shift rows step
		void r_mix_columns();						// the reverse mix columns step
		void r_add_round_key(bytes);				// the reverse add round key step
};

void key_schedule(){
	for (int i=0;i<16;i++){
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
	for (int i=0;i<9;i++){
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

void sub_bytes(){
	for (int i=0;i<16;i++){
		block[i] = forward_S[block[i]];
	}
}

void shift_rows(){
	bytes tem;
	tem[0] = block[4]; // rotate over 1 byte
	block[4] = block[5];
	block[5] = block[6];
	block[6] = block[7];
	block[7] = tem[0];
	tem[0] = block[8]; // rotate over 2 byte
	block[8] = block[10];
	block[10] = tem[0];
	tem[0] = block[9];
	block[9] = block[11];
	block[11] = tem[0];
	tem[0] = block[12]; // rotate over 3 byte
	block[12] = block[15];
	block[15] = block[14];
	block[14] = block[13];
	block[13] = tem[0];
}

void mix_columns(){
	bytes tem[4];
	for (int i =0;i<4;i++){
		tem[0] = 2*block[i*4]^3*block[i*4+1]^1*block[i*4+2]^1*block[i*4+3];
		tem[1] = 1*block[i*4]^2*block[i*4+1]^3*block[i*4+2]^1*block[i*4+3];
		tem[2] = 1*block[i*4]^1*block[i*4+1]^2*block[i*4+2]^3*block[i*4+3];
		tem[3] = 3*block[i*4]^1*block[i*4+1]^1*block[i*4+2]^2*block[i*4+3];
		block[i*4] = tem[0];
		block[i*4+1] = tem[1];
		block[i*4+2] = tem[2];
		block[i*4+3] = tem[3];
	}
}

void add_round_key(bytes turn){
	for (int i = 0; i<16;i++)
		block[i] = block[i]^round_keys[turn][i];
}

void r_sub_bytes(){
	for (int i=0;i<16;i++){
		block[i] = inverse_S[block[i]];
	}
}

void r_shift_rows(){
	bytes tem;
	tem[0] = block[12]; // rotate over 1 byte
	block[12] = block[13];
	block[13] = block[14];
	block[14] = block[15];
	block[15] = tem[0];
	tem[0] = block[8]; // rotate over 2 byte
	block[8] = block[10];
	block[10] = tem[0];
	tem[0] = block[9];
	block[9] = block[11];
	block[11] = tem[0];
	tem[0] = block[4]; // rotate over 3 byte
	block[4] = block[7];
	block[7] = block[6];
	block[6] = block[5];
	block[5] = tem[0];
}

void r_mix_columns(){
	bytes tem[4];
	for (int i =0;i<4;i++){
		tem[0] = 14*block[i*4]^11*block[i*4+1]^13*block[i*4+2]^9*block[i*4+3];
		tem[1] = 9*block[i*4]^14*block[i*4+1]^11*block[i*4+2]^13*block[i*4+3];
		tem[2] = 13*block[i*4]^9*block[i*4+1]^14*block[i*4+2]^11*block[i*4+3];
		tem[3] = 11*block[i*4]^13*block[i*4+1]^9*block[i*4+2]^14*block[i*4+3];
		block[i*4] = tem[0];
		block[i*4+1] = tem[1];
		block[i*4+2] = tem[2];
		block[i*4+3] = tem[3];
	}
}

void r_add_round_key(bytes type){
	add_round_key(turn);
}

#endif
