#include<trie.h>
#include<algorithm>

class dictionary {
	private:
		int size, len;								// size of the dictionary, length of bits
		bytes cur;									// cur bytes
		std::vector<bytes> words;					// corresponding bytes of the index
		trie dictionary_tree;						// a trie storing the dictionary
		bool check_exist(byte) const;				// check if adding next_byte still exists in dictionary_tree
		void update_cur(byte);						// update current position of dictionary_tree
		void add_entry(byte);						// add new entry to dictionary_tree
		static bits code2bits(code) const;			// convert code to bits
		static code bits2code(bits) const;			// convert bits to code
		void increase_size();						// increase size and len
	public:
		int next_length() const;					// return the next encoded bits' length
		bits encode(byte);							// return the next encoded bits
		bytes decode(bits);							// return the next decoded bytes
		dictionary();								// constructor
}

bool dictionary::check_exist(byte next_byte) const {
	return dictionary_tree.exist_child(next_byte);
}

void dictionary::update_cur(byte next_byte) {
	dictionary_tree.go_to_child(next_byte);
	cur.push_back(next_byte);
}

void dictionary:: add_entry(byte next_byte) {
	cur.push_back(next_byte);
	dictionary_tree.add_new_node(next_byte, code(size));
	increase_size();
	words.push_back(cur);
	cur.clear();
}

static bits dictionary::code2bits(code val) const {
	bits tmp;
	for (int i = 0; i < len; i++) {
		tmp.push_back(std::bitset<1> (val % 2));
		val /= 2;
	}
	std::reverse(tmp.begin(), tmp.end());
	return tmp;
}

static code dictionary::bits2code(bits raw) const {
	int val = 0;
	for (auto a_bit: raw)
		val = val * 2 + a_bit[0];
	return code(val);
}

void dictionary::increase_size() {
	if (size == (size & -size))
		len++;
	size++;
}

int dictionary::next_length() const {
	if (size == (size & -size))
		return len + 1;
	else
		return len;
}

bits dictionary::encode(byte next_byte) {
	if (check_exist(next_byte)) {
		update_cur(next_byte);
		return bits(0);
	}
	else {
		add_entry(next_byte);
		code val = dictionary_tree.get_val();
		dictionary_tree.reset();
		return code2bits(val);
	}
}

bytes dictionary::decode(bits raw) {
	code val = bits2code(raw);
	bytes data = words[int(val)];
	if (cur.size())
		add_entry(data[0]);
	for (auto next_byte: data)
		update_cur(next_byte);
	return data;
}

dictionary::dictionary() {
	size = 256;
	len = 8;
	cur = bytes(0);
	words = std::vector<bytes>(0);
	diciontary_tree = trie();
	for (int i = 0; i < 256; i++)
		add_entry(byte(i));
}
