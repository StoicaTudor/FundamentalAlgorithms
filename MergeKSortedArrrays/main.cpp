 /*
	Stoica Tudor Mihai
	Group 40423
	
	Task: You are required to implement correctly and efficiently an O(nlogk) method for merging k
		sorted sequences, where n is the total number of elements. (Hint: use a heap, see seminar no. 2
		notes). Implementation requirements:
				● Use linked lists to represent the k sorted sequences and the output sequence

	Algorithm complexity: O (n log k) or O (n * k * log k)
		
	- In order to be as efficient and fast as possible, we use a minHeap structure. A minHeap is a binary tree, in which every parent is less or equal than its children.
	- We first construct a heap with k elements -> first elements of the lists (arr[0]). After that, repeat until the heap is != NULL (or while mergedArrIndex < k * n
	-> while we still have to introduce elements in mergedArr):
	
	heapify()
	take the root as the next element and place it into the mergedArr (mergedArr is the final sorted and merged array)
	instead of the root, place element->next; if element->next is null, then place as root minHeap[k - 1] -> (minHeap[0] = minHeap[k - 1])
	
	In this way, our heap will always have size k, and if one list ends its elements, the size of the heap decreases with 1
	
	Heapify assures us that root will always be the smallest element in the minHeap 
	
	Lets denote the macro operation we are doing OP; OP means heapify + extract the next element from the lists and place it into the heap;
	heapify takes log k and the extraction takes O(1); so OP = O(log k)
	Since we do OP for every element in the lists , the overall complexity will be n*k*log k. Every list has n elements. We have k lists. Heapify takes log k. We have to multiply
	these numbers and we will get our complexity.

*/
#include <iostream>

#include "Profiler.h"

using namespace std;

Profiler profiler("Merge K N sized Arrays");

struct Node {
  struct Node* next;
  int val;
};

struct Node* newNode(int val) {
  struct Node* newNode = new Node();

  newNode->next = NULL;
  newNode->val = val;

  return newNode;
}
class MergeKSortedNArrays {
 private:
  static const int MAX_K = 100;
  Node** arr;
  int* mergedArr;
  int mergedArrIndex = 0;
  int n;
  int nn;
  int k;
  Node* minHeap[MAX_K];
  const char* assigns;
  const char* comparisons;

 public:
  MergeKSortedNArrays(Node** arr, int n, int k) {
    this->arr = arr;
    this->n = n;
    this->k = k;

    this->assigns = "Total Assigns";
    this->comparisons = "Total Comparisons";
    profiler.createOperation(assigns, n);
    profiler.createOperation(comparisons, n);

    initMergedArr();
    initMinHeap();
  }

  MergeKSortedNArrays(Node** arr, int n, int k, const char* opAssignsName,
                      const char* opComparisonsName, int nn) {
    this->arr = arr;
    this->n = n;
    this->k = k;
    this->nn = nn;

    this->assigns = opAssignsName;
    this->comparisons = opComparisonsName;
    profiler.createOperation(assigns, nn);
    profiler.createOperation(comparisons, nn);

    initMergedArr();
    initMinHeap();
  }

  void doMerging() {
    int size = k;

    while (mergedArrIndex < k * n) {
      heapify(0, size);
      addInMergedArr(minHeap[0]->val);

      profiler.countOperation(comparisons, nn, 1);
      profiler.countOperation(assigns, nn, 1);

      if (minHeap[0]->next != NULL) {
        minHeap[0] = minHeap[0]->next;
      } else {
        minHeap[0] = minHeap[k - 1];
        size--;
      }
    }
  }

  void printMergedArr() {
    for (int i = 0; i < n * k; i++) {
      cout << mergedArr[i] << " ";
    }
  }

  int* getMergedArr() { return mergedArr; }

  void freeMemory() {
    free(arr);
    free(minHeap);
  }

 private:
  void addInMergedArr(int val) {
    profiler.countOperation(assigns, nn, 1);
    mergedArr[mergedArrIndex++] = val;
  }

  void initMergedArr() { mergedArr = new int[k * n]; }

  void initMinHeap() {
    profiler.countOperation(assigns, nn, k);

    for (int i = 0; i < k; i++) {
      minHeap[i] = new Node;
      minHeap[i] = arr[i];
    }
  }

  void heapify(int index, int size) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    profiler.countOperation(comparisons, nn, 2);

    if (left < size && minHeap[smallest]->val > minHeap[left]->val) {
      smallest = left;
    }

    if (right < size && minHeap[smallest]->val > minHeap[right]->val) {
      smallest = right;
    }

    if (smallest != index) {
      profiler.countOperation(assigns, nn, 3);
      swap(minHeap[smallest], minHeap[index]);

      heapify(smallest, size);
    }
  }
};

class Task2 {
 private:
  static const int MAX_K = 100;
  Node** arr;
  int* mergedArr;
  int mergedArrIndex = 0;
  int n;
  int k;
  Node* minHeap[MAX_K];

 public:
  Task2(Node** arr, int n, int k, Operation as, Operation co) {
    this->arr = arr;
    this->n = n;
    this->k = k;

    initMergedArr();
    initMinHeap(as, co);
  }

  void doMerging(Operation as, Operation co) {
    int size = k;

    while (mergedArrIndex < k * n) {
      heapify(0, size, as, co);
      addInMergedArr(minHeap[0]->val, as, co);

      as.count();
      co.count();

      if (minHeap[0]->next != NULL) {
        minHeap[0] = minHeap[0]->next;
      } else {
        minHeap[0] = minHeap[k - 1];
        size--;
      }
    }
  }

  void printMergedArr() {
    for (int i = 0; i < n * k; i++) {
      cout << mergedArr[i] << " ";
    }
  }

  int* getMergedArr() { return mergedArr; }

  void freeMemory() {
    free(arr);
    free(minHeap);
  }

 private:
  void addInMergedArr(int val, Operation as, Operation co) {
    as.count();
    mergedArr[mergedArrIndex++] = val;
  }

  void initMergedArr() { mergedArr = new int[k * n]; }

  void initMinHeap(Operation as, Operation co) {
    as.count(k);

    for (int i = 0; i < k; i++) {
      minHeap[i] = new Node;
      minHeap[i] = arr[i];
    }
  }

  void heapify(int index, int size, Operation as, Operation co) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    co.count(2);

    if (left < size && minHeap[smallest]->val > minHeap[left]->val) {
      smallest = left;
    }

    if (right < size && minHeap[smallest]->val > minHeap[right]->val) {
      smallest = right;
    }

    if (smallest != index) {
      as.count(3);
      swap(minHeap[smallest], minHeap[index]);

      heapify(smallest, size, as, co);
    }
  }
};

void demo() {
  const int k = 3;
  int n = 4;

  Node* arr[k];

  arr[0] = new Node;
  arr[0]->val = 1;

  arr[0]->next = new Node;
  arr[0]->next->val = 3;

  arr[0]->next->next = new Node;
  arr[0]->next->next->val = 5;

  arr[0]->next->next->next = new Node;
  arr[0]->next->next->next->val = 7;
  arr[0]->next->next->next->next = NULL;

  arr[1] = new Node;
  arr[1]->val = 2;

  arr[1]->next = new Node;
  arr[1]->next->val = 4;

  arr[1]->next->next = new Node;
  arr[1]->next->next->val = 6;

  arr[1]->next->next->next = new Node;
  arr[1]->next->next->next->val = 8;
  arr[1]->next->next->next->next = NULL;

  arr[2] = new Node;
  arr[2]->val = 0;

  arr[2]->next = new Node;
  arr[2]->next->val = 9;

  arr[2]->next->next = new Node;
  arr[2]->next->next->val = 10;

  arr[2]->next->next->next = new Node;
  arr[2]->next->next->next->val = 11;
  arr[2]->next->next->next->next = NULL;

  MergeKSortedNArrays merge(arr, n, k);
  merge.doMerging();
  merge.printMergedArr();
  // merge.freeMemory();
}

bool isSorted(int* arr) {
  for (int i = 0; i < (sizeof(arr) / sizeof(arr[0])) - 1; i++) {
    if (arr[i + 1] < arr[i]) {
      return false;
    }
  }

  return true;
}

Node* getNodeListFromIntList(int* value, int arrSize) {
  Node* newArr = NULL;
  Node* first = NULL;

  for (int i = 0; i < arrSize; i++) {
    Node* newNode = new Node;
    newNode->val = value[i];
    newNode->next = NULL;

    if (newArr == NULL) {
      newArr = newNode;
      first = newArr;
    } else {
      newArr->next = newNode;
      newArr = newArr->next;
    }
  }

  newArr = first;

  return newArr;
}

void task1() {
  const int MAX_SIZE = 10000;
  const int STEP_SIZE = 400;

  for (int arrSize = STEP_SIZE; arrSize <= MAX_SIZE; arrSize += STEP_SIZE) {
    cout << arrSize << "  /  " << MAX_SIZE << "\n";

    // k=5
    Node** arr = new Node*[5];

    for (int i = 0; i < 5; i++) {
      int* templateArr = new int[arrSize];
      FillRandomArray(templateArr, arrSize, 10, 5000, false, 1);
      arr[i] = getNodeListFromIntList(templateArr, arrSize);
      free(templateArr);
    }

    MergeKSortedNArrays merge(arr, arrSize, 5, "Total Assigns k=5",
                              "Total Comparisons k=5", arrSize);
    merge.doMerging();
    // merge.freeMemory();

    free(arr);

    // k=10
    arr = new Node*[10];

    for (int i = 0; i < 10; i++) {
      int* templateArr = new int[arrSize];
      FillRandomArray(templateArr, arrSize, 10, 5000, false, 1);
      arr[i] = getNodeListFromIntList(templateArr, arrSize);
      free(templateArr);
    }

    merge = MergeKSortedNArrays(arr, arrSize, 10, "Total Assigns k=10",
                                "Total Comparisons k=10", arrSize);
    merge.doMerging();
    // merge.freeMemory();

    free(arr);

    // k=100

    arr = new Node*[100];

    for (int i = 0; i < 100; i++) {
      int* templateArr = new int[arrSize];
      FillRandomArray(templateArr, arrSize, 10, 100, false, 1);
      arr[i] = getNodeListFromIntList(templateArr, arrSize);
      free(templateArr);
    }

    merge = MergeKSortedNArrays(arr, arrSize, 100, "Total Assigns k=100",
                                "Total Comparisons k=100", arrSize);
    merge.doMerging();
    // merge.freeMemory();

    free(arr);
  }

  profiler.addSeries("Total Operations k=5", "Total Assigns k=5",
                     "Total Comparisons k=5");
  profiler.addSeries("Total Operations k=10", "Total Assigns k=10",
                     "Total Comparisons k=10");
  profiler.addSeries("Total Operations k=100", "Total Assigns k=100",
                     "Total Comparisons k=100");

  profiler.createGroup("Total Assigns", "Total Assigns k=5",
                       "Total Assigns k=10", "Total Assigns k=100");
  profiler.createGroup("Total Comparisons", "Total Comparisons k=5",
                       "Total Comparisons k=10", "Total Comparisons k=100");
  profiler.createGroup("Total Operations", "Total Operations k=5",
                       "Total Operations k=10", "Total Operations k=100");

  profiler.showReport();
}

void task2() {
  int n = 10000;

  for (int k = 10; k <= 500; k += 10) {
    Operation as = profiler.createOperation("Total Assigns", k);
    Operation co = profiler.createOperation("Total Comparisons", k);

    Node** arr = new Node*[k];
    int arrSize = n / k;

    for (int i = 0; i < k; i++) {
      int* templateArr = new int[arrSize];
      FillRandomArray(templateArr, arrSize, 10, 5000, false, 1);
      arr[i] = getNodeListFromIntList(templateArr, arrSize);
      free(templateArr);
    }

    Task2 merge(arr, arrSize, k, as, co);
    merge.doMerging(as, co);

    profiler.addSeries("Total Operations", "Total Assigns",
                       "Total Comparisons");

    free(arr);
  }

  profiler.showReport();
}

int main() {
  // task1();
   // task2();
   demo();
  return 0;
}