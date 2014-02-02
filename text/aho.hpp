#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

class const_string
{
	char* _M_str;
public:
	explicit const_string(const char* _str): _M_str(new char[strlen(_str)+1])
	{
		memcpy(_M_str, _str, strlen(_str)+1);
	}
	const_string(const const_string& _cstr): _M_str(_cstr._M_str) {}
	const_string& operator=(const const_string& _cstr)
	{
		_M_str=_cstr._M_str;
	return *this;
	}
	~const_string(){}

	const char* str() const
	{return _M_str;}

	bool operator==(const const_string& _cstr)
	{return this==&_cstr;}

	bool operator!=(const const_string& _cstr)
	{return this!=&_cstr;}

	template<class ostream_type>
	friend ostream_type& operator<<(ostream_type& os, const const_string& _cstr)
	{return os << _cstr._M_str;}

	operator const char*() const
	{return _M_str;}
};

// aho.cpp
class aho
{
	class class_trie
	{
	public:
		struct node
		{
			int E[256], fail, long_sh_pat, pattern_id; // fail pointer, max shorter pattern, pattern id
			bool is_pattern; // is pattern end in this vertex
			unsigned char character; // this node character
			node(unsigned char letter=0): fail(), long_sh_pat(), pattern_id(), is_pattern(false), character(letter)
			{
				for(int i=0; i<256; ++i)
					E[i]=0;
			}
			~node(){}
		};

		vector<node> graph;

		class_trie(): graph(1) // add root
		{
			graph.front().fail=graph.front().long_sh_pat=0; // max shorter pattern isn't exist
		}

		void swap(class_trie& _t)
		{
			graph.swap(_t.graph);
		}

		int add_word(const string& word, int id);
		void add_fails(); // and the longest shorter patterns, based on BFS algorithm
	} trie;

	vector<vector<unsigned>* > fin; // finding patterns

public:
	aho(): trie(), fin(){}
	~aho(){}

	vector<vector<unsigned>* >::size_type size()
	{return fin.size();}

	vector<unsigned>& operator[](vector<vector<unsigned>* >::size_type n)
	{return *fin[n];}

	const vector<unsigned>& operator[](vector<vector<unsigned>* >::size_type n) const
	{return *fin[n];}

	void swap(aho& _a)
	{
		trie.swap(_a.trie);
		fin.swap(_a.fin);
	}

	void find(const vector<string>& patterns, const string& text);
};

class special_aho
{
	class class_trie
	{
	public:
		struct node
		{
			int E[256], fail, long_sh_pat, pattern_id; // fail pointer, max shorter pattern, pattern id
			bool is_pattern; // is pattern end in this vertex
			// unsigned char color; // highlight color
			unsigned char character; // this node character
			node(unsigned char letter=0): fail(), long_sh_pat(), pattern_id(), is_pattern(false), character(letter)
			{
				for(int i=0; i<256; ++i)
					E[i]=0;
			}
			~node(){}
		};

		vector<node> graph;

		class_trie(): graph(1) // add root
		{
			graph.front().fail=graph.front().long_sh_pat=0; // max shorter pattern isn't exist
		}

		void swap(class_trie& _t)
		{
			graph.swap(_t.graph);
		}

		int add_word(const string& word, int id);
		void add_fails(); // and the longest shorter patterns, based on BFS algorithm
	} trie;

	vector<pair<string, const_string> > patterns;
	vector<int> fin; // finding patterns

public:
	special_aho(): trie(), patterns(), fin(){}
	~special_aho(){}

	vector<int>::size_type size()
	{return fin.size();}

	int& operator[](vector<int>::size_type n)
	{return fin[n];}

	const int& operator[](vector<int>::size_type n) const
	{return fin[n];}

	void swap(special_aho& _a)
	{
		trie.swap(_a.trie);
		fin.swap(_a.fin);
	}

	const pair<string, const_string>& pattern(vector<pair<string, const_string> >::size_type n) const
	{return patterns[n];}

	void set_patterns(const vector<pair<string, const_string> >& new_patterns);

	void find(const string& text);
};