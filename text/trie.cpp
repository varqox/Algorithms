#include "trie.hpp"
#include <stack>

template<class T>
typename Trie<T>::iterator Trie<T>::search(const string& name) const
{
	node* actual_node=this->root;
	for(string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son[static_cast<unsigned char>(*i)]==NULL)
			return false;
		actual_node=actual_node->son[static_cast<unsigned char>(*i)];
	}
return (actual_node->is_pattern ? iterator(actual_node):this->end());
}

template<class T>
pair<typename Trie<T>::iterator, bool> Trie<T>::insert(const string& name)
{
#if __cplusplus >= 201103L
	this->once_operation.lock();
#endif
	node* actual_node=this->root;
	for(string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son[static_cast<unsigned char>(*i)]==NULL)
			actual_node->son[static_cast<unsigned char>(*i)]=new node(*i);
		actual_node=actual_node->son[static_cast<unsigned char>(*i)];
	}
	if(actual_node->is_pattern)
	{
	#if __cplusplus >= 201103L
		this->once_operation.unlock();
	#endif
		return make_pair(iterator(actual_node), false);
	}
	actual_node->is_pattern=true;
#if __cplusplus >= 201103L
	this->once_operation.unlock();
#endif
	return make_pair(iterator(actual_node), true);
}

template<class T>
void Trie<T>::erase(const string& name)
{
#if __cplusplus >= 201103L
	this->once_operation.lock();
#endif
	stack<node*> nodes_stack;
	nodes_stack.push(this->root);
	for(string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(nodes_stack.top()->son[static_cast<unsigned char>(*i)]==NULL)
			goto erase_end;
		nodes_stack.push(nodes_stack.top()->son[static_cast<unsigned char>(*i)]);
	}
	nodes_stack.top()->is_pattern=false;
	node* removed_node;
	while(nodes_stack.size()>1)
	{
		removed_node=nodes_stack.top();
		for(int i=0; i<256; ++i)
			if(removed_node->son[i]!=NULL)
				goto erase_end;
		nodes_stack.pop();
		nodes_stack.top()->son[removed_node->key]=NULL; // we remove link
		delete removed_node;
	}
erase_end:;
#if __cplusplus >= 201103L
	this->once_operation.unlock();
#endif
}