#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>
#include <queue>

using namespace std;

#ifdef DEBUG
#include <iostream>

#define D(...) __VA_ARGS__
#define LOG(x) cerr << #x << ": " << x
#define LOGN(x) cerr << #x << ": " << x << endl

#else
#define D(...)
#define LOG(x)
#define LOGN(x)
#endif

// Read only bit file class
class input_bit_file
{
	FILE* file;
	char buffer[1024];
	size_t first_byte, end_byte;
	unsigned char current_bit;
	bool all_good;

	input_bit_file(const  input_bit_file&);
	input_bit_file& operator=(const input_bit_file&);

public:
	explicit input_bit_file(const char* file_name): file(fopen(file_name, "rb")), first_byte(0), end_byte(0), current_bit(0), all_good(file != NULL)
	{
		if(all_good && (end_byte = fread(buffer, sizeof(char), 1024 , file)) == 0)
			all_good = false;
	}

	~input_bit_file()
	{
		if(file != NULL)
			fclose(file);
	}

	void open(const char* file_name)
	{
		if(file != NULL)
			fclose(file);
		file = fopen(file_name, "rb");
		first_byte = end_byte = current_bit = 0;
		all_good = file != NULL;
		if(all_good && (end_byte = fread(buffer, sizeof(char), 1024 , file)) == 0)
			all_good = false;
	}

	void close()
	{
		if(file != NULL)
			fclose(file);
		file = NULL;
	}

	bool good()
	{return all_good;}

	bool get_bit()
	{
		bool result = buffer[first_byte] & (1 << current_bit++);
		// D(cout << result;)
		if(current_bit == 8)
		{
			current_bit = 0;
			++first_byte;
			if(first_byte == end_byte)
			{
				first_byte = 0;
				if((end_byte = fread(buffer, sizeof(char), 1024 , file)) == 0)
					all_good = false;
			}
		}
		return result;
	}

	int getchar()
	{
		int res = 0;
		if(get_bit()) res |= 128;
		if(get_bit()) res |= 64;
		if(get_bit()) res |= 32;
		if(get_bit()) res |= 16;
		if(get_bit()) res |= 8;
		if(get_bit()) res |= 4;
		if(get_bit()) res |= 2;
		if(get_bit()) res |= 1;
		if(all_good)
			return res;
		return -1;
	}

	size_t pos()
	{return ((ftell(file)+first_byte-end_byte) << 3) + current_bit;}
};

// Write only bit file class
class output_bit_file
{
	FILE* file;
	char buffer[1024];
	size_t current_byte;
	unsigned char current_bit;
	bool all_good;

	output_bit_file(const output_bit_file&);
	output_bit_file& operator=(const output_bit_file&);

public:
	explicit output_bit_file(const char* file_name): file(fopen(file_name, "wb")), current_byte(0), current_bit(0), all_good(file != NULL){}

	~output_bit_file()
	{
		if(all_good && (current_byte > 0 || current_bit > 0))
			fwrite(buffer, sizeof(char), current_byte+(current_bit == 0 ? 0:1), file);
		if(file != NULL)
			fclose(file);
	}

	void open(const char* file_name)
	{
		if(file != NULL)
		{
			if(all_good && (current_byte > 0 || current_bit > 0))
				fwrite(buffer, sizeof(char), current_byte+(current_bit == 0 ? 0:1), file);
			fclose(file);
		}
		file = fopen(file_name, "wb");
		current_byte = current_bit = 0;
		all_good = file != NULL;
	}

	void close()
	{
		if(all_good && (current_byte > 0 || current_bit > 0))
		{
				fwrite(buffer, sizeof(char), current_byte+(current_bit == 0 ? 0:1), file);
				current_byte = current_bit = 0;
		}
		if(file != NULL)
			fclose(file);
		file = NULL;
	}

	bool good()
	{return all_good;}

	void put_bit(bool bit)
	{
		// I have to finish this function
		if(bit)
			buffer[current_byte] |= 1 << current_bit++;
		else
			buffer[current_byte] &= ~(1 << current_bit++);
		if(current_bit == 8)
		{
			current_bit = 0;
			++current_byte;
			if(current_byte == 1024)
			{
				current_byte = 0;
				if(1024 != fwrite(buffer, sizeof(char), 1024, file))
					all_good = false;
			}
		}
	}

	void put_bits(const vector<bool>& vb)
	{
		for(unsigned i = 0; all_good && i < vb.size(); ++i)
			put_bit(vb[i]);
	}

	size_t pos()
	{return ((ftell(file)+current_byte) << 3) + current_bit;}
};

class bool_iter
{
	char *point, bit;
public:
	typedef bool value_type;
	typedef bool* pointer;
	typedef bool& reference;
	typedef size_t difference_type;
	typedef std::input_iterator_tag iterator_category;

	bool_iter(void* p, char b=7): point(reinterpret_cast<char*>(p)), bit(b){}

	bool_iter(const bool_iter& bi): point(bi.point), bit(bi.bit){}

	bool_iter& operator=(const bool_iter& bi)
	{
		point = bi.point;
		bit = bi.bit;
		return *this;
	}

	bool operator*() const
	{return *point & (1<<bit);}

	bool_iter& operator++()
	{
		if(bit == 0)
		{
			bit = 8;
			++point;
		}
		--bit;
		return *this;
	}

	bool_iter operator++(int)
	{
		bool_iter bi = *this;
		if(bit == 0)
		{
			bit = 8;
			++point;
		}
		--bit;
		return bi;
	}

	bool operator==(const bool_iter& bi) const
	{return point == bi.point && bit == bi.bit;}

	bool operator!=(const bool_iter& bi) const
	{return point != bi.point || bit != bi.bit;}
};

class tree_class
{
	tree_class(const tree_class&);
	tree_class& operator=(const tree_class&);
public:
	struct node
	{
		typedef node* ptr;
		ptr left, right;
		int number, value;
		node(const ptr& l=NULL, const ptr& r=NULL, int n=0, int v=-1): left(l), right(r), number(n), value(v){}
		bool operator<(const node& _n) const
		{return number < _n.number;}
	};

	node::ptr root;
	vector<bool> image;

	tree_class(node::ptr r=NULL): root(r), image(){}

	void build(input_bit_file& file, int arr[], node::ptr x)
	{
		static int limit = 511, idx = 0;
		if(x == root)
			limit = 511, idx = 0;
		if(--limit < 0 || idx > 255)
			return;
		if(file.get_bit())
			D(cout<<1,/*cout<<char(arr[idx]),*/)x->value = arr[idx++];
		else
			D(cout<<0,)
			build(file, arr, x->left = new node),
			build(file, arr, x->right = new node);
	}

	tree_class(input_bit_file& file): root(NULL), image()
	{
		int arr[256]={-1};
		int c;
		for(int i = 0; i < 256; ++i)
		{
			if(c = file.getchar(), c == -1)
				return;
			else arr[c] = i;
			// LOG(file.pos())D(<< " -> " << c << endl;)
		}
		if(arr[0] != -1)
		{
			root = new node;
			build(file, arr, root);
		}
		D(cout << endl;)
	}

	~tree_class()
	{destruct(root);}

	void destruct(node::ptr x)
	{
		if(x->left != NULL)
			destruct(x->left);
		if(x->right != NULL)
			destruct(x->right);
		delete x;
	}

	void clear()
	{
		destruct(root);
		root = NULL;
	}

	void DFS(vector<bool> prefix[])
	{
		class DFS_class
		{
			DFS_class(const DFS_class&);
			DFS_class& operator=(const DFS_class&);
		public:
			unsigned char arr[256], idx;
			vector<bool> code, *pref, &img;

			DFS_class(vector<bool> *v, vector<bool>& i): idx(0), code(), pref(v), img(i)
			{memset(arr, 255, 256);}

			void run(node::ptr x)
			{
				if(x->value != -1)
				{
					img.push_back(true);
					arr[x->value] = idx++;
					pref[x->value] = code;
				}
				else
				{
					img.push_back(false);
					code.push_back(false);
					if(x->left != NULL)
						run(x->left);
					code.back() = true;
					if(x->right != NULL)
						run(x->right);
					code.pop_back();
				}
			}
		} dfs(prefix, image);
		if(root != NULL)
		{
			if(root->left == NULL && root->right == NULL)
			{
				node::ptr k = new node(NULL, NULL, 0, 0), r = new node(root, k);
				root = r;
			}
			dfs.run(root);
		}
		D(for(unsigned i=0; i<image.size(); ++i)
			cout << image[i];
		cout << endl;)
		D(cout << "Partial tree image size: " << image.size() << " bits" << endl;)
		image.insert(image.begin(), bool_iter(dfs.arr), bool_iter(dfs.arr+256));
		/*D(for(unsigned i=0; i<image.size(); ++i)
		{
			if(i%8 == 0)
				cout << endl<< i/8 << ": ";
			cout << image[i];
		}
		cout << endl;)*/
		D(cout << "Total tree image size: " << image.size() << " bits" << endl;)
	}
};

struct comp
{
	bool operator()(const tree_class::node::ptr& a, const tree_class::node::ptr& b) const
	{return *a < *b;}
};

void compress(const char* in, const char* out, bool test_only=false)
{
	string $ = in;
	if(strcmp(out, "") == 0)
	{
		$ += ".huff";
		out = $.c_str();
	}
	unsigned long long count[256]={}, file_size, counter = 100;
	FILE* fin = fopen(in, "rb");
	if(fin == NULL)
	{
		printf("Error opening file: %s", in);
		return;
	}
	fseek(fin, 0, SEEK_END);
	file_size = ftell(fin);
	rewind(fin);
	printf("File size: %llu\n", file_size);
	for(int c = getc(fin), percent = -1; c != EOF; counter+=100, c = getc(fin))
	{
		++count[static_cast<unsigned>(c)];
		int tmp = counter / file_size;
		if(tmp != percent)
		{
			printf("\rCounting bytes...  %i%%", percent = tmp);
			fflush(stdout);
		}
	}
	printf("\n");
	rewind(fin);
	// Make huffman tree
	multiset<tree_class::node::ptr, comp> heap;
	for(int i=0; i<256; ++i)
		if(count[i] > 0)
			heap.insert(new tree_class::node(NULL, NULL, count[i], i));
	while(heap.size() > 1)
	{
		tree_class::node::ptr x = new tree_class::node(*heap.begin(), *++heap.begin(), (*heap.begin())->number+(*++heap.begin())->number);
		heap.erase(heap.begin());
		heap.erase(heap.begin());
		heap.insert(x);
	}
	tree_class tree;
	if(heap.size())
		tree.root = *heap.begin();
	heap.clear();
	// Make prefix array
	vector<bool> prefix[256]={};
	tree.DFS(prefix);
	// Calculate trailing unset bits & compressed size
	unsigned long long compressed_size = 3 + tree.image.size();
	//                                   ^ this for unset_bits
	for(int i = 0; i < 256; ++i)
		compressed_size += prefix[i].size() * count[i];
	char unset_bits = compressed_size & 7; // % 8
	compressed_size >>= 3;
	if(unset_bits)
	{
		++compressed_size;
		unset_bits = 8 - unset_bits;
	}
	D(printf("Trailing unset bits: %i\n", unset_bits);)
	printf("Compressed size: %llu\nCompression ratio: %llu%%\n", compressed_size, 100 * compressed_size / file_size);
	if(test_only)
	{
		fclose(fin);
		return;
	}
	output_bit_file fout(out);
	// Write tree.image to file
	fout.put_bits(tree.image);
	// Write unset_bits to file
	fout.put_bit(unset_bits & 4);
	fout.put_bit(unset_bits & 2);
	fout.put_bit(unset_bits & 1);
	// Compression
	counter = 100;
	for(int c = getc(fin), percent = -1; c != EOF; counter+=100, c = getc(fin))
	{
		fout.put_bits(prefix[static_cast<unsigned>(c)]);
		int tmp = counter / file_size;
		if(tmp != percent)
		{
			printf("\rCompressing...  %i%%", percent = tmp);
			fflush(stdout);
		}
	}
	if(counter != file_size*100+100)
		printf("\nCompression error");
	printf("\n");
	fclose(fin);
}

void decompress(const char* in, const char* out)
{
	string $ = in;
	if(strcmp(out, "") == 0)
	{
		$ += ".1";
		out = $.c_str();
	}
	unsigned long long file_size, counter, bit_file_size, bit_i = 0;
	input_bit_file fin(in);
	if(!fin.good())
	{
		printf("Error opening file: %s\n", in);
		return;
	}
	// Get in file size
	FILE* fout = fopen(in, "rb");
	fseek(fout, 0, SEEK_END);
	file_size = ftell(fout);
	// Open out file
	freopen(out, "wb", fout);
	bit_file_size = file_size << 3;
	printf("File size: %llu\n", file_size);
	tree_class tree(fin);
	LOGN(fin.pos());
	// Unset bits
	if(fin.get_bit()) bit_file_size -= 4;
	if(fin.get_bit()) bit_file_size -= 2;
	if(fin.get_bit()) --bit_file_size;
	bit_i = fin.pos();
	LOGN(bit_i);
	LOGN(bit_file_size);
	counter = bit_i * 100 + 100;
	tree_class::node::ptr current_node = tree.root;
	for(int percent = -1; bit_i < bit_file_size; counter += 100, ++bit_i)
	{
		// Do decompression here
		if(fin.get_bit())
			current_node = current_node->right;
		else
			current_node = current_node->left;
		if(current_node->value != -1)
			putc(current_node->value, fout), current_node = tree.root;
		int tmp = counter / bit_file_size;
		if(tmp != percent)
		{
			printf("\rDecompressing...  %i%%", percent = tmp);
			fflush(stdout);
		}
	}
	printf("\n");
	fclose(fout);
}

int main(int argc, char const **argv)
{
	if(argc < 3)
	{
		cout << "Usage <option> <file> [out_file]\nOptions:\n    a - add file to compress\n    t - test compression ratio without compressing\n    x - unpack file" << endl;
		return 1;
	}
	if(strcmp(argv[1], "a") == 0)
		compress(argv[2], (argc < 4 ? "":argv[3]));
	else if(strcmp(argv[1], "t") == 0)
		compress(argv[2], (argc < 4 ? "":argv[3]), true);
	else if(strcmp(argv[1], "x") == 0)
		decompress(argv[2], (argc < 4 ? "":argv[3]));
	return 0;
}