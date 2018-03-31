#include<trie.h>
#include<algorithm>

class dictionary {
	private:
		int size, len;					// size of the dictionary, length of bits
		std::vector<bytes> words;			// corresponding bytes of the index
		trie dictionary_tree;				// a trie storing the dictionary
		bool check_exist(byte) const;			// check if adding next_byte still exists in dictionary_tree
		void update_cur(byte);				// update current position of dictionary_tree
		void add_entry(byte);				// add new entry to dictionary_tree
		static bits int2bits(int) const;		// convert integer to bits
		void increase_size();				// increase size and len
	public:
		bits encode(byte);				// return the next encoded bits
		bytes decode(bits);				// return the next decoded bytes
		dictionary();					// constructor
		~dictionary();					// desctructor
		dictionary(const dictionary&);			// copy constructor
		dictionary& operator= (const dictionary&);	// copy assignment
		dictionary(dictionary&&);			// move constrctor
		dictionary& operator= (dictionary&&);		// move assignment
}

bool dictionary::check_exist(byte next_byte) const {
	return dictionary_tree.exist_child(next_byte);
}

void dictionary::update_cur(byte next_byte) {
	dictionary_tree.go_to_child(next_byte);
}

void dictionary:: add_entry(byte next_byte, int val) {
	dictionary_tree.add_new_node(next_byte, val);
	increase_size();
}

static bits dictionary::int2bits(int val) const {
	bits tmp;
	for (int i = 0; i < len; i++) {
		tmp.push_back(std::bitset<1> (val % 2));
		val /= 2;
	}
	std::reverse(tmp.begin(), tmp.end());
	return tmp;
}

void dictionary::increase_size() {
	if (size == (size & -size))
		len++;
	size++;
}

bits dictionary::encode(byte next_byte) {
	if (check_exist(next_byte)) {
		update_cur(next_byte);
		return bits(0);
	}
	else {
		add_entry(next_byte, size);
		int val = dictionary_tree.get_val();
		dictionary_tree.reset();
		return int2bits(val);
	}
}
