#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

inline unsigned long int CeilToPowerOf2(unsigned long int x)
{return (x&x-1 ? 1ul<<(sizeof(unsigned long)*8-__builtin_clzl(x)):x);}


template<typename type>
class mvector
{
public:
	typedef unsigned long int size_type;
	typedef type value_type;
	typedef type& reference;
	typedef const type& const_reference;
private:
	size_type arr_size, _size;
	type* arr;
	template<typename T>
	static void swap(T& a, T& b)
	{
		T tmp=a;
		a=b;
		b=tmp;
	}
public:
	explicit mvector(size_type = 0, const type& = type());
	mvector(const mvector&);

	~mvector()
	{if(arr_size) delete[] arr;}

	void swap(mvector& _v)
	{
		swap(this->arr, _v.arr);
		swap(this->_size, _v._size);
		swap(this->arr_size, _v.arr_size);
	}

	size_type size()
	{return this->_size;}

	void resize(size_type, const type& = type());
	void push_back(const type& val)
	{
		this->resize(++this->_size, val);
	}

	void pop_back()
	{--this->_size;}

	type& front()
	{return *this->arr;}

	type& back()
	{return this->arr[this->_size-1];}

	void clear()
	{mvector<type>().swap(*this);}

	type& operator[](size_type _i)
	{return arr[_i];}
};

template<typename type>
mvector<type>::mvector(size_type n, const type& val)
:_size(n),
 arr_size(CeilToPowerOf2(n)),
 arr(new type[this->arr_size])
{
	for(size_type i=0; i<this->_size; ++i)
		this->arr[i]=val;
}

template<typename type>
mvector<type>::mvector(const mvector& _v)
:_size(_v._size),
 arr_size(_v.arr_size),
 arr(new type[this->arr_size])
{
	for(size_type i=0; i<this->_size; ++i)
		this->arr[i]=_v.arr[i];
}

template<typename type>
void mvector<type>::resize(size_type n, const type& val)
{
	if(n<=this->arr_size)
	{
		if(n>this->_size)
			for(size_type i=this->_size; i<n; ++i)
				this->arr[i]=val;
	}
	else
	{
		this->arr_size=CeilToPowerOf2(n);
		type* new_arr=new type[this->arr_size];
		for(size_type i=0; i<this->_size; ++i)
			new_arr[i]=this->arr[i];
		for(size_type i=this->_size; i<n; ++i)
			new_arr[i]=val;
		delete[] this->arr;
		this->arr=new_arr;
	}
	this->_size=n;
}

/*template<typename type>
void mvector<type>::push_back(const type& val)
{
	if(this->_size==this->arr_size)
	{
		if(this->arr_size==0) ++this->arr_size;
		type* new_arr=new type[this->arr_size<<=1];
		for(int i=0; i<this->_size; ++i)
			new_arr[i]=this->arr[i];
		delete[] this->arr;
		this->arr=new_arr;
	}
	this->arr[this->_size++]=val;
}*/

struct lol
{
	int a, b, c, d;
	lol(int k=0): a(k), b(k), c(k), d(k){}
	~lol(){}
	operator int(){return this->a;}
};

int main(int argc, char **argv)
{
	mvector<lol> a;
	int k=0;
	for(int i=0; i<10000000; ++i)
	{
		a.push_back(i);
		k+=a.back();
	}
	for(int i=0; i<10000000; ++i)
	{
		a.pop_back();
	}
	// cin.get();
	cout << k << endl;
	/*ios_base::sync_with_stdio(false);
	if(argc<2)
	{
		cout << "Usage: test <file>" << endl;
		return 0;
	}
	string file_name=argv[1], input;
	fstream file(argv[1], ios_base::in);
	if(file.good())
	{
		while(!file.eof())
			input+=file.get();
		file.close();
		cout << input << endl;
	}
	else cout << "Cannot open file!" << endl;*/
return 0;
}