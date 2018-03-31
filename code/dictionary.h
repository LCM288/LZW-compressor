#include<vector>
#include<bits.h>
#include<trie.h>

class dictionary {
	private:
		int _size;					// size of the dictionary
		std::vector<bytes> words;			// corresponding bytes of the index
		trie dictionary_tree;				// a trie storing the dictionary
		bool check_exist(byte) const;			// check if adding next_byte still exists in dictionary_tree
		void update_cur(byte);				// update current position of dictionary_tree
	public:
		int size() const;				// return the size of the dictionary
		bits encode(byte)				// return the next encoded bits
		bytes decode(bits)				// return the next decoded bytes
		dictionary();					// constructor
		~dictionary();					// desctructor
		dictionary(const dictionary&);			// copy constructor
		dictionary& operator= (const dictionary&);	// copy assignment
		dictionary(dictionary&&);			// move constrctor
		dictionary& operator= (dictionary&&);		// move assignment
}
