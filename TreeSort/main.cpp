#include <iostream>
#include <cstdlib>
#include "Profiler.h"
#define NR_TESTS 100

using namespace std;

Profiler profiler("TreeSort Profiler");

struct NodeT {
    int value;
    NodeT * parent;
    NodeT * left;
    NodeT * right;
};

class BinaryTree {
    private:
        int arrSize;
        int * arr;
        int * sortedArray;
        NodeT * treeRoot;
        int index;

        NodeT * newNode(int value, Operation tsComparisons, Operation tsAssigns) {
            NodeT * node = new NodeT;
            tsAssigns.count();
            node -> value = value;
            tsAssigns.count();
            tsAssigns.count();
            node -> right = node -> left = NULL;
            return node;
        }

        NodeT * insertNode(NodeT * node, int value, Operation tsComparisons, Operation tsAssigns) {
            tsComparisons.count();

            if (node == NULL) {
                return newNode(value, tsComparisons, tsAssigns);
            }

            tsComparisons.count();

            if (value < node -> value) {
                tsAssigns.count();
                node->left = insertNode(node->left, value, tsComparisons, tsAssigns);
            }
            else {
                tsAssigns.count();
                node->right = insertNode(node->right, value, tsComparisons, tsAssigns);
            }

            return node;
        }

    public:
        BinaryTree(int * arr, int arrSize) {
            this -> arr = arr;
            this -> arrSize = arrSize;
            this->treeRoot = NULL;
            this->sortedArray = new int[this->arrSize];

//            for(int i=0;i<arrSize;i++){
//                cout<<arr[i]<<" ";
//            }
        }

        NodeT * getTreeRoot() {
            return treeRoot;
        }

        void createBinaryTree(Operation tsComparisons, Operation tsAssigns) {
            this->treeRoot = insertNode(treeRoot, arr[0], tsComparisons, tsAssigns);

            for (int i = 1; i < arrSize ; i++) {
                insertNode(treeRoot, arr[i], tsComparisons, tsAssigns);
            }
        }

        void printInOrder() {
            inOrder(this->treeRoot);
            cout << endl;
        }

        void inOrder(NodeT * node) {
            if (node != NULL) {
                inOrder(node->left);
                cout << node->value << " ";
                inOrder(node->right);
            }
            else {
                return;
            }
        }

        int * getSortedArray(Operation tsComparisons, Operation tsAssigns) {
            this->index = 0;
            sortedArrayInOrder(this->treeRoot, tsComparisons, tsAssigns);
            // cout<<this->index<<endl;
            return this->sortedArray;
        }

        void sortedArrayInOrder(NodeT * node, Operation tsComparisons, Operation tsAssigns) {
            tsComparisons.count();

            if (node != NULL) {
                sortedArrayInOrder(node->left, tsComparisons, tsAssigns);
                tsAssigns.count();
                sortedArray[index++] = node->value;
                sortedArrayInOrder(node->right, tsComparisons, tsAssigns);
            }
            else {
                return;
            }
        }

        void freeMemory() {
            free(this->arr);
            free(this->treeRoot);
        }
};


int main() {

//    int templateArray [] = {9, 2, 6, 1, 0, 4};
//    int arrSize = (int)(sizeof(templateArray) / sizeof(int));
//    int * arr = new int[arrSize];
//
//        Operation tsComparisons = profiler.createOperation("TreeSort Comparisons", arrSize);
//    Operation tsAssigns = profiler.createOperation("TreeSort Assigns", arrSize);
//
//    for (int i = 0; i < arrSize; i++) {
//        arr[i] = templateArray[i];
//    }
//
//    BinaryTree binaryTree(arr, arrSize);
//    binaryTree.createBinaryTree(tsComparisons, tsAssigns);
//    // binaryTree.printInOrder();
//    int * sortedArr = binaryTree.getSortedArray(tsComparisons, tsAssigns);
//
//    for (int i = 0; i < arrSize; i++) {
//        cout << sortedArr[i] << " ";
//    }
//
//    binaryTree.freeMemory();
//    profiler.showReport();


        int arrSize = 0;
        int * arr;

        for (int i = 0; i < NR_TESTS; i++) {
            arrSize +=100;
            arr=new int[arrSize + 5];
            Operation tsComparisons = profiler.createOperation("TreeSort Comparisons", arrSize);
            Operation tsAssigns = profiler.createOperation("TreeSort Assigns", arrSize);
            FillRandomArray(arr, arrSize);
            BinaryTree binaryTree(arr, arrSize);
            binaryTree.createBinaryTree(tsComparisons, tsAssigns);
            binaryTree.getSortedArray(tsComparisons, tsAssigns);
            binaryTree.freeMemory();
            free(arr);
        }

        profiler.showReport();
    return 0;
}
