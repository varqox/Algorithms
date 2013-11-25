#include <iostream>
#include <deque>

using namespace std;

deque<int> kmp(const string& text, const string& pattern)
{
	deque<int> out;
	int *P=new int[pattern.size()], k=0, pl=pattern.size();
	P[0]=0;
	for(int i=1; i<pl; ++i)
	{
		while(k>0 && pattern[k]!=pattern[i])
			k=P[k-1];
		if(pattern[k]==pattern[i]) ++k;
		P[i]=k;
	}
	k=0;
	for(int tl=text.size(), i=0; i<tl; ++i)
	{
		while(k>0 && pattern[k]!=text[i])
			k=P[k-1];
		if(pattern[k]==text[i]) ++k;
		if(k==pl)
		{
			out.push_back(i);
			k=P[k-1];
		}
	}
	delete[] P;
return out;
}

int main()
{
	ios_base::sync_with_stdio(0);
	string pattern, text;
	deque<int> wys;
	cin >> text;
	cin >> pattern;
	cout << pattern << " :";
	wys=kmp(text, pattern);
	while(!wys.empty())
	{
		cout << ' ' << wys.front();
		wys.pop_front();
	}
	cout << endl;
return 0;
}