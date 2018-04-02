#include"trie.h"

#ifndef DICTIONARY_H
#define DICTIONARY_H

class dictionary {
	private:
		static const int reconstruct_size = 65536;  // reconstruct size
		int size, len;								// size of the dictionary, length of bits
		bytes cur;									// cur bytes
		std::vector<bytes> words;					// corresponding bytes of the index
		trie dictionary_tree;						// a trie storing the dictionary
		bool check_exist(byte) const;				// check if adding next_byte still exists in dictionary_tree
		void update_cur(byte);						// update current position of dictionary_tree
		void add_entry(byte);						// add new entry to dictionary_tree
		bits code2bits(code);						// convert code to bits
		static code bits2code(bits);				// convert bits to code
		void increase_size();						// increase size and len
		void reconstruct();							// reconstruct the dictionary
	public:
		int get_size() const{ return size; };
		int next_length() const;					// return the next encoded bits' length
		bits encode(byte);							// return the next encoded bits
		bytes decode(bits);							// return the next decoded bytes
		dictionary();								// constructor
};

bool dictionary::check_exist(byte next_byte) const {
	return dictionary_tree.exist_child(next_byte);
}

void dictionary::update_cur(byte next_byte) {
	dictionary_tree.go_to_child(next_byte);
	cur.push_back(next_byte);
}

void dictionary::add_entry(byte next_byte) {
	cur.push_back(next_byte);
	dictionary_tree.add_new_node(next_byte, code(size));
	increase_size();
	words.push_back(cur);
	cur.clear();
}

bits dictionary::code2bits(code val) {
	bits tmp(len);
	for (int i = len - 1; i >= 0; i--) {
		tmp[i] = (int(val) % 2);
		val = code(int(val) / 2);
	}
	return tmp;
}

code dictionary::bits2code(bits raw) {
	int val = 0;
	for (auto a_bit: raw)
		val = val * 2 + a_bit;
	return code(val);
}

void dictionary::increase_size() {
	if (size == (size & -size) && size >= 2)
		len++;
	size++;
}

void dictionary::reconstruct() {
	size = 0;
	len = 1;
	cur.clear();
	words.clear();
	dictionary_tree.reconstruct();
	for (int i = 0; i < 256; i++)
		add_entry(byte(i));
}

int dictionary::next_length() const {
	int next_size = size + 1;
	if (next_size == (next_size & -next_size) || size == (size & -size) && size >= 2)
		return len + 1;
	else
		return len;
}

bits dictionary::encode(byte next_byte) {
	if (check_exist(next_byte)) {
		code val = dictionary_tree.get_val();
		update_cur(next_byte);
		if (int(dictionary_tree.get_val()) == size - 1) {			// if the code is too new, it cannot be decoded
			increase_size();
			words.push_back(cur);
			cur.clear();
			dictionary_tree.reset();
			bits output = code2bits(val);
			if (size == reconstruct_size)
				reconstruct();
			update_cur(next_byte);
			return output;
		}
		return bits(0);
	}
	else {
		add_entry(next_byte);
		code val = dictionary_tree.get_val();
		dictionary_tree.reset();
		bits output = code2bits(val);
		if (size == reconstruct_size)
			reconstruct();
		update_cur(next_byte);
		return output;
	}
}

bytes dictionary::decode(bits raw) {
	code val = bits2code(raw);
	bytes data = words[int(val)];
	if (cur.size())
		add_entry(data[0]);
	dictionary_tree.reset();
	for (auto next_byte: data)
		update_cur(next_byte);
	if (size == reconstruct_size - 1)
		reconstruct();
	return data;
}

dictionary::dictionary() {
	size = 0;
	len = 1;
	for (int i = 0; i < 256; i++)
		add_entry(byte(i));
}

#endif
