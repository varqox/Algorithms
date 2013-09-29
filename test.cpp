#include "useful_functions.hpp"
#include <iostream>

using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	while(true)
	{
		int a;
		cin >> a;
		cout << "ceil_to_power2(): " << ceil_to_power2(a) << endl;
		cout << "floor_to_power2(): " << floor_to_power2(a) << endl;
	}
return 0;
}