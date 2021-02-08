/*
Stoica Tudor Mihai
Grupa 3, Calculatoare Engleza

I created 3 classes for each algorithm I implemented.
For each algorithm, I identified the worst and best case:

Binary Insertion Sort:
average case: O(nlogn)
best case: ascending order -> O(nlogn)
worst case: descending order -> O(n^2)

Bubble Sort:
average case: O(n^2)
best case: ascending order -> O(n)
worst case: descending order -> O(n^2)

Select Sort:
average case: O(n^2)
best case: ascending order -> O(n^2)
worst case: descending order, but the first element is smaller than every each -> O(n^2)

Selection Sort is the most used algorithm and finds the minimum element from the unsorted array and puts it back at the start of the array.
Bubble Sort always compares and swaps 2 adjacent neighbours, if they are in a wrong order.
Binary Insertion Sort uses binary search method to find the exact position of the inserted element in the array.

*/


#include <iostream>
#include <cstdlib>
#include "Profiler.h"
#define NR_TESTS 5
#define STEP_SIZE 100
#define MAX_SIZE 10000

using namespace std;

Profiler profiler("Average Case");

class BinaryInsertionSort {

    private:
        int* arr;
        int arrSize, left, mid, right;

    public:
        BinaryInsertionSort(int* arr, int arrSize) {

            this->arr = arr;
            this->arrSize = arrSize;
        }

    public:
        void doBinaryInsertionSort() {

            Operation bisComparisons = profiler.createOperation("BinaryInsertionSort comparisons", arrSize);
            Operation bisAssigns = profiler.createOperation("BinaryInsertionSort assigns", arrSize);

            for (int i = 1; i < arrSize; i++) {
                bisAssigns.count();
                int elementIndex = getBinarySearchTheoreticalElementPosition(i, arr[i], bisComparisons, bisAssigns);

                bisComparisons.count();

                if (i != elementIndex) {
                    bisAssigns.count();
                    int valueToInsert = arr[i];

                    for (int j = i - 1; j >= elementIndex; j--) {
                        bisAssigns.count();
                        arr[j + 1] = arr[j];
                    }

                    bisAssigns.count();
                    arr[elementIndex] = valueToInsert;
                }
            }

            //            profiler.divideValues("BinaryInsertionSort comparisons", NR_TESTS);
            //            profiler.divideValues("BinaryInsertionSort assigns", NR_TESTS);
            profiler.addSeries("BinaryInsertionSort Total Operations", "BinaryInsertionSort comparisons", "BinaryInsertionSort assigns");
            free(arr);  // finally we free the memory from the array we have
            // provided
        }

    private:
        int getBinarySearchTheoreticalElementPosition(int currentIterationIndex,
                int elementToBeInserted, Operation bisComparisons, Operation bisAssigns) {
            left = 0;
            right = currentIterationIndex;

            while (left < right) {
                bisComparisons.count();
                mid = (left + right) / 2;

                bisComparisons.count();

                if (arr[mid] == elementToBeInserted) {
                    return mid;
                }

                bisComparisons.count();

                if (arr[mid] > elementToBeInserted) {
                    right = mid - 1;
                }
                else {
                    left = mid + 1;
                }
            }

            bisComparisons.count();
            return (elementToBeInserted > arr[left]) ? (left + 1) : left;
        }
};

class BubbleSort {
    private:
        int * arr;
        int arrSize;

    public:
        BubbleSort(int * arr, int arrSize) {
            this->arr = arr;
            this->arrSize = arrSize;
        }

    public:
        void doBubbleSort() {

            Operation bubbleComparisons = profiler.createOperation("BubbleSort comparisons", arrSize);
            Operation bubbleAssigns = profiler.createOperation("BubbleSort assigns", arrSize);

            bool sorted = true;
            int nrIterationsSoFar = -1;

            do {
                //            bubbleComparisons.count();
                sorted = true;
                nrIterationsSoFar++;

                for (int i = 0; i < arrSize - 1 - nrIterationsSoFar; i++) {
                    bubbleComparisons.count();

                    if (arr[i] > arr[i + 1]) {
                        sorted = false;
                        bubbleAssigns.count();
                        swap(arr[i], arr[i + 1]);
                    }
                }
            }
            while (!sorted);

            //            profiler.divideValues("BubbleSort comparisons", NR_TESTS);
            //            profiler.divideValues("BubbleSort assigns", NR_TESTS);
            profiler.addSeries("BubbleSort Total Operations", "BubbleSort comparisons", "BubbleSort assigns");
        }
};

class SelectionSort {

    private:
        int * arr;
        int arrSize;

    public:
        SelectionSort(int * arr, int arrSize) {
            this->arr = arr;
            this->arrSize = arrSize;
        }

    public:
        void doSelectionSort() {

            Operation ssComparisons = profiler.createOperation("SelectionSort comparisons", arrSize);
            Operation ssAssigns = profiler.createOperation("SelectionSort assigns", arrSize);

            for (int i = 0; i < arrSize - 1; i++) {
                for (int j = i + 1; j < arrSize; j++) {
                    ssComparisons.count();

                    if (arr[i] > arr[j]) {
                        ssAssigns.count();
                        swap(arr[i], arr[j]);
                    }
                }
            }

            //            profiler.divideValues("SelectionSort comparisons", NR_TESTS);
            //            profiler.divideValues("SelectionSort assigns", NR_TESTS);
            profiler.addSeries("SelectionSort Total Operations", "SelectionSort comparisons", "SelectionSort assigns");
        }
};

int main() {

    for (int i = 0; i < NR_TESTS; i++) {
        for (int arraySize = STEP_SIZE; arraySize < MAX_SIZE; arraySize += STEP_SIZE) {

            int* arr = new int[arraySize];
            FillRandomArray(arr, arraySize);

            int * auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            BinaryInsertionSort binaryInsertionSort(auxiliarArr, arraySize);
            binaryInsertionSort.doBinaryInsertionSort();

            auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            BubbleSort bubbleSort(auxiliarArr, arraySize);
            bubbleSort.doBubbleSort();

            auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            SelectionSort selectionSort(auxiliarArr, arraySize);
            selectionSort.doSelectionSort();

            free(arr);
            free(auxiliarArr);
        }
    }

    profiler.createGroup("Comparisons", "BinaryInsertionSort comparisons", "BubbleSort comparisons", "SelectionSort comparisons");
    profiler.createGroup("Assigns", "BinaryInsertionSort assigns", "BubbleSort assigns", "SelectionSort assigns");
    profiler.createGroup("Total", "BinaryInsertionSort Total Operations", "BubbleSort Total Operations", "SelectionSort Total Operations");
    profiler.showReport();

    profiler.reset("BestCase");

    for (int i = 0; i < NR_TESTS; i++) { /// best case
        for (int arraySize = STEP_SIZE; arraySize < MAX_SIZE; arraySize += STEP_SIZE) {

            int* arr = new int[arraySize];
            FillRandomArray(arr, arraySize, 0, 50000, false, 1);

            int * auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            BinaryInsertionSort binaryInsertionSort(auxiliarArr, arraySize);
            binaryInsertionSort.doBinaryInsertionSort();

            auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            BubbleSort bubbleSort(auxiliarArr, arraySize);
            bubbleSort.doBubbleSort();

            auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            SelectionSort selectionSort(auxiliarArr, arraySize);
            selectionSort.doSelectionSort();

            free(arr);
            free(auxiliarArr);
        }
    }

    profiler.createGroup("Comparisons", "BinaryInsertionSort comparisons", "BubbleSort comparisons", "SelectionSort comparisons");
    profiler.createGroup("Assigns", "BinaryInsertionSort assigns", "BubbleSort assigns", "SelectionSort assigns");
    profiler.createGroup("Total", "BinaryInsertionSort Total Operations", "BubbleSort Total Operations", "SelectionSort Total Operations");
    profiler.showReport();

    profiler.reset("WorstCase");

    for (int i = 0; i < NR_TESTS; i++) { /// worst case
        for (int arraySize = STEP_SIZE; arraySize < MAX_SIZE; arraySize += STEP_SIZE) {

            int* arr = new int[arraySize];
            FillRandomArray(arr, arraySize, 1, 50000, false, 2);

            int * auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            BinaryInsertionSort binaryInsertionSort(auxiliarArr, arraySize);
            binaryInsertionSort.doBinaryInsertionSort();

            auxiliarArr = new int[arraySize];

            for (int i = 0; i < arraySize; i++) {
                auxiliarArr[i] = arr[i];
            }

            BubbleSort bubbleSort(auxiliarArr, arraySize);
            bubbleSort.doBubbleSort();

            auxiliarArr = new int[arraySize];

            for (int i = 1; i < arraySize; i++) {
                auxiliarArr[arraySize - i] = arr[i];
            }

            auxiliarArr[0] = 0;

            SelectionSort selectionSort(auxiliarArr, arraySize);
            selectionSort.doSelectionSort();

            free(arr);
            free(auxiliarArr);
        }
    }

    profiler.createGroup("Comparisons", "BinaryInsertionSort comparisons", "BubbleSort comparisons", "SelectionSort comparisons");
    profiler.createGroup("Assigns", "BinaryInsertionSort assigns", "BubbleSort assigns", "SelectionSort assigns");
    profiler.createGroup("Total", "BinaryInsertionSort Total Operations", "BubbleSort Total Operations", "SelectionSort Total Operations");
    profiler.showReport();
}

