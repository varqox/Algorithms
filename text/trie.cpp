#include "trie.hpp"

namespace std
{
	template <class BidirectionalIterator>
	void reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		while((first!=last) && (first!=--last))
		{
			iter_swap(first,last);
			++first;
		}
	}
}

// Trie

template<class T>
typename Trie<T>::iterator Trie<T>::search(const std::string& name) const
{
	node* actual_node=this->root;
	for(std::string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son[static_cast<unsigned char>(*i)]==NULL)
			return this->end();
		actual_node=actual_node->son[static_cast<unsigned char>(*i)];
	}
return (actual_node->is_pattern ? iterator(actual_node):this->end());
}

template<class T>
std::pair<typename Trie<T>::iterator, bool> Trie<T>::insert(const std::string& name)
{
#if __cplusplus >= 201103L
	this->once_operation.lock();
#endif
	node* actual_node=this->root;
	for(std::string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son[static_cast<unsigned char>(*i)]==NULL)
			actual_node->son[static_cast<unsigned char>(*i)]=new node(actual_node, *i);
		actual_node=actual_node->son[static_cast<unsigned char>(*i)];
	}
	if(actual_node->is_pattern)
	{
	#if __cplusplus >= 201103L
		this->once_operation.unlock();
	#endif
		return std::make_pair(iterator(actual_node), false);
	}
	actual_node->is_pattern=true;
	actual_node->value=new T;
#if __cplusplus >= 201103L
	this->once_operation.unlock();
#endif
	return std::make_pair(iterator(actual_node), true);
}

template<class T>
void Trie<T>::erase(const std::string& name)
{
#if __cplusplus >= 201103L
	this->once_operation.lock();
#endif
	node* actual_node=this->search(name).p;
	if(actual_node==NULL)
		goto erase_end;
	actual_node->is_pattern=false;
	delete actual_node->value;
	node* removed_node;
	while(actual_node!=this->root)
	{
		removed_node=actual_node;
		for(int i=0; i<256; ++i)
			if(removed_node->son[i]!=NULL)
				goto erase_end;
		actual_node=actual_node->parent;
		actual_node->son[removed_node->key]=NULL; // remove link
		delete removed_node;
	}
erase_end:;
#if __cplusplus >= 201103L
	this->once_operation.unlock();
#endif
}

template<class T>
std::string Trie<T>::get_name(const iterator& _it)
{
	std::string out;
	node* actual_node=_it.p;
	while(actual_node!=this->root)
	{
		out+=actual_node->key;
		actual_node=actual_node->parent;
	}
	std::reverse(out.begin(), out.end());
return out;
}

// CompressedTrie

template<class T>
typename CompressedTrie<T>::iterator CompressedTrie<T>::search(const std::string& name) const
{
	node* actual_node=this->root;
	typename node::son_type::iterator it;
	for(std::string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son.end()==(it=actual_node->son.find(*i)))
			return this->end();
		actual_node=it->second;
	}
return (actual_node->is_pattern ? iterator(actual_node):this->end());
}

template<class T>
std::pair<typename CompressedTrie<T>::iterator, bool> CompressedTrie<T>::insert(const std::string& name)
{
#if __cplusplus >= 201103L
	this->once_operation.lock();
#endif
	node* actual_node=this->root;
	typename node::son_type::iterator it;
	for(std::string::const_iterator i=name.begin(); i!=name.end(); ++i)
	{
		if(actual_node->son.end()==(it=actual_node->son.find(*i)))
			it=actual_node->son.insert(std::make_pair(*i, new node(actual_node, *i))).first;
		actual_node=it->second;
	}
	if(actual_node->is_pattern)
	{
	#if __cplusplus >= 201103L
		this->once_operation.unlock();
	#endif
		return std::make_pair(iterator(actual_node), false);
	}
	actual_node->is_pattern=true;
	actual_node->value=new T;
#if __cplusplus >= 201103L
	this->once_operation.unlock();
#endif
	return std::make_pair(iterator(actual_node), true);
}

template<class T>
void CompressedTrie<T>::erase(const std::string& name)
{
#if __cplusplus >= 201103L
	this->once_operation.lock();
#endif
	node* actual_node=this->search(name).p;
	if(actual_node==NULL)
		goto erase_end;
	actual_node->is_pattern=false;
	delete actual_node->value;
	node* removed_node;
	while(actual_node!=this->root)
	{
		removed_node=actual_node;
		if(!removed_node->son.empty())
			goto erase_end;
		actual_node=actual_node->parent;
		actual_node->son.erase(removed_node->key); // remove link
		delete removed_node;
	}
erase_end:;
#if __cplusplus >= 201103L
	this->once_operation.unlock();
#endif
}

template<class T>
std::string CompressedTrie<T>::get_name(const iterator& _it)
{
	std::string out;
	node* actual_node=_it.p;
	while(actual_node!=this->root)
	{
		out+=actual_node->key;
		actual_node=actual_node->parent;
	}
	std::reverse(out.begin(), out.end());
return out;
}