#include <string>

#pragma once

class Trie
{
private:
	struct node;
	node* root;
public:
	Trie();
	~Trie();

	bool search(const std::string&) const;
	void insert(const std::string&);
	void erase(const std::string&);
};