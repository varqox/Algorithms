#pragma once

#include <map>
#include <string>
#include <vector>

class compressed_aho_base
{
protected:
	// node of Compressed Trie tree
	struct node
	{
		typedef node* _ptr;
		typedef std::multimap<unsigned char, _ptr> son_type;

		bool is_pattern;
		unsigned char key;
		unsigned pattern_id; // pattern id
		_ptr fail, long_sh_pat; // fail pointer, the longest shorter pattern
		son_type son;

		node(unsigned char _k = 0, bool _ip = false, _ptr _f = NULL, _ptr _l = NULL): is_pattern(_ip), key(_k), pattern_id(), fail(_f), long_sh_pat(_l), son()
		{}

		node(const node& nd): is_pattern(nd.is_pattern), key(nd.key), pattern_id(nd.pattern_id), fail(nd.fail), long_sh_pat(nd.long_sh_pat), son(nd.son)
		{}

		node& operator=(const node& nd)
		{
			is_pattern = nd.is_pattern;
			key = nd.key;
			pattern_id = nd.pattern_id;
			fail = nd.fail;
			long_sh_pat = nd.long_sh_pat;
			son = nd.son;
			return *this;
		}

		~node()
		{
			for(son_type::iterator i = son.begin(); i != son.end(); ++i)
				delete i->second;
		}

		void copy(_ptr nd)
		{
			is_pattern = nd->is_pattern;
			pattern_id = nd->pattern_id;
			key = nd->key;
			son.clear();
			for(son_type::iterator i = nd->son.begin(); i != nd->son.end(); ++i)
				son.insert(std::make_pair(i->first, new node(*i->second)))->second->copy(i->second);
		}
	};

	node::_ptr root;

	void add_fails();

	// returns real id of inserted word
	unsigned insert(const std::string& word, unsigned id);

	compressed_aho_base(): root(new node)
	{
		root->fail = root->long_sh_pat = root;
	}

	compressed_aho_base(const compressed_aho_base& ab): root(new node)
	{
		root->copy(ab.root);
		root->fail = root->long_sh_pat = root;
		add_fails();
	}

	compressed_aho_base& operator=(const compressed_aho_base& ab)
	{
		delete root;
		root = new node;
		root->copy(ab.root);
		root->fail = root->long_sh_pat = root;
		add_fails();
		return *this;
	}

	virtual ~compressed_aho_base()
	{
		delete root;
	}
};

class compressed_aho : public compressed_aho_base
{
protected:
	std::vector<std::vector<unsigned>*> fin; // finded patterns
	std::vector<unsigned> fin_id; // tells real (in Trie) id of pattern

public:
	compressed_aho(): compressed_aho_base(), fin(), fin_id()
	{}

	compressed_aho(const compressed_aho& _a): compressed_aho_base(_a), fin(_a.fin), fin_id(_a.fin_id)
	{
		for(unsigned i = 0, s = fin.size(); i < s; ++i)
		{
			if(fin_id[i] == i)
				fin[i] = new std::vector<unsigned>(*fin[i]);
			else
				fin[i] = fin[fin_id[i]];
		}
	}

	compressed_aho& operator=(compressed_aho& _a)
	{
		compressed_aho_base::operator=(_a);
		fin = _a.fin;
		fin_id = _a.fin_id;
		for(unsigned i = 0, s = fin.size(); i < s; ++i)
		{
			if(fin_id[i] == i)
				fin[i] = new std::vector<unsigned>(*fin[i]);
			else
				fin[i] = fin[fin_id[i]];
		}
		return *this;
	}

	virtual ~compressed_aho()
	{
		for(unsigned i = 0, s = fin.size(); i < s; ++i)
			if(fin_id[i] == i)
				delete fin[i];
	}

	void swap(compressed_aho& _a)
	{
		std::swap(root, _a.root);
		std::swap(fin, _a.fin);
		std::swap(fin_id, _a.fin_id);
	}

	std::vector<std::vector<unsigned>*>::size_type size() const
	{return fin.size();}

	std::vector<unsigned>& operator[](std::vector<std::vector<unsigned>*>::size_type n)
	{return *fin[n];}

	const std::vector<unsigned>& operator[](std::vector<std::vector<unsigned>*>::size_type n) const
	{return *fin[n];}

	void find(const std::vector<std::string>& patterns, const std::string& text);
};

template<class T>
class special_compressed_aho : public compressed_aho_base
{
protected:
	std::vector<int> fin; // finded patterns occurrences
	std::vector<std::pair<std::string, T> > patterns;

public:
	special_compressed_aho(): compressed_aho_base(), fin(), patterns()
	{}

	special_compressed_aho(const special_compressed_aho& _a): compressed_aho_base(_a), fin(_a.fin), patterns(_a.patterns)
	{}

	special_compressed_aho& operator=(special_compressed_aho& _a)
	{
		compressed_aho_base::operator=(_a);
		fin = _a.fin;
		patterns = _a.patterns;
		return *this;
	}

	virtual ~special_compressed_aho()
	{}

	void swap(special_compressed_aho& _a)
	{
		std::swap(root, _a.root);
		std::swap(fin, _a.fin);
		std::swap(patterns, _a.patterns);
	}

	std::vector<unsigned>::size_type size() const
	{return fin.size();}

	int& operator[](std::vector<int>::size_type n)
	{return fin[n];}

	const int& operator[](std::vector<int>::size_type n) const
	{return fin[n];}

	const std::pair<std::string, T>& pattern(typename std::vector<std::pair<std::string, T> >::size_type n) const
	{return patterns[n];}

	void set_patterns(const std::vector<std::pair<std::string, T> >& new_patterns);
	void find(const std::string& text);
};

template<class T>
void special_compressed_aho<T>::set_patterns(const std::vector<std::pair<std::string, T> >& new_patterns)
{
	patterns = new_patterns;
	// clear Trie
	delete root;
	root = new node;
	root->fail = root->long_sh_pat = root;
	// Build Trie
	for(unsigned i = 0, s = patterns.size(); i < s; ++i)
		insert(patterns[i].first, i);
	add_fails(); // add fail and long_sh_pat edges
}

template<class T>
void special_compressed_aho<T>::find(const std::string& text)
{
	// clear fin
	fin.resize(text.size());
	node::_ptr actual = root, found; // actual node and auxiliary pointer (to finding patterns)
	node::son_type::iterator it;
	for(unsigned i = 0, s = text.size(); i < s; ++i)
	{
		// we search for node wich has soon with key == *i
		while((it = actual->son.find(text[i])) == actual->son.end() && actual != root)
			actual = actual->fail;
		// if we find this son (else actual == root)
		if(it != actual->son.end())
			actual = it->second;
		// default value (if none pattern will be found)
		fin[i] = -1;
		// if actual node is pattern then we'll add it to fin
		if(actual->is_pattern)
			fin[i - patterns[actual->pattern_id].first.size() + 1] = actual->pattern_id;
		// find orher patterns
		else if((found = actual->long_sh_pat) != root)
			fin[i - patterns[found->pattern_id].first.size() + 1] = found->pattern_id;
	}
}