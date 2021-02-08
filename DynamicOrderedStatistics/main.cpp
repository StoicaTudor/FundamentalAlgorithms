/*
	Stoica Tudor Mihai
	Group 30423

	Task: 
	statistics tree (chapter 14.1 from the book 1 ).
	You have to use a balanced, augmented Binary Search Tree. Each node in the tree holds, besides
	the necessary information, also the size field (i.e. the size of the sub-tree rooted at the node).
	The management operations of an order statistics tree are:
	● BUILD_TREE(n)
	○ builds a balanced BST containing the keys 1,2,...n ( hint: use a divide and conquer
	approach)
	○ make sure you initialize the size field in each tree node
	● OS-SELECT(tree, i)
	○ selects the element with the ith smallest key
	○ the pseudo-code is available in chapter 14.1 from the book 1
	● OS-DELETE(tree, i)
	○ you may use the deletion from a BST, without increasing the height of the tree
	(why don’t you need to rebalance the tree?)
	○ keep the size information consistent after subsequent deletes
	○ there are several alternatives to update the size field without increasing the
	complexity of the algorithm (it is up to you to figure this out).

	BUILD_TREE: 
	-> we use DIVIDE ET IMPERA; since the array is sorted, we use recursive calls, by splitting the array into 2 halves
	-> we use the children's size, in order to build parent's size

	// create the node with its left child, the call of buildTree in the interval (left, midIndex) and its right child, (midIndex + 1, right)
		Tree* root = new Tree(mid, buildTree(left, midIndex, assigns, comparisons), buildTree(midIndex + 1, right, assigns, comparisons));

		comparisons.count();
		if (root->left != NULL) {
			assigns.count();
			root->size += root->left->size;
			// the children help the parent to create its size
		}

		comparisons.count();
		if (root->right != NULL) {
			assigns.count();
			root->size += root->right->size;
			// the children help the parent to create its size
		}

		OS_SELECT: O(log n) -> text book algorithm; we use nodes sizes
		OS_DELTE: use OS_SELECT to identify k th smallest element, then delete it, using regular bst delete

*/

#include <iostream>
#include<vector>
#include<cstdlib>
#include "Profiler.h"

#define NR_TESTS 5
#define STEP_SIZE 100
#define MAX_SIZE 10000

Profiler profiler("DynamicOrderStatistics");

using namespace std;

class DynamicOrderStatistics {

public:
	struct Tree {

		int val;
		Tree* left, * right;
		int size = 1;

	public:
		Tree(int val, Tree* left, Tree* right) {
			this->val = val;
			this->left = left;
			this->right = right;
		}
	};

private:
	const int SPACE = 13;
	vector<int> arr;

public:

	Tree* root;

	static vector<int> getDemoInputArr() {

		vector <int> arr;

		arr.push_back(0);
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		arr.push_back(4);
		arr.push_back(5);
		arr.push_back(6);
		//arr.push_back(7);
		//arr.push_back(8);
		//arr.push_back(9);

		return arr;
	}

	DynamicOrderStatistics(vector<int> arr) {
		this->arr = arr;
	}

	Tree* buildTree(int left, int right, Operation assigns, Operation comparisons) {
		if (left >= right) {

			return NULL;
		}

		// since the array is sorted, the max element is the right element
		int midIndex = (left + right) / 2;
		int mid = arr[midIndex];

		// create the node with its left child, the call of buildTree in the interval (left, midIndex) and its right child, (midIndex + 1, right)
		Tree* root = new Tree(mid, buildTree(left, midIndex, assigns, comparisons), buildTree(midIndex + 1, right, assigns, comparisons));

		comparisons.count();
		if (root->left != NULL) {
			assigns.count();
			root->size += root->left->size;
			// the children help the parent to create its size
		}

		comparisons.count();
		if (root->right != NULL) {
			assigns.count();
			root->size += root->right->size;
			// the children help the parent to create its size
		}

		return root;
	}

	void inOrderKeys(Tree* node) {

		if (node != NULL) {

			inOrderKeys(node->left);
			cout << node->val << " ";
			inOrderKeys(node->right);
		}
	}

	void inOrderSize(Tree* node) {

		if (node != NULL) {

			inOrderSize(node->left);
			cout << node->size << " ";
			inOrderSize(node->right);
		}
	}

	int getArrSize() {
		return arr.size();
	}

	Tree* OS_SELECT(Tree* node, int index, Operation assigns, Operation comparisons) {

		comparisons.count();
		if (node == NULL) {
			return NULL;
		}


		int count = -1;

		comparisons.count();
		if (node->right == NULL) {

			assigns.count();
			count = node->size;
		} else {

			assigns.count();
			count = node->size - node->right->size;
		}


		if (count == index) {

			return node;
		}

		if (count > index) {

			return OS_SELECT(node->left, index, assigns, comparisons);
		}

		return OS_SELECT(node->right, index - count, assigns, comparisons);
	}

	void OS_DELETE(Tree* rootNode, int index, Operation assigns, Operation comparisons) {

		Tree* nodeToDelete = OS_SELECT(rootNode, index, assigns, comparisons);

		assigns.count();
		this->root = deleteNode(rootNode, nodeToDelete, assigns, comparisons);
	}

	void prettyPrint(Tree* node, int space) {

		if (node == NULL) {
			return;
		}

		space += SPACE;

		prettyPrint(node->right, space);

		cout << endl;

		for (int i = SPACE; i < space; i++) {

			cout << " ";
		}

		cout << node->val << "\n";

		prettyPrint(node->left, space);
	}

	static vector<int> convertIntPointerToIntVector(int* array) {

		vector <int> intVector;

		for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {

			intVector.push_back(array[i]);
		}

		return intVector;
	}

private:

	Tree* minValue(Tree* node, Operation assigns, Operation comparisons) {

		assigns.count();
		Tree* current = node;

		comparisons.count(2);
		while (current && current->left != NULL) {

			assigns.count();
			current = current->left;
		}

		return current;
	}

	Tree* deleteNode(Tree* node, Tree* nodeToDelete, Operation assigns, Operation comparisons) {

		comparisons.count();
		if (node == NULL) {

			return NULL;
		}

		assigns.count();
		node->size--;

		comparisons.count();
		if (nodeToDelete->val < node->val) {

			assigns.count();
			node->left = deleteNode(node->left, nodeToDelete, assigns, comparisons);
		}

		else if (nodeToDelete->val > node->val) {

			assigns.count();
			node->right = deleteNode(node->right, nodeToDelete, assigns, comparisons);
		}

		else {

			comparisons.count();
			if (node->left == NULL) {

				assigns.count();
				Tree* aux = node->right;
				free(node);
				return aux;

			} else if (node->right == NULL) {

				assigns.count();
				Tree* aux = node->left;
				free(node);
				return aux;
			}

			Tree* aux = minValue(node->right, assigns, comparisons);

			assigns.count();
			node->val = aux->val;

			assigns.count();
			node->right = deleteNode(node->right, aux, assigns, comparisons);
		}

		return node;
	}
};

void runDemo() {

	DynamicOrderStatistics dos(DynamicOrderStatistics::getDemoInputArr());

	Operation assigns = profiler.createOperation("Assigns", dos.getArrSize());
	Operation comparisons = profiler.createOperation("Comparisons", dos.getArrSize());

	dos.root = dos.buildTree(0, dos.getArrSize(), assigns, comparisons);

	cout << "In Order Traversal For Values:\n";
	dos.inOrderKeys(dos.root);
	cout << endl << endl;

	cout << "In Order Traversal For Sizes:\n";
	dos.inOrderSize(dos.root);
	cout << endl << endl;

	cout << "Pretty Print:\n";
	dos.prettyPrint(dos.root, 0);
	cout << endl << endl;

	for (int i = 1; i <= dos.getArrSize(); i++) {

		int aux = i;
		DynamicOrderStatistics::Tree* res = dos.OS_SELECT(dos.root, i, assigns, comparisons);

		if (res == NULL) {
			cout << "There are less than " << i << " nodes in the BST\n";
		} else {
			cout << i << " th Smallest Element is " << res->val << endl;
		}
	}

	cout << "\nAttempting to delete the 2 nd smallets element:\n";
	dos.OS_DELETE(dos.root, 2, assigns, comparisons);
	cout << "Deletion Succeded\n";
	cout << endl << endl;

	cout << "Attempting to delete the 3 rd smallets element:\n";
	dos.OS_DELETE(dos.root, 3, assigns, comparisons);
	cout << "Deletion Succeded\n";
	cout << endl << endl;

	cout << "In Order Traversal For Values:\n";
	dos.inOrderKeys(dos.root);
	cout << endl << endl;

	cout << "In Order Traversal For Sizes:\n";
	dos.inOrderSize(dos.root);
	cout << endl << endl;

	cout << "Pretty Print:\n";
	dos.prettyPrint(dos.root, 0);
	cout << endl << endl;

	for (int i = 1; i <= dos.getArrSize(); i++) {

		int aux = i;
		DynamicOrderStatistics::Tree* res = dos.OS_SELECT(dos.root, i, assigns, comparisons);

		if (res == NULL) {
			cout << "There are less than " << i << " nodes in the BST\n";
		} else {
			cout << i << "th Smallest Element is " << res->val << endl;
		}
	}
}

void mainTask() {

	for (int i = 0; i < NR_TESTS; i++) {

		for (int arrSize = STEP_SIZE; arrSize <= MAX_SIZE; arrSize += STEP_SIZE) {

			int* arr = new int[arrSize];

			FillRandomArray(arr, arrSize, 0, arrSize, false, 1);

			Operation assigns = profiler.createOperation("Assigns", arrSize);
			Operation comparisons = profiler.createOperation("Comparisons", arrSize);

			DynamicOrderStatistics dos(DynamicOrderStatistics::convertIntPointerToIntVector(arr));
			dos.buildTree(0, dos.getArrSize(), assigns, comparisons);

			int range = arrSize;

			for (int i = 0; i < arrSize; i++) {

				if (rand() % 2 == 0) {

					dos.OS_SELECT(dos.root, rand() % range, assigns, comparisons);
				} else {
					dos.OS_DELETE(dos.root, rand() % range, assigns, comparisons);
					range--;
				}
			}
			profiler.addSeries("Total Operations", "Assigns", "Comparisons");
			free(arr);
		}
	}

	profiler.showReport();
}

int main() {

	runDemo();
	// mainTask();

	return 0;
}