#pragma once
//it is very hard to implement iterator for tree
//so to iterate through it we will use my vector
#include "vector.h"

template<class Key, class T>
class AVLTree
{
private:
	class Node
	{
	public:
		Node* left;
		Node* right;
		T content;
		Key key;
		//int balanceFactor;

		Node() :left(nullptr), right(nullptr) {}
		Node(Node* _left, Node* _right) : left(_left), right(_right) {}
		Node(const Key keyVal, const T& val) :content(val), key(keyVal), left(nullptr), right(nullptr) {}
		~Node()
		{
			delete left;
			delete right;
		}

		Node& operator=(const Node& other)
		{
			if (this == &other)
				return *this;

			this->right = other.right;
			this->left = other.left;
			this->content = other.content;

			return *this;
		}
	};

	Node* root = nullptr;


	//avl tree rotations
	Node* rightRightRotation(Node* parent)
	{
		Node* n = parent->right;
		parent->right = n->left;
		n->left = parent;

		return n;
	}

	Node* leftLeftRotation(Node* parent)
	{
		Node* n = parent->left;
		parent->left = n->right;
		n->right = parent;

		return n;
	}

	Node* leftRightRotation(Node* parent)
	{
		Node* n = parent->left;
		parent->left = rightRightRotation(n);

		return leftLeftRotation(parent);
	}

	Node* rightLeftRotation(Node* parent)
	{
		Node* n = parent->right;
		parent->right = leftLeftRotation(n);

		return rightRightRotation(parent);
	}

public:

	int calculateHeight(Node* rootNode)
	{
		int height = 0;
		if (rootNode != nullptr)
		{
			int left_height = calculateHeight(rootNode->left);
			int right_height = calculateHeight(rootNode->right);
			height = std::max(left_height, right_height) + 1;
		}

		return height;
	}

	int calculateBalanceFactor(Node* rootNode)
	{
		int left_height = calculateHeight(rootNode->left);
		int right_height = calculateHeight(rootNode->right);

		return left_height - right_height;
	}

	int getTreeHeight()
	{
		return calculateTreeHeight();
	}
private:

	int calculateTreeHeight()
	{
		return calculateHeight(root);
	}

	Node* balance(Node* parent)
	{
		if (parent == nullptr)
			return nullptr;

		int balanceFactor = calculateBalanceFactor(parent);

		//perform rotations
		//node is right heavy

		if (balanceFactor > 1)
		{
			if (calculateBalanceFactor(parent->left) > 0)
				parent = leftLeftRotation(parent);
			else
				parent = leftRightRotation(parent);
		}
		//node is left heavy
		else if (balanceFactor < -1)
		{
			if (calculateBalanceFactor(parent->right) > 0)
				parent = rightLeftRotation(parent);
			else
				parent = rightRightRotation(parent);
		}

		return parent;
	}

	Node* insert(Node* rootNode, const std::pair<Key, T>& content)
	{
		Key key = content.first;
		if (rootNode == nullptr)
		{
			rootNode = new Node(content.first, content.second);
			return rootNode;
		}

		//if duplicate key is inserted, return existing key
		//this is the behaviour of original map
		if (key == rootNode->key)
			return rootNode;

		else if (key < rootNode->key)
			rootNode->left = insert(rootNode->left, content);
		else if (key >= rootNode->key)
			rootNode->right = insert(rootNode->right, content);

		rootNode = balance(rootNode);
		return rootNode;
	}


	//cannot use just delete, cause it is keyword
	Node* deleteNode(Node* rootNode, const Key& key)
	{
		if (rootNode == nullptr)
			return nullptr;

		while (rootNode != nullptr)
		{
			//root node has been found
			if (rootNode->key == key)
					break;
			if (rootNode->key < key)
				rootNode->right = deleteNode(rootNode->right, key);
			else if (rootNode->key > key)
				rootNode->left = deleteNode(rootNode->left, key);
		}

		//key has been found
		if (rootNode->left == nullptr && rootNode->right == nullptr)
			rootNode = nullptr;
		else if (rootNode->left == nullptr)
		{
			rootNode->key = findLeft(rootNode->right)->key;
			rootNode->right = deleteNode(rootNode->right, rootNode->key);
		}
		else
		{
			rootNode->key = findRight(rootNode->left)->key;
			rootNode->left = deleteNode(rootNode->left, rootNode->key);
		}

		return balance(rootNode);
	}

	//finds last left node of given root
	Node* findLeft(Node* rootNode)
	{
		if (rootNode == nullptr)
			return rootNode;

		while (rootNode->left != nullptr)
			rootNode = rootNode->left;
		return rootNode;
	}

	//finds last right node of given root
	Node* findRight(Node* rootNode)
	{
		if (rootNode == nullptr)
			return rootNode;

		while (rootNode->right != nullptr)
			rootNode = rootNode->right;
		return rootNode;
	}

	Node* find(Node* rootNode, const Key& key)
	{

		while (rootNode != nullptr)
		{
			if (rootNode->key == key)
				return rootNode;
			else if (key < rootNode->key)
				rootNode = rootNode->left;
			else if (key > rootNode->key)
				rootNode = rootNode->right;
		}
		return nullptr;
	}

	/*
	void printContent(Node* rootNode)
	{
		if (rootNode != nullptr)
		{
			cout << rootNode->content << "(" << calculateBalanceFactor(rootNode) << ")" << "\n";
		}
		else
			return;
		printContent(rootNode->left);
		printContent(rootNode->right);
	}
	*/

	//this function uses recursion, our vector is not static safe, so we pass reference to it
	void getContent(Node* rootNode, myVector<T>& copy)
	{

		if (rootNode != nullptr)
		{
			getContent(rootNode->left, copy);
			getContent(rootNode->right, copy);
			copy.push_back(rootNode->content);
		}
	}
public:

	//we do insert like in normal BST, but we do balance at the end
	void insert(const std::pair<Key, T>& val)
	{
		root = insert(root, val);
	}

	void deleteNode(const Key& key)
	{
		root = deleteNode(root, key);
	}

	Node* find(const Key& key)
	{
		return find(root, key);
	}

	T& operator[](const Key& key)
	{
		Node* n = find(root, key);
		if (n == nullptr)
			root = insert(root, { key, T() });
		else
			return n->content;
		
		//this will execute in case of inserting a new node
		return find(root, key)->content;
	}
	
	/*
	void printContent()
	{
		printContent(root);
	}
	*/
	

	//instead of implementing iterator
	myVector<T> getContent()
	{
		myVector<T> copyVec;
		getContent(root, copyVec);
		return copyVec;
	}

	void clear()
	{
		delete root;
		root = nullptr;
	}

	~AVLTree()
	{
		delete root;
	}
};
