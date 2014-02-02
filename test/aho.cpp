#include <iostream>
#include <vector>
#include <queue>

using namespace std;

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

int aho::class_trie::add_word(const string& word, int id)
{
	int ver=0; // actual node (vertex)
	for(int s=word.size(), i=0; i<s; ++i)
	{
		if(graph[ver].E[static_cast<unsigned char>(word[i])]!=0) ver=graph[ver].E[static_cast<unsigned char>(word[i])]; // actual view node = next node
		else
		{
			ver=graph[ver].E[static_cast<unsigned char>(word[i])]=graph.size(); // add id of new node
			graph.push_back(node(word[i])); // add new node
		}
	}
	if(!graph[ver].is_pattern)
	{
		graph[ver].is_pattern=true;
		graph[ver].pattern_id=id;
	}
return graph[ver].pattern_id;
}

void aho::class_trie::add_fails() // and the longest shorter patterns, based on BFS algorithm
{
	queue<int> V;
	// add root childrens
	for(int i=0; i<256; ++i)
	{
		if(graph.front().E[i]!=0) // if children exists
		{
			graph[graph.front().E[i]].fail=graph[graph.front().E[i]].long_sh_pat=0;
			V.push(graph.front().E[i]);
		}
	}
	while(!V.empty())
	{
		int actual=V.front(); // id of actual view node
		for(int i=0; i<256; ++i) // i is character of view node
		{
			if(graph[actual].E[i]!=0) // if children exists
			{
				actual=graph[actual].fail; // we have view node parent's fial edge
				while(actual>0 && graph[actual].E[i]==0) // while we don't have node with children of actual character (i)
					actual=graph[actual].fail;
				actual=graph[graph[V.front()].E[i]].fail=graph[actual].E[i]; // the longest sufix, if 0 then longest sufix = root
				// add the longest shorter pattern
				if(graph[actual].is_pattern) // if the fail node is pattern then is long_sh_pat
					graph[graph[V.front()].E[i]].long_sh_pat=actual;
				else // long_sh_pat is the fail node's long_sh_pat
					graph[graph[V.front()].E[i]].long_sh_pat=graph[actual].long_sh_pat;
				actual=V.front();
				V.push(graph[actual].E[i]); // add this children to queue
			}
		}
		V.pop(); // remove visited node
	}
}

void aho::find(const vector<string>& patterns, const string& text)
{
	vector<class_trie::node>().swap(trie.graph); // clear trie.graph
	vector<vector<unsigned>* >().swap(fin); // clear fin
	fin.resize(patterns.size()); // set number of patterns
	// trie::init(); 
	class_trie().swap(trie); // initialize trie
	unsigned tmp;
	for(int i=patterns.size()-1; i>=0; --i) // add patterns to trie
	{
		tmp=trie.add_word(patterns[i], i);
		if(tmp==static_cast<unsigned>(i)) fin[i]=new vector<unsigned>;
		else fin[i]=fin[tmp];
	}
	trie.add_fails(); // add fails edges
	int act=0, pat; // actual node - root
	for(int s=text.size(), i=0; i<s; ++i)
	{
		while(act>0 && trie.graph[act].E[static_cast<unsigned char>(text[i])]==0)
			act=trie.graph[act].fail; // while we can't add text[i] to path, go to fail node
		if(trie.graph[act].E[static_cast<unsigned char>(text[i])]!=0) // if we can add text[i] to path
			act=trie.graph[act].E[static_cast<unsigned char>(text[i])];
		if(trie.graph[act].is_pattern) // if actual node is pattern, then add it to fin
			fin[trie.graph[act].pattern_id]->push_back(i);
		pat=trie.graph[act].long_sh_pat; // go to the pattern node
		while(pat>0) // finding patterns
		{
			fin[trie.graph[pat].pattern_id]->push_back(i); // add pat node to fin
			pat=trie.graph[pat].long_sh_pat; // go to the next pattern
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	string text;
	cin >> text;
	int n;
	cin >> n;
	aho _aho;
	vector<string> pats(n);
	for(int i=0; i<n; ++i)
		cin >> pats[i];
	_aho.find(pats, text);
	for(int i=0; i<n; ++i)
	{
		cout << pats[i] << " :";
		for(unsigned j=0; j<_aho[i].size(); ++j)
			cout << ' ' << _aho[i][j];
		cout << endl;
	}
return 0;
}
