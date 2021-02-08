/*
    Stoica Tudor Mihai
    Group 3

    Task: implement quick sort and compare it to heap sort
    implement quick select

    QuickSort speed complexity: 
    best : O(n log n)
    average : O(n log n)
    worst : O(n^2)

    QuickSort space complexity:
    extra O(log n) -> stack space

    -QuickSort works recursively, by partitioning the array and recursively repeat the algorithm for the right side and left side of the partioned array
    -once the partition method has been executed, the PIVOT (which can be choosen: the LAST ELEMENT, the FIRST ELEMENT, the MIDDLE ELEMENT or a RANDOM ELEMENT)
    (I have done in my assignement all these 4 situations) will be placed at its proper position; moreover, all elements which are smaller than the pivot are
    placed on the pivot's left side and all elements bigger than the pivot are placed on the pivot's right side
    -maybe I can describe better the partition method in a example
    0 9 7 23 4 6        12       10 3 -> take 12 as a pivot; 
    after partitioning we will have:
    0 9 7 4 6     12     23 10 3
    thus, 12 is at its proper position and all its smaller friends are placed at its left, while all its bigger buddies are placed at its right
    - if we repeat this partitioning for 0 9 7 4 6 and 23 10 3 and so on, for each extra partition which will be generated, all elements will be placed
    at their proper position; thus, we will have our array sorted
    - in practice, quick sort randomized performs the best; quick sort performs better than heap sort, since heap sort must do several more swaps when heapifying
    */

#include <cstdlib>
#include <iostream>

#include "Profiler.h"
#define NR_TESTS 5
#define STEP_SIZE 100
#define MAX_SIZE 10000

using namespace std;

Profiler profiler("Lab 3 profiler");

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

class QuickSort {
 private:
  int* arr;
  int arrSize;
  int quickSortType;
  const char* assigns;
  const char* comparisons;

 public:
  static const int LAST_ELEMENT = 0;
  static const int FIRST_ELEMENT = 1;
  static const int MIDDLE_ELEMENT = 2;
  static const int RANDOM_ELEMENT = 3;

 public:
  QuickSort(int* arr, int arrSize, const int quickSortType,
            const char* opAssignsName, const char* opComparisonsName) {
    this->arr = arr;
    this->arrSize = arrSize;
    this->quickSortType = quickSortType;
    this->assigns = opAssignsName;
    this->comparisons = opComparisonsName;

    profiler.createOperation(opAssignsName, this->arrSize);
    profiler.createOperation(opComparisonsName, this->arrSize);
  }

  void doQuickSort(int left, int right) {
    if (left < right) {
      int pivotIndex = partitionAndGetNewPivotPosition(left, right);
      doQuickSort(left, pivotIndex - 1);
      doQuickSort(pivotIndex + 1, right);
    }
  }

  void freeMemory() { free(arr); }

  void printArray() {
    for (int i = 0; i < arrSize; i++) {
      cout << arr[i] << " ";
    }
    cout << "\n";
  }

 private:
  int partitionAndGetNewPivotPosition(int left, int right) {
    int pivotIndex = right;
    int j = left - 1;
    int i;

    switch (this->quickSortType) {
      case QuickSort::LAST_ELEMENT:
        pivotIndex = right;
        break;

      case QuickSort::FIRST_ELEMENT:
        profiler.countOperation(this->assigns, this->arrSize, 3);
        swap(arr[right], arr[left]);
        break;

      case QuickSort::MIDDLE_ELEMENT:
        profiler.countOperation(this->assigns, this->arrSize, 3);
        swap(arr[right], arr[(left + right) / 2]);
        break;

      case QuickSort::RANDOM_ELEMENT:
        profiler.countOperation(this->assigns, this->arrSize, 3);
        swap(arr[right], arr[rand() % (right - left) + left]);
        break;

      default:
        pivotIndex = right;
        break;
    }

    for (i = left; i < right; i++) {
      profiler.countOperation(this->comparisons, this->arrSize);
      if (arr[pivotIndex] > arr[i]) {
        j++;
        profiler.countOperation(this->assigns, this->arrSize, 3);
        swap(arr[i], arr[j]);
      }
    }
    profiler.countOperation(this->assigns, this->arrSize);
    swap(arr[pivotIndex], arr[j + 1]);

    return (j + 1);
  }
};

class QuickSelect {
 private:
  int* arr;
  int arrSize;

 public:
  QuickSelect(int* arr, int arrSize) {
    this->arr = arr;
    this->arrSize = arrSize;
  }

  int findTheNSmallestElement(int left, int right, int n) {
    if (n <= right - left + 1 && n > 0) {
      int pivotIndex = doPartition(left, right);

      if (pivotIndex - left == n - 1) {
        return arr[pivotIndex];
      }

      if (pivotIndex - left > n - 1) {
        return findTheNSmallestElement(left, pivotIndex - 1, n);
      } else {
        return findTheNSmallestElement(pivotIndex + 1, right,
                                       n - pivotIndex + left - 1);
      }
    }
  }

  void printArray() {
    for (int i = 0; i < arrSize; i++) {
      cout << arr[i] << " ";
    }

    cout << "\n";
  }

  void freeMemory() { free(arr); }

 private:
  int doPartition(int left, int right) {
    int pivotIndex = right;
    int j = left;

    for (int i = left; i < right; i++) {
      if (arr[i] <= arr[pivotIndex]) {
        swap(arr[j], arr[i]);
        j++;
      }
    }
    swap(arr[j], arr[right]);
    return j;
  }
};

int main() {
  int* arr = new int[MAX_SIZE];
  int* tempArr = new int[MAX_SIZE];

  for (int i = 0; i < NR_TESTS; i++) {
    for (int arrSize = STEP_SIZE; arrSize <= MAX_SIZE; arrSize += STEP_SIZE) {
      FillRandomArray(arr, arrSize);

      for (int k = 0; k < arrSize; k++) {
        tempArr[k] = arr[k];
      }

      QuickSort quickSortL(tempArr, arrSize, QuickSort::LAST_ELEMENT,
                           "QuickSort assigns LAST_ELEMENT",
                           "QuickSort comparisons LAST_ELEMENT");
      quickSortL.doQuickSort(0, arrSize - 1);
      quickSortL.freeMemory();

      for (int k = 0; k < arrSize; k++) {
        tempArr[k] = arr[k];
      }

      QuickSort quickSortF(tempArr, arrSize, QuickSort::FIRST_ELEMENT,
                           "QuickSort assigns FIRST_ELEMENT",
                           "QuickSort comparisons FIRST_ELEMENT");
      quickSortF.doQuickSort(0, arrSize - 1);
      quickSortF.freeMemory();

      for (int k = 0; k < arrSize; k++) {
        tempArr[k] = arr[k];
      }

      QuickSort quickSortM(tempArr, arrSize, QuickSort::MIDDLE_ELEMENT,
                           "QuickSort assigns MIDDLE_ELEMENT",
                           "QuickSort comparisons MIDDLE_ELEMENT");
      quickSortM.doQuickSort(0, arrSize - 1);
      quickSortM.freeMemory();

      for (int k = 0; k < arrSize; k++) {
        tempArr[k] = arr[k];
      }

      QuickSort quickSortR(tempArr, arrSize, QuickSort::RANDOM_ELEMENT,
                           "QuickSort assigns RANDOM_ELEMENT",
                           "QuickSort comparisons RANDOM_ELEMENT");
      quickSortR.doQuickSort(0, arrSize - 1);
      quickSortR.freeMemory();

      for (int k = 0; k < arrSize; k++) {
        tempArr[k] = arr[k];
      }

      HeapSort heapSort(tempArr, arrSize);
      heapSort.doHeapSortBU();
      heapSort.freeMemory();
    }
  }

  profiler.createGroup(
      "Assigns", "QuickSort assigns LAST_ELEMENT",
      "QuickSort assigns FIRST_ELEMENT", "QuickSort assigns MIDDLE_ELEMENT",
      "QuickSort assigns RANDOM_ELEMENT", "HeapSort assigns BU");

  profiler.createGroup("Comparisons", "QuickSort comparisons LAST_ELEMENT",
                       "QuickSort comparisons FIRST_ELEMENT",
                       "QuickSort comparisons MIDDLE_ELEMENT",
                       "QuickSort comparisons RANDOM_ELEMENT",
                       "HeapSort comparisons BU");

  profiler.addSeries("Total operations QS LAST",
                     "QuickSort assigns LAST_ELEMENT",
                     "QuickSort comparisons LAST_ELEMENT");
  profiler.addSeries("Total operations QS FIRST",
                     "QuickSort assigns FIRST_ELEMENT",
                     "QuickSort comparisons FIRST_ELEMENT");
  profiler.addSeries("Total operations QS MIDDLE",
                     "QuickSort assigns MIDDLE_ELEMENT",
                     "QuickSort comparisons MIDDLE_ELEMENT");
  profiler.addSeries("Total operations QS RANDOM",
                     "QuickSort assigns RANDOM_ELEMENT",
                     "QuickSort comparisons RANDOM_ELEMENT");
  profiler.addSeries("Total operations HS BU", "HeapSort assigns BU",
                     "HeapSort comparisons BU");

  profiler.createGroup("Total", "Total operations QS LAST",
                       "Total operations QS FIRST",
                       "Total operations QS MIDDLE",
                       "Total operations QS RANDOM", "Total operations HS BU");

  profiler.showReport();

  arr = new int[20];
  FillRandomArray(arr, 20);
  int arrSize = 20;

  QuickSelect quickSelect(arr, arrSize);
  cout << quickSelect.findTheNSmallestElement(0, arrSize - 1, 3) << "\n";
  quickSelect.printArray();
  quickSelect.freeMemory();

  free(arr);
  free(tempArr);

  return 0;
}