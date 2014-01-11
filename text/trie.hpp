#include <string>
#include <map>

#if __cplusplus >= 201103L
#include <mutex>
#endif

#pragma once

// Trie

template<class T>
class Trie
{
protected:
#if __cplusplus >= 201103L
	std::mutex once_operation;
#endif
	struct node
	{
		bool is_pattern;
		unsigned char key;
		node* parent;
		node* son[256];
		T* value;

		node(node* new_parent, const unsigned char new_key=0, bool new_is_pattern=false): is_pattern(new_is_pattern), key(new_key), parent(new_parent)
		{
			for(int i=0; i<256; ++i)
				son[i]=NULL;
		}

		~node()
		{
			if(this->is_pattern)
				delete this->value;
			for(int i=0; i<256; ++i)
				if(this->son[i]!=NULL) delete this->son[i];
		}
	};
	node* root;
public:
	class iterator : public std::iterator<std::input_iterator_tag, T>
	{
		node* p;
		iterator(node* x): p(x){}
	public:
		iterator(): p(NULL){}
		iterator(const iterator& _it): p(_it.p){}
		bool operator==(const iterator& _it) {return this->p==_it.p;}
		bool operator!=(const iterator& _it) {return this->p!=_it.p;}
		T& operator*() {return *this->p->value;}
		T* operator->() {return this->p->value;}
		friend class Trie;
	};

	Trie(): root(new node(NULL)){}

	~Trie()
	{delete this->root;}

#if __cplusplus >= 201103L
	void lock(){this->once_operation.lock();}
	void unlock(){this->once_operation.unlock();}
#endif

	iterator end() const
	{return iterator(NULL);}

	iterator find(const std::string&) const;

	/*
	* second is true if all is OK or
	* false if element already exists.
	*/
	std::pair<iterator, bool> insert(const std::string&);
	void erase(const std::string&);
	std::string get_name(const iterator&);
};

// CompressedTrie

template<class T>
class CompressedTrie
{
protected:
#if __cplusplus >= 201103L
	std::mutex once_operation;
#endif
	struct node
	{
		typedef std::map<unsigned char, node*> son_type;

		bool is_pattern;
		unsigned char key;
		node* parent;
		son_type son;
		T* value;

		node(node* new_parent, const unsigned char new_key=0, bool new_is_pattern=false): is_pattern(new_is_pattern), key(new_key), parent(new_parent){}

		~node()
		{
			if(this->is_pattern)
				delete this->value;
			for(typename son_type::iterator i=this->son.begin(); i!=this->son.end(); ++i)
				delete i->second;
		}
	};
	node* root;
public:
	class iterator : public std::iterator<std::input_iterator_tag, T>
	{
		node* p;
		iterator(node* x): p(x){}
	public:
		iterator(): p(NULL){}
		iterator(const iterator& _it): p(_it.p){}
		bool operator==(const iterator& _it) {return this->p==_it.p;}
		bool operator!=(const iterator& _it) {return this->p!=_it.p;}
		T& operator*() {return *this->p->value;}
		T* operator->() {return this->p->value;}
		friend class CompressedTrie;
	};

	CompressedTrie(): root(new node(NULL)){}

	~CompressedTrie()
	{delete this->root;}

#if __cplusplus >= 201103L
	void lock(){this->once_operation.lock();}
	void unlock(){this->once_operation.unlock();}
#endif

	iterator end() const
	{return iterator(NULL);}

	iterator find(const std::string&) const;

	/*
	* second is true if all is OK or
	* false if element already exists.
	*/
	std::pair<iterator, bool> insert(const std::string&);
	void erase(const std::string&);
	std::string get_name(const iterator&);
};

// Methods of Trie and CompressedTrie

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
typename Trie<T>::iterator Trie<T>::find(const std::string& name) const
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
	actual_node->value=new T();
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
	node* actual_node=this->find(name).p;
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
typename CompressedTrie<T>::iterator CompressedTrie<T>::find(const std::string& name) const
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
	actual_node->value=new T();
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
	node* actual_node=this->find(name).p;
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