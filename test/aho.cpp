#include <iostream>
#include <vector>
#include <queue>
#include "../text/aho.cpp"

using namespace std;

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
