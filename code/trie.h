#include<bits.h>

class trie;

class node {
	friend class trie;
	private:
		int val;							// value stored on the node
		std::vector<node*> child(256);					// children of the node
		node();								// constructor
		~node();							// destructor
}

class trie {
	private:
		node* cur, root;			// store the current position and the root of the trie
		static int byte2int (byte) const;	// convert byte to int (0~255)
	public:
		bool exist_child(byte) const;		// check if current node has a corresponding child
		void add_new_node(byte);		// add a new child for current node
		void go_to_child(byte);			// change current position to the child node
		void reset();				// reset current position to root
		trie();					// constructor
		~trie();				// destructor
		trie(const trie&);			// copy constructor
		trie& operator= (const trie&);		// copy assignment
		trie(trie&&);				// move constructor
		trie& operator= (trie&&);		// move assignment
}