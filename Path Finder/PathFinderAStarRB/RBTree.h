#pragma once
using namespace std;

template <class Key, class T>
class RBTree
{
private:
	enum Color { Black, Red };
	class Node
	{
		friend class RBTree;
	public:
		Node(const Key& _key, const T& _content) :key(_key), content(_content) {}
		Node() {}
		Key key;
		T content;
	private:
		Color color = Red;
		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
	};

private:
	Node* root;
	//custom null node prevents reading from nullptr pointer, thus preventing from errors
	Node* TNULL;

	Node* search(Node* node, const Key& key) {
		while (node != TNULL)
		{
			if (node->key == key)
				return node;
			if (node->key > key)
				node = node->left;
			else
				node = node->right;
		}

		//TNULL case
		return node;
	}

	// fix the rb tree modified by the delete operation
	//cases means balance cases, can be found on the net
	void fixDelete(Node* x) {
		Node* s;
		while (x != root && x->color == Black) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == Red) {
					// case 3.1
					s->color = Black;
					x->parent->color = Red;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == Black && s->right->color == Black) {
					// case 3.2
					s->color = Red;
					x = x->parent;
				}
				else {
					if (s->right->color == Black) {
						// case 3.3
						s->left->color = Black;
						s->color = Red;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = Black;
					s->right->color = Black;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == Red) {
					// case 3.1
					s->color = Black;
					x->parent->color = Red;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == Black && s->right->color == Black) {
					// case 3.2
					s->color = Red;
					x = x->parent;
				}
				else {
					if (s->left->color == Black) {
						// case 3.3
						s->right->color = Black;
						s->color = Red;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = Black;
					s->left->color = Black;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = Black;
	}


	void rbTransplant(Node* u, Node* v) {
		if (u->parent == TNULL) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(Node* node, const Key& key)
	{
		// find the node containing key
		//z - node with key, x,y helpers in transplant
		Node* z = TNULL;
		Node* x;
		Node* y;
		while (node != TNULL)
		{
			if (node->key == key)
				z = node;

			if (node->key <= key)
				node = node->right;

			else
				node = node->left;
		}

		//node has not been found
		if (z == TNULL)
			return;

		y = z;
		Color y_original_color = y->color;
		if (z->left == TNULL)
		{
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL)
		{
			x = z->left;
			rbTransplant(z, z->left);
		}
		else
		{
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == Black)
		{
			fixDelete(x);
		}
	}

	//for recursive calls to delete whole tree
	void deleteNodeHelper(Node* root)
	{
		if (root == TNULL)
			return;

		deleteNodeHelper(root->right);
		deleteNodeHelper(root->left);

		delete root;
	}

	// fix the red-black tree with rotations
	void fixInsert(Node* k)
	{
		Node* u;
		while (k->parent->color == Red)
		{
			if (k->parent == k->parent->parent->right)
			{
				u = k->parent->parent->left;
				if (u->color == Red)
				{
					// case 3.1
					u->color = Black;
					k->parent->color = Black;
					k->parent->parent->color = Red;
					k = k->parent->parent;
				}
				else
				{
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = Black;
					k->parent->parent->color = Red;
					leftRotate(k->parent->parent);
				}
			}
			else
			{
				u = k->parent->parent->right; // uncle

				if (u->color == Red) {
					// mirror case 3.1
					u->color = Black;
					k->parent->color = Black;
					k->parent->parent->color = Red;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = Black;
					k->parent->parent->color = Red;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = Black;
	}

	// rotate left at node x
	void leftRotate(Node* x)
	{
		Node* y = x->right;
		x->right = y->left;
		if (y->left != TNULL)
			y->left->parent = x;

		y->parent = x->parent;
		if (x->parent == TNULL)
			this->root = y;

		else if (x == x->parent->left)
			x->parent->left = y;

		else
			x->parent->right = y;

		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(Node* x)
	{
		Node* y = x->left;
		x->left = y->right;
		if (y->right != TNULL)
			y->right->parent = x;

		y->parent = x->parent;
		if (x->parent == TNULL)
			this->root = y;

		else if (x == x->parent->right)
			x->parent->right = y;

		else
			x->parent->left = y;

		y->right = x;
		x->parent = y;
	}

public:
	RBTree() {
		TNULL = new Node();
		TNULL->color = Black;
		TNULL->left = TNULL;
		TNULL->right = TNULL;
		root = TNULL;
	}

	// find the node with the minimum key
	Node* minimum(Node* node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	Node* maximum(Node* node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}


	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(const pair<Key, T>& content)
	{
		// Ordinary Binary Search Insertion
		Node* node = new Node(content.first, content.second);
		node->parent = TNULL;
		node->left = TNULL;
		node->right = TNULL;
		node->color = Red; // new node must be red

		Node* y = TNULL;
		Node* x = this->root;

		while (x != TNULL)
		{
			y = x;
			//key exists
			if (node->key == x->key)
				return;
			if (node->key < x->key)
				x = x->left;
			else
				x = x->right;

		}

		// y is parent of x
		node->parent = y;
		if (y == TNULL)
			root = node;

		else if (node->key < y->key)
			y->left = node;
		else
			y->right = node;


		// if new node is a root node, simply return
		if (node->parent == TNULL) {
			node->color = Black;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == TNULL) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	T& operator[](const Key& key)
	{
		Node* n = search(root, key);
		if (n != TNULL)
			return n->content;
		else
			insert({ key, T() });

		return search(root, key)->content;
	}

	Node* find(const Key& key)
	{
		Node* n = search(root, key);
		if (n == TNULL)
			return nullptr;

		return n;
	}

	// delete the node from the tree
	void deleteNode(const Key& key) {
		deleteNodeHelper(this->root, key);
	}

	~RBTree()
	{
		deleteNodeHelper(this->root);
	}

};