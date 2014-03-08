#include "compressed_aho.hpp"
#include <queue>

using namespace std;

void compressed_aho_base::add_fails()
{
	queue<node::_ptr> V;
	// add root childrens
	for(node::son_type::iterator it = root->son.begin(); it != root->son.end(); ++it)
	{
		it->second->fail = it->second->long_sh_pat = root;
		V.push(it->second);
	}
	node::_ptr actual;
	// until we didn't visit all nodes
	while(V.size())
	{
		// view all children of actual
		for(node::son_type::iterator x, it = V.front()->son.begin(); it != V.front()->son.end(); ++it)
		{
			// fail node of V.front()
			actual = V.front()->fail;
			// until we get node with son about key of it->second or root
			while((x = actual->son.find(it->second->key)) == actual->son.end() && actual != root)
				actual = actual->fail;
			// if x point to existing node we move actual and it->second->fail to x
			if(x != actual->son.end())
				actual = it->second->fail = x->second;
			// else it->fail has to point to root
			else
				it->second->fail = root;
			// add the longes shorter pattern
			if(actual->is_pattern) // if actual (fail node) is end of pattern then it is long_sh_pat of it->second
				it->second->long_sh_pat = actual;
			else // long_sh_pat of it->second is long_sh_pat of actual
				it->second->long_sh_pat = actual->long_sh_pat;
			// add this children to queue
			V.push(it->second);
		}
		V.pop(); // remove visited node
	}
}

unsigned compressed_aho_base::insert(const string& word, unsigned id)
{
	node::_ptr actual = root;
	node::son_type::iterator it;
	for(string::const_iterator i = word.begin(); i != word.end(); ++i)
	{
		if((it = actual->son.find(*i)) == actual->son.end())
			it = actual->son.insert(make_pair(*i, new node(*i)));
		actual = it->second;
	}
	if(!actual->is_pattern)
	{
		actual->is_pattern = true;
		actual->pattern_id = id;
	}
return actual->pattern_id;
}

void compressed_aho::find(const vector<string>& patterns, const string& text)
{
	// clear fin
	for(unsigned i = 0, s = fin.size(); i < s; ++i)
		if(fin_id[i] == i)
			delete fin[i];
	fin.resize(patterns.size());
	fin_id.resize(patterns.size());
	// clear Trie
	delete root;
	root = new node;
	root->fail = root->long_sh_pat = root;
	// Build Trie
	for(unsigned i = 0, s = patterns.size(); i < s; ++i)
	{
		fin_id[i] = insert(patterns[i], i);
		if(fin_id[i] == i)
			fin[i] = new vector<unsigned>;
		else
			fin[i] = fin[fin_id[i]];
	}
	add_fails(); // add fail and long_sh_pat edges
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
		// if actual node is pattern then we'll add it to fin
		if(actual->is_pattern)
			fin[actual->pattern_id]->push_back(i - patterns[actual->pattern_id].size() + 1);
		// find orher patterns
		found = actual->long_sh_pat;
		// while found point to pattern
		while(found != root)
			// add found pattern and go to next pattern
			fin[found->pattern_id]->push_back(i - patterns[found->pattern_id].size() + 1), found = found->long_sh_pat;
	}
}