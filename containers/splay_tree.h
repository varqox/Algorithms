#pragma once

struct less {
	template<class T>
	bool operator ()(const T& a, const T& b) const { return a < b; }
};

// Allow duplicate keys
template<class T, class Comp = less>
class SplayTree {
public:
	static const int L = 0, R = 1;
	struct BaseNode {
		BaseNode *p, *kid[2];
	};

	struct Node {
		Node *p, *kid[2];
		T val;
	};

	Node *nil, *root;
	Comp cmp;

	SplayTree() : nil((Node*)new BaseNode), root(nil), cmp() {
		nil->p = nil->kid[L] = nil->kid[R] = nil;
	}

	~SplayTree() {
		release(root);
		delete nil;
	}

	void release(Node *x) {
		if (x != nil) {
			release(x->kid[L]);
			release(x->kid[R]);
			delete x;
		}
	}

	Node* new_node(const T& v, Node *parent = 0) {
		Node *n = new Node;
		*n = (Node){ parent == 0 ? nil : parent, {nil, nil}, v };
		return n;
	}

	void rotate(Node *x, bool dir) {
		Node *vv = x->kid[!dir];
		x->kid[!dir] = vv->kid[dir];
		x->kid[!dir]->p = x;

		vv->p = x->p;
		vv->p->kid[vv->p->kid[R] == x] = vv;

		vv->kid[dir] = x;
		x->p = vv;
	}

	void splay(Node *x) {
		if (x == nil)
			return;
		while (x->p != nil) {
			if (x->p->p != nil &&
				(x->p->kid[L] == x) == (x->p->p->kid[L] == x->p))
			{
				rotate(x->p->p, x->p->p->kid[L] == x->p);
				rotate(x->p, x->p->kid[L] == x);
			} else
				rotate(x->p, x->p->kid[L] == x);
		}
		root = x;
	}

	Node* dirmost(Node *x, bool dir) {
		return x == nil || x->kid[dir] == nil ? x : dirmost(x->kid[dir], dir);
	}

	Node* find(const T& v) {
		Node *x = root, *y = nil;
		while (x != nil && x->val != v)
			y = x, x = x->kid[cmp(x->val, v)];
		splay(x == nil ? y : x);
		return x;
	}

	Node *insert(const T& v) {
		if (root == nil)
			return root = new_node(v);
		find(v);
		Node *x = new_node(v, root);
		bool dir = cmp(root->val, v);
			x->kid[dir] = root->kid[dir];
			x->kid[dir]->p = x;

			root->kid[dir] = x;
		splay(x);
		return root;
	}

	void erase(const T& v) {
		if (find(v) != nil)
			erase(root);
	}

	void erase(Node* x) {
		splay(x); // x == root
		Node *vv = root->kid[R];
		if (root->kid[L] == nil) {
			root = vv;
			root->p = nil;
		} else {
			root = root->kid[L];
			root->p = nil;
			splay(dirmost(root, R));
			root->kid[R] = vv;
			vv->p = root;
		}
		delete x;
	}
};
