#ifndef AES_H
#define AES_H

class AES {
	protected:
		static const bytes rcon[10];			// the rcon values
		static const bytes forward_mc, inverse_mc;	// the mixcolumns matrix
		static const bytes forward_S, inverse_S;	// the S tables
		bytes cipher_key, round_keys[10];		// the cipher key and 10 round keys (matrix form)
		bytes block;					// the block being processed
		void key_schedule();				// the key schedule
		static bytes hash?(const char* password);	// hashing password into 128 bit
		void sub_bytes();				// the sub bytes step
		void shift_rows();				// the shift rows step
		void mix_columns();				// the mix columns step
		void add_round_key(bytes);			// the add round key step
		void r_sub_bytes();				// the reverse sub bytes step
		void r_shift_rows();				// the reverse shift rows step
		void r_mix_columns();				// the reverse mix columns step
		void r_add_round_key(bytes);			// the reverse add round key step
};

#endif
