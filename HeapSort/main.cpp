#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

struct NodeT {
    NodeT * right, * left;
    int value;
};

class HeapSort {

    private: int * arr;
        int arrSize;
        NodeT * heapRoot;
        NodeT ** nodesQueue;
        int nodesQueueLeft, nodesQueueRight;

    public: HeapSort(int * arr, int arrSize) {
            this->arr = arr;
            this->arrSize = arrSize;
        }

        void buildHeap() {
            int maxSizeQueue = pow(2, (int)(log2(arrSize) + 1)) + arrSize;
            this->nodesQueue = new NodeT * [maxSizeQueue];
            this->nodesQueueLeft = this->nodesQueueRight = 0;

            this->heapRoot = createNewNode(arr[0], this->heapRoot);
            this->heapRoot->left = createNewNode(-1, this->heapRoot->left);
            addInQueue(this->heapRoot->left);
            this->heapRoot->right = createNewNode(-1, this->heapRoot->right);
            addInQueue(this->heapRoot->right);

            for (int i = 1; i < arrSize; i++) {
                insertNode(arr[i]);
            }

            free(this->nodesQueue);
        }

        void printHeapInOrder() {
            inOrder(this->heapRoot);
            cout << endl;
        }

    private:
        void insertNode(int value) {
            NodeT * newNode = popQueue();
            newNode->value = value; /// newNode = createNewNode(value,newNode);
            newNode->left = createNewNode(-1, newNode->left);
            addInQueue(newNode->left);
            newNode->right = createNewNode(-1, newNode->right);
            addInQueue(newNode->right);
            // cout<<this->heapRoot->value<< " "<<this->heapRoot->left->value<<endl;
        }

        NodeT * createNewNode(int value, NodeT * newNode) {
            newNode = new NodeT;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->value = value;

            return newNode;
        }

        NodeT * popQueue() {
            NodeT * nodeToReturn = nodesQueue[nodesQueueLeft];
            nodesQueueLeft++;
            return nodeToReturn;
        }

        void addInQueue(NodeT * node) {
            nodesQueue[nodesQueueRight] = node;
            nodesQueueRight++;
        }

        void inOrder(NodeT * node) {
            if (node == NULL) {

                return;
            }
            else
                if (node->value == -1) {
                    node = NULL;
                    return;
                }
                else {
                    inOrder(node->left);
                    cout << node->value << " ";
                    inOrder(node->right);
                }
        }

        void hipify(NodeT * node) {
            if (node->left->left == NULL ||
                    node->left->left->value == -1 ||
                    node->right->left == NULL ||
                    node->right->left->value == -1) {

                /// NodeT * maxiNode = getMaxiNode(node);
            }
        }

        NodeT * getMaxiNode() {
        }
};

int main() {

    int tempArr [] = {9, 6, 19, 1, 2, 3, 8, 100};
    int arrSize = (int)(sizeof(tempArr) / sizeof(int));
    int * arr = new int[arrSize];

    for (int i = 0; i < arrSize; i++) {
        arr[i] = tempArr[i];
    }

    HeapSort heapSort(arr, arrSize);
    heapSort.buildHeap();
    heapSort.printHeapInOrder();

    return 0;
}
