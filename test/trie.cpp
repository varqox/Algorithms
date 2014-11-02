#include "../text/trie.hpp"
#include <string>
#include <cstdlib>
#include <cstdio>

namespace XXX {

struct none {};

template<class T = none>
class Trie {
private:
	typedef unsigned char uchar;
	class Node {
	private:
		Node(const Node&);
		Node& operator=(const Node&);

	public:
		uchar key_;
		Node* p_; // parent
		class Sons {
		private:
			short size_, real_size_;
			Node** t_;

		public:
			Sons(): size_(0), real_size_(0), t_(NULL) {}

			~Sons();
			Node* operator[](uchar c);
			void insert(Node* p);
			void erase(uchar c);
		} sons_;
		T* val_; // value

		Node(uchar key, Node* p = NULL, T* val = NULL) : key_(key), p_(p), sons_(), val_(val) {}

		~Node() {
			if (val_)
				delete val_;
		}
	};

	Node* root;

	Trie(const Trie&); // Have to write
	Trie& operator=(const Trie&); // Have to write

public:
	Trie(): root(new Node('\0')) {}

	~Trie() { delete root; }

	class iterator {
	protected:
		Node* p_;
		iterator(Node* x) : p_(x) {}

	public:
		iterator() : p_(NULL) {}

		iterator(const iterator& it): p_(it.p_) {}

		iterator& operator=(const iterator& it) {
			p_ = it.p_;
			return *this;
		}

		bool operator==(const iterator& it) { return p_ == it.p_; }

		bool operator!=(const iterator& it) { return p_ != it.p_; }

		T& operator*() { return *p_->value; }

		T* operator->() { return p_->value; }

		friend class Trie;
	};

	void insert(const std::string str, T* val = new T);
	iterator find(const std::string str);

	iterator end() const { return NULL; }
};

template<class T>
Trie<T>::Node::Sons::~Sons() {
	if (t_) {
		for (short i = 0; i < size_; ++i)
			delete t_[i];
		delete[] t_;
	}
}

template<class T>
typename Trie<T>::Node* Trie<T>::Node::Sons::operator[](uchar c) {
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

template<class T>
void Trie<T>::Node::Sons::insert(Node* p) {
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

template<class T>
void Trie<T>::Node::Sons::erase(uchar c) {
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

template<class T>
void Trie<T>::insert(const std::string str, T* val) {
	size_t i = 0, len = str.size();
	Node *p = root, *k;
	for (; i < len; ++i) {
		k = p->sons_[str[i]];
		if (k == NULL)
			break;
		p = k;
	}
	for (; i < len; ++i) {
		k = new Node(str[i], p);
		p->sons_.insert(k);
		p = k;
	}
	p->val_ = val;
}

template<class T>
typename Trie<T>::iterator Trie<T>::find(const std::string str) {
	size_t i = 0, len = str.size();
	Node *p = root;
	for (; i < len; ++i) {
		p = p->sons_[str[i]];
		if (p == NULL)
			return NULL;
	}
	return p;
}

} // namespace XXX

using std::string;

const int MAX_LEN = 20, N = 1000000;


int main() {
	/*XXX::none* p;
	p = new XXX::none;
	printf("%i\n", sizeof(*	p));
	printf("%p\n", p);
	return 0;*/
	srand(12984912);
	// Trie::CompressedTrie<XXX::none> trie;
	XXX::Trie<> trie;
	/*trie.insert("abc");
	trie.insert("ab");
	printf("%i\n", trie.end() != trie.find("abc"));
	printf("%i\n", trie.end() != trie.find("ab"));
	printf("%i\n", trie.end() != trie.find("abb"));
	return 0;*/
	for (size_t i = 0; i < N; ++i) {
		string s;
		for (size_t j = 1 + rand() % MAX_LEN; j > 0; --j)
			s += static_cast<char>('a' + rand() % 26);
		trie.insert(s);
	}
	return 0;
}