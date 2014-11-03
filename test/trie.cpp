#include "../text/trie.hpp"
#include <string>
#include <cstdlib>
#include <cstdio>

namespace XXX {

struct None {};

template<class BaseNode>
class BaseTrie {
protected:
	typedef unsigned char uchar;

	class Node : public BaseNode {
	private:
		Node(const Node&);
		Node& operator=(const Node&);

	public:
		uchar key_;
		bool ins_; // is this word inserted
		class Sons {
		private:
			short size_, real_size_;
			Node** t_;

			Sons(const Sons&);
			Sons& operator=(const Sons&);

		public:
			Sons(): size_(0), real_size_(0), t_(NULL) {}

			~Sons();
			Node* operator[](uchar c);
			void insert(Node* p);
			void erase(uchar c);
		} sons_;

		Node(uchar key) : key_(key), ins_(false), sons_() {}

		~Node() {}
	};

	Node* root;

	BaseTrie(const BaseTrie&); // Have to write
	BaseTrie& operator=(const BaseTrie&); // Have to write

public:
	BaseTrie(): root(new Node('\0')) {}
	virtual ~BaseTrie() { delete root; }

	Node* insert(const std::string str);
	Node* find(const std::string str);
};

template<class BaseNode>
BaseTrie<BaseNode>::Node::Sons::~Sons() {
	if (t_) {
		for (short i = 0; i < size_; ++i)
			delete t_[i];
		delete[] t_;
	}
}

template<class BaseNode>
typename BaseTrie<BaseNode>::Node* BaseTrie<BaseNode>::Node::Sons::operator[](uchar c) {
	// Simple searching (for speed up)
	if (size_ < 5) {
		for (short i = 0; i < size_; ++i)
			if (t_[i]->key_ == c)
				return t_[i];
		return NULL;
	}
	// Binary serach
	short down = 0, up = size_ - 1, mid;
	while (down < up) {
		mid = (down + up) >> 1;
		if (t_[mid]->key_ == c)
			return t_[mid];
		else if (t_[mid]->key_ < c)
			down = mid + 1;
		else
			up = mid - 1;
	}
	if (down == up)
		return t_[up];
	return NULL;
}

template<class BaseNode>
void BaseTrie<BaseNode>::Node::Sons::insert(Node* p) {
	if (t_ == NULL) { // First son
		t_ = new Node*[1];
		size_ = real_size_ = 1;
		t_[0] = p;
	} else {
		if (size_ == real_size_) { // No more space - have to make some
			real_size_ <<= 1;
			Node** x = new Node*[real_size_];
			for (short i = 0; i < size_; ++i)
				x[i] = t_[i];
			delete[] t_;
			t_ = x;
		}
		// Find position
		short pos = size_;
		while (pos > 0 && t_[pos - 1]->key_ > p->key_)
			--pos;
		for (short i = size_; i > pos; --i)
			t_[i] = t_[i-1];
		t_[pos] = p; // Real insert
	}
}

template<class BaseNode>
void BaseTrie<BaseNode>::Node::Sons::erase(uchar c) {
	Node *p = operator[](c); // Find node to erase
	if (p) { // If exists
		// Find position
		short pos = size_;
		while (t_[pos - 1] != p)
			--pos;
		// Move others
		for (; pos < size_; ++pos)
			t_[pos - 1] = t_[pos];
		delete p; // Remove node
		--size_;
	}
}

template<class BaseNode>
typename BaseTrie<BaseNode>::Node* BaseTrie<BaseNode>::insert(const std::string str) {
	size_t i = 0, len = str.size();
	Node *p = root, *k;
	// Go through existing nodes
	for (; i < len; ++i) {
		k = p->sons_[str[i]];
		if (k == NULL)
			break;
		p = k;
	}
	// Create missing
	for (; i < len; ++i) {
		k = new Node(str[i]);
		p->sons_.insert(k);
		p = k;
	}
	p->ins_ = true; // Mark insertion
	return p;
}

template<class BaseNode>
typename BaseTrie<BaseNode>::Node* BaseTrie<BaseNode>::find(const std::string str) {
	size_t i = 0, len = str.size();
	Node *p = root;
	for (; i < len; ++i) {
		p = p->sons_[str[i]];
		if (p == NULL)
			return NULL;
	}
	return p;
}

template<class T>
class TrieNode {
private:
	TrieNode(const TrieNode&);
	TrieNode& operator=(const TrieNode&);

public:
	TrieNode* p_;
	T* val_;

	TrieNode(): p_(NULL), val_(NULL) {}
	virtual ~TrieNode() {
		if (val_)
			delete val_;
	}
};

template<class T = None>
class Trie : protected BaseTrie< TrieNode<T> > {
public:
	Trie() {}
	virtual ~Trie() {}

	class iterator {
	protected:
		typename BaseTrie< TrieNode<T> >::Node* p_;
		iterator(typename BaseTrie< TrieNode<T> >::Node* x) : p_(x) {}

	public:
		iterator() : p_(NULL) {}
		iterator(const iterator& it): p_(it.p_) {}
		iterator& operator=(const iterator& it) {
			p_ = it.p_;
			return *this;
		}
		bool operator==(const iterator& it) { return p_ == it.p_; }
		bool operator!=(const iterator& it) { return p_ != it.p_; }
		T& operator*() { return *p_->val_; }
		T* operator->() { return p_->val_; }

		friend class Trie;
	};

	iterator insert(const std::string str, T* val = NULL) {
		typename BaseTrie< TrieNode<T> >::Node* p = BaseTrie< TrieNode<T> >::insert(str);
		p->val_ = val;
		return p;
	}
	iterator insert(const std::string str, T val) { return insert(str, new T(val)); }
	iterator find(const std::string str) { return iterator(BaseTrie< TrieNode<T> >::find(str)); }
	iterator end() const { return NULL; }
};

template<>
class Trie<None> : protected BaseTrie<None> {
private:
	Trie(const Trie&); // Have to write
	Trie& operator=(const Trie&); // Have to write

public:
	Trie() {}
	virtual ~Trie() {}

	void insert(const std::string str) { BaseTrie::insert(str); }

	bool find(const std::string str) {
		Node* p = BaseTrie::find(str);
		return (p == NULL ? false : p->ins_);
	}
};

} // namespace XXX

using std::string;

const int MAX_LEN = 20, N = 1000000;

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
	srand(12984912);
	// Trie::CompressedTrie<XXX::none> trie;
	XXX::Trie<> trie;
	/*XXX::Trie<char> k;
	cout << *k.insert("123", '$') << endl;
	cout << *k.insert("12", ';') << endl;
	cout << *k.find("123") << endl;
	cout << *k.find("12") << endl;*/
	for (size_t i = 0; i < N; ++i) {
		string s;
		for (size_t j = 1 + rand() % MAX_LEN; j > 0; --j)
			s += static_cast<char>('a' + rand() % 26);
		trie.insert(s);
	}
	return 0;
}
