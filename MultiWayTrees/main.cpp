/*
	Stoica Tudor Mihai
	Group 30423
	Lab 6

	Task: 
	You are required to implement correctly and efficiently linear time transformations between three different representations for a multi-way tree:
	R1: parent representation: for each key you are given the parent key, in a vector.
	R2: multi-way tree representation: for each node you have the key and a vector of children nodes
	R3: binary tree representation: for each node, you have the key, and two pointers: one to the first child node, and one to the brother on the right (i.e. the next brother node)
	Also, you are required to write a pretty print procedure on R3, which performs a preorder traversal on the binary representation and outputs the tree in a friendly manner (see the image on the next page for an example).
	Therefore, you are given as input a multi-way tree in the parent representation (R1). You are required to implement T1, which transforms the tree to a multi-way representation (R2), then T2, which transforms from the multi-way representation to the binary representation (R3). Then, on the binary representation, you are asked to write a pretty print procedure (using a pre-order traversal).
	You should be able to design the necessary data structures by yourselves. You may use intermediate structures (i.e. additional memory).

	T1:

	- The result is stored in MultiWayTree * multiWayArr which has the type 	

		struct MultiWayTree {
		int father;
		vector<int> childrenNodes;
		int key;
		}

		which means that every node has a key, father and an array of children

		void parentToMultiWayTree() {
		for (unsigned int i = 0; i < parentsArr.size(); i++) { // only 1 iteration O(n)
			if (parentsArr[i] >= 0) { // if we do not speak about the root

				multiWayArr[parentsArr[i]].key = parentsArr[i]; // 
				multiWayArr[i].key = i; // set the key to the index
				multiWayArr[parentsArr[i]].addChild(i); // add the child into the array
				multiWayArr[i].setFather(parentsArr[i]); // its parent is parentsArr[i]

			} else if (parentsArr[i] == -1) {  // if we speak about the root
				rootKey = i;
			}
		}
	}

	this transformation runs in O(n), since we do just 1 iteration
	this additional data structure helps us to memorate every detail about a node

	T2:

		void transformMultiWayArrToTree(Node* node) {

		int firstChild = getFirstChild(node->key);

		if (firstChild == NO_FIRST_CHILD) {

			node->left = NULL;
		} else {

			node->left = new Node(firstChild);
			transformMultiWayArrToTree(node->left);
		}

		int brother = getBrother(node->key);

		if (brother == NO_BROTHER) {

			node->right = NULL;
		} else {

			node->right = new Node(brother);
			transformMultiWayArrToTree(node->right);
		}
	}

	this method: 

	runs in O(n) -> more on that later

	starts from the root node
	always gets the first child
	if there is no first child, then the node->left remains set on NULL
	else if there is a first child, node->left becomes that first child and we perform the method recursively on the first child

	always gets the first right brother
	if there is no right brother, then the node->right remains set on NULL 
	else if there is a right brother, node->right becomes that right brother and we perform the method recursively on the right brother

	how to get the firstChild:

	runs in O(1)

	int getFirstChild(int key) {

		if (multiWayArr[key].childrenNodes.size() > 0) { // if we are not a leaf
			// if he has children

			int keyOfFirstChild = multiWayArr[key].childrenNodes.at(0); // get the first child
			return multiWayArr[keyOfFirstChild].key; // return the first child
		} else {
			return NO_FIRST_CHILD; // if there is no first child, return -2
		}
	}

	how to get the right brother:

	runs in O(1) -> O(n) in the worst case

		int getBrother(int key) {

		int fatherKey = multiWayArr[key].father; // get the father

		// iterate children until you find int key; save the brother in int brotherKey

		int brotherKey = NO_BROTHER; // initially we presume that there is no brother
		bool found = false; // utility variable

		for (int child : multiWayArr[fatherKey].childrenNodes) { // iterate through children until we find the int key parameter

			if (found == true) { // in the next iteration after the (found = true;) line, we can get the brotherKey which is the next child after key

				brotherKey = child;
				break;
			} else { // found == false
				if (child == key) { // if we found the int key parameter
					found = true; // set the utility variable to true
				}
			}
		}

		return brotherKey;
	}

	the entire transformMultiWayArrToTree runs in O(n) because we analyse once every node; it doesn't matter that in the worst case scenario the getBrother is O(n), because
	the average complexity will still remain O(n) for T2

	I have used 

		struct Node {
		int key;
		Node* left;
		Node* right;

		Node(int key) {
			this->key = key;
			this->left = this->right = NULL;
		}
	};

	which helps me memorate the tree

*/

#include <iostream>
#include <vector>

using namespace std;

class Transformation {
public:
	static vector<int> getDemoParentsArr() {  // the example from the book
		vector<int> demoArr;
		demoArr.push_back(2 - 1);
		demoArr.push_back(7 - 1);
		demoArr.push_back(5 - 1);
		demoArr.push_back(2 - 1);
		demoArr.push_back(7 - 1);
		demoArr.push_back(7 - 1);
		demoArr.push_back(-1);
		demoArr.push_back(5 - 1);
		demoArr.push_back(2 - 1);

		return demoArr;
	}

private:
	struct MultiWayTree {
		int father;
		vector<int> childrenNodes;
		int key;

		MultiWayTree(int key) { this->key = key; }

		void addChild(int childKey) { this->childrenNodes.push_back(childKey); }

		void setFather(int fatherKey) { this->father = fatherKey; }

		MultiWayTree() {}
	};

	struct Node {
		int key;
		Node* left;
		Node* right;

		Node(int key) {
			this->key = key;
			this->left = this->right = NULL;
		}
	};

	vector<int> parentsArr;
	MultiWayTree* multiWayArr;
	int rootKey;
	int nrKeys;
	const int NO_FIRST_CHILD = -2;
	const int NO_BROTHER = -2;
	Node* root;

public:
	Transformation(vector<int> parentsArr) {
		this->parentsArr = parentsArr;
		multiWayArr = new MultiWayTree[parentsArr.size()];
		this->nrKeys = parentsArr.size();
	}

	void parentToMultiWayTree() {
		for (unsigned int i = 0; i < parentsArr.size(); i++) { // only 1 iteration O(n)
			if (parentsArr[i] >= 0) { // if we do not speak about the root

				multiWayArr[parentsArr[i]].key = parentsArr[i];
				multiWayArr[i].key = i;
				multiWayArr[parentsArr[i]].addChild(i);
				multiWayArr[i].setFather(parentsArr[i]);

			} else if (parentsArr[i] == -1) {  // if we speak about the root
				rootKey = i;
			}
		}
	}

	void multiwayArrToTree() {
		root = new Node(rootKey);
		transformMultiWayArrToTree(root);
	}

	void printMultiWayArr() {
		for (int i = 0; i < nrKeys; i++) {
			cout << "key: " << multiWayArr[i].key << "     ";
			cout << "father: " << multiWayArr[i].father << "     ";
			cout << "children:  ";

			for (int child : multiWayArr[i].childrenNodes) {
				cout << child << " ";
			}

			cout << endl;
		}
		cout << endl;
	}

	void printPreOrder() {
		preOrder(root);
	}

	void print2D() {
		print2DUtil(root, 0);
	}

private:
	void transformMultiWayArrToTree(Node* node) {

		int firstChild = getFirstChild(node->key);

		if (firstChild == NO_FIRST_CHILD) {

			node->left = NULL;
		} else {

			node->left = new Node(firstChild);
			transformMultiWayArrToTree(node->left);
		}

		int brother = getBrother(node->key);

		if (brother == NO_BROTHER) {

			node->right = NULL;
		} else {

			node->right = new Node(brother);
			transformMultiWayArrToTree(node->right);
		}
	}

	int getFirstChild(int key) {

		if (multiWayArr[key].childrenNodes.size() > 0) {
			// if he has children

			int keyOfFirstChild = multiWayArr[key].childrenNodes.at(0);
			return multiWayArr[keyOfFirstChild].key;
		} else {
			return NO_FIRST_CHILD;
		}
	}

	int getBrother(int key) {

		int fatherKey = multiWayArr[key].father;

		// iterate children until you find int key; save the brother in int brotherKey

		int brotherKey = NO_BROTHER;
		bool found = false;

		for (int child : multiWayArr[fatherKey].childrenNodes) {

			if (found == true) {

				brotherKey = child;
				break;
			} else { // found == false
				if (child == key) {
					found = true;
				}
			}
		}

		return brotherKey;
	}

	void preOrder(Node* node) {
		if (node == NULL) {
			return;
		}

		cout << node->key << " ";

		preOrder(node->left);

		preOrder(node->right);
	}

	void print2DUtil(Node* root, int space) {
		if (root == NULL)
			return;

		space += 10;

		print2DUtil(root->right, space);

		cout << endl;
		for (int i = 10; i < space; i++)
		{
			cout << " ";
		}

		cout << root->key << "\n";

		print2DUtil(root->left, space);
	}
};

int main() {
	Transformation trans(Transformation::getDemoParentsArr());
	trans.parentToMultiWayTree();
	trans.printMultiWayArr();
	trans.multiwayArrToTree();
	trans.printPreOrder();
	trans.print2D();

	return 0;
}