#include <iostream>

using namespace std;

class MergeSort {
 private:
  int* arr;
  int arrSize;

 public:
  MergeSort(int* arr, int arrSize) {
    this->arr = arr;
    this->arrSize = arrSize;
  }

  void doMergeSort(int left, int right) {
    if (left < right) {
      int mid = (left + right) / 2;

      doMergeSort(left, mid);
      doMergeSort(mid + 1, right);

      forge(left, mid, right);
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
  void forge(int left, int mid, int right) {
    /*
     * I. left......mid
     * II. mid+1......right
     */

    // don't forget to try to deallocate the memory here

    int* tempArr = new int[right - left + 1];
    int indexI = left, indexII = mid + 1, tempArrIndex = 0;

    while (indexI <= mid && indexII <= right) {
      if (arr[indexI] < arr[indexII]) {
        tempArr[tempArrIndex++] = arr[indexI++];
      } else {
        tempArr[tempArrIndex++] = arr[indexII++];
      }
    }

    while (indexI <= mid) {
      tempArr[tempArrIndex++] = arr[indexI++];
    }

    while (indexII <= right) {
      tempArr[tempArrIndex++] = arr[indexII++];
    }

    tempArrIndex = 0;

    for (int i = left; i <= right; i++) {
      arr[i] = tempArr[tempArrIndex++];
    }
  }
};

int main() {
  int arr[] = {0, 5, 1, 9, 2, 7, 3, 8, 4, 6};
  int arrSize = sizeof(arr) / sizeof(arr[0]);

  MergeSort mergeSort(arr, arrSize);
  mergeSort.doMergeSort(0, arrSize - 1);
  mergeSort.printArray();
  mergeSort.freeMemory();

  return 0;
}