/*
    Stoica Tudor Mihai
    Group 3

     Task:
        Implement correctly and efficiently two methods for building a heap,
        namely the bottom-­up and the top-­down strategies. Additionally, implement
        heapsort.

        HeapSort BU bottom up:
        -first of all we create a max-heap, 
        by calling the heapify method for each parent in the heap
        -then we extract one by one the biggest numbers in the array, 
        we place them at the end of the array and decrement the size of the array
        - the heapify method works recursively: for a parent and its children-> the 
        biggest key comes as a parent
        if anything swaped (if the initial parent was not the biggest key -> 
        recursively repeat the algorithm for the child swaped index);
        - thus we ensure that every time we are able to extract the biggest
        key in the array which is situated at the root of the heap

        HeapSort TD top down:
        - starts with a empty heap and inserts new keys to it in a repeated manner
        - thus heapify needs to be called for every heap addition, meaning 
        that it is time consuming and it performs worse as the size of the 
        heap increases


        Average complexity: O(nlogn)

        Observations: the bottom up should outbest the top down approach,
        but it seems that I did not do something quite ok;

*/

#include <iostream>

#include "Profiler.h"
#define NR_TESTS 5
#define STEP_SIZE 100
#define MAX_SIZE 10000
using namespace std;

Profiler profiler("HeapSort");

class HeapSort {
 private:
  int* arr;
  int arrSize;

 public:
  HeapSort(int* arr, int arrSize) {
    this->arr = arr;
    this->arrSize = arrSize;
  }

  void doHeapSortBU() {
    profiler.createOperation("HeapSort assigns BU", this->arrSize);
    profiler.createOperation("HeapSort comparisons BU", this->arrSize);

    for (int i = this->arrSize / 2 - 1; i >= 0; i--) {
      heapifyBU(i, this->arrSize);
    }

    for (int i = this->arrSize - 1; i > 0; i--) {
      profiler.countOperation("HeapSort assigns BU", this->arrSize, 3);
      swap(arr[0], arr[i]);
      heapifyBU(0, i);
    }
  }

  void doHeapSortTD() {
    profiler.createOperation("HeapSort assigns TD", this->arrSize);
    profiler.createOperation("HeapSort comparisons TD", this->arrSize);

    int heapSize = 0;

    for (int i = 1; i < arrSize; i++) {
      heapSize++;
      int j = heapSize;
      profiler.countOperation("HeapSort comparisons TD", this->arrSize);

      if (arr[i] >= arr[j]) {
        profiler.countOperation("HeapSort assigns TD", this->arrSize);
        arr[j] = arr[i];
        profiler.countOperation("HeapSort comparisons TD", this->arrSize);

        while (j > 0 && arr[(j - 1) / 2] < arr[j]) {
          profiler.countOperation("HeapSort assigns TD", this->arrSize, 3);

          swap(arr[j], arr[(j - 1) / 2]);
          j = (j - 1) / 2;
        }
      }
    }
  }

  void printArray() {
    for (int i = 0; i < this->arrSize; i++) {
      cout << arr[i] << " ";
    }
    cout << "\n";
  }

  void freeMemory() { free(arr); }

 private:
  void heapifyBU(int rootIndex, int size) {
    int right = 2 * rootIndex + 1;
    int left = 2 * rootIndex + 2;
    int biggestElementIndex = rootIndex;

    profiler.countOperation("HeapSort comparisons BU", this->arrSize);
    if (left < size && arr[left] > arr[biggestElementIndex]) {
      biggestElementIndex = left;
    }

    profiler.countOperation("HeapSort comparisons BU", this->arrSize);
    if (right < size && arr[right] > arr[biggestElementIndex]) {
      biggestElementIndex = right;
    }

    if (biggestElementIndex == rootIndex) {  // if nothing changed
      return;
    } else {
      profiler.countOperation("HeapSort assigns BU", this->arrSize, 3);
      swap(arr[rootIndex], arr[biggestElementIndex]);
      heapifyBU(biggestElementIndex, size);
    }
  }
};

int main() {
  int* arr = new int[MAX_SIZE];
  int* tempArr = new int[MAX_SIZE];
  int arrSize = 0;

  for (int i = 0; i < NR_TESTS; i++) {
    for (arrSize = STEP_SIZE; arrSize <= MAX_SIZE; arrSize += STEP_SIZE) {

      FillRandomArray(arr, arrSize);

      for (int index = 0; index < arrSize; index++) {
        tempArr[index] = arr[index];
      }

      HeapSort heapSort(tempArr, arrSize);
      heapSort.doHeapSortBU();
      // heapSort.freeMemory();

      for (int index = 0; index < arrSize; index++) {
        tempArr[index] = arr[index];
      }

      heapSort = HeapSort(tempArr, arrSize);
      heapSort.doHeapSortTD();
      // heapSort.freeMemory();
    }
  }

  profiler.createGroup("Assigns", "HeapSort assigns BU", "HeapSort assigns TD");
  profiler.createGroup("Comparisons", "HeapSort comparisons BU",
                       "HeapSort comparisons TD");

  profiler.addSeries("Total operations TD", "HeapSort comparisons TD",
                     "HeapSort assigns TD");
  profiler.addSeries("Total operations BU", "HeapSort comparisons BU",
                     "HeapSort assigns BU");

  profiler.showReport();

  free(arr);
  free(tempArr);

  return 0;
}