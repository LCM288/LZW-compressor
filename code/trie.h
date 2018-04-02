#include"bits.h"
#include<map> 

#ifndef TRIE_H
#define TRIE_H

struct node {
		code val;							// value stored on the node
		#ifdef USE_MAP
		std::map<byte, node*> child;
		#else
		node* child[256];					// children of the node
		#endif
		node();								// constructor
		node(code);							// constructor
		~node();							// destructor
};

class trie {
	private:
		node *cur, *root;					// store the current position and the root of the trie
	public:
		code get_val() const;				// get value of current position
		bool exist_child(byte) const;		// check if current node has a corresponding child
		void add_new_node(byte, code);		// add a new child for current node
		void go_to_child(byte);				// change current position to the child node
		void reset();						// reset current position to root
		void reconstruct();					// reconstruct the trie
		trie();								// constructor
		~trie();							// destructor
};

node::node(){
	val = code(0);
	#ifndef USE_MAP
	for (int i = 0; i < 256; i++)
		child[i] = NULL;
	#endif
}

node::node(code set_val){
	val = set_val;
	#ifndef USE_MAP
	for (int i = 0; i < 256; i++)
		child[i] = NULL;
	#endif
}

node::~node(){
	for (int i = 0; i < 256; i++){
		#ifdef USE_MAP
		if (child.find(byte(i)) != child.end())
			delete child[byte(i)];
		#else
		if (child[i] != NULL)
			delete child[i];
		#endif
	}
}

code trie::get_val() const{
	return cur -> val;
}

bool trie::exist_child(byte character) const{
	#ifdef USE_MAP
	if (cur -> child.find(character) != cur -> child.end())
	#else
	if (cur -> child[int(character)] != NULL)
	#endif
		return true;
	else
		return false;
}

void trie::add_new_node(byte character, code val){
	#ifdef USE_MAP
	cur -> child[character] = new node(val);
	#else
	cur -> child[int(character)] = new node(val);
	#endif
}

void trie::go_to_child(byte character){
	#ifdef USE_MAP
	cur = cur -> child[character];
	#else
	cur = cur -> child[int(character)];
	#endif
}
void trie::reset(){
	cur = root;
}

void trie::reconstruct() {
	delete root;
	cur = root = new node();
}

trie::trie(){
	root = new node();
	cur = root;
}

trie::~trie(){
	delete root;
}
#endif
