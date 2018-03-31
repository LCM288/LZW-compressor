#include<bits.h>

 node {
		code val;							// value stored on the node
		node* child[256];						// children of the node
		node();								// constructor
		node(code);							// constructor
		~node();							// destructor
}

class trie {
	private:
		node* cur, root;			// store the current position and the root of the trie
		static int byte2int (byte) const;	// convert byte to int (0~255)
	public:
		int get_val() const;			// get value of current position
		bool exist_child(byte) const;		// check if current node has a corresponding child
		void add_new_node(byte, code);		// add a new child for current node
		void go_to_child(byte);			// change current position to the child node
		void reset();				// reset current position to root
		trie();					// constructor
		~trie();				// destructor
}

node::node(){
	val = code(0);
	for (int i = 0;i<256;i++){
		child[i] = NULL;
	}
}

node::node(code set_val){
	val = set_val;
	for (int i = 0;i<256;i++){
		child[i] = NULL;
	}
}

node::~node(){
	for (int i=0;i<256;i++){
		if (child[i] != NULL){
			delete child[i];
		}
	}
}

static int trie::byte2int (byte character) const{
	return int(character);
}
int trie::get_val() const{
	return cur->val;
}

bool trie::exist_child(byte character) const{
	if (child[byte2int(character)]!=NULL){
		return true;
	}
	else {
		return false;
	}
}

void trie::add_new_node(byte character, code val){
	cur -> child[byte2int(character)] = new node(val);
}

void trie::go_to_child(byte character){
	cur = cur -> child[byte2int(character)];
}
void trie::reset(){
	cur = root;
}


trie::trie(){
	root = new node();
	cur = root;
}

trie::~trie(){
	delete root;
}
