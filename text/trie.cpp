#include "trie.hpp"
#include <stack>

using namespace std;

struct Trie::node
{
	node* son[256];
	unsigned char key;
	bool is_pattern;

	node(const unsigned char new_key=0, bool new_is_pattern=false): key(new_key), is_pattern(new_is_pattern)
	{
		for(int i=0; i<256; ++i)
			son[i]=NULL;
	}

	~node()
	{
		for(int i=0; i<256; ++i)
			if(this->son[i]!=NULL) delete this->son[i];
	}
};

Trie::Trie(): root(new node)
{}

Trie::~Trie()
{delete this->root;}

bool Trie::search(const string& name) const
{
	node* actual_node=this->root;
	for(string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son[static_cast<unsigned char>(*i)]==NULL)
			return false;
		actual_node=actual_node->son[static_cast<unsigned char>(*i)];
	}
return actual_node->is_pattern;
}

void Trie::insert(const string& name)
{
	node* actual_node=this->root;
	for(string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son[static_cast<unsigned char>(*i)]==NULL)
			actual_node->son[static_cast<unsigned char>(*i)]=new node(*i);
		actual_node=actual_node->son[static_cast<unsigned char>(*i)];
	}
	actual_node->is_pattern=true;
}

void Trie::erase(const string& name)
{
	stack<node*> nodes_stack;
	nodes_stack.push(this->root);
	for(string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(nodes_stack.top()->son[static_cast<unsigned char>(*i)]==NULL)
			return;
		nodes_stack.push(nodes_stack.top()->son[static_cast<unsigned char>(*i)]);
	}
	nodes_stack.top()->is_pattern=false;
	node* removed_node;
	while(nodes_stack.size()>1)
	{
		removed_node=nodes_stack.top();
		for(int i=0; i<256; ++i)
			if(removed_node->son[i]!=NULL) return;
		nodes_stack.pop();
		delete nodes_stack.top()->son[removed_node->key];
	}
}