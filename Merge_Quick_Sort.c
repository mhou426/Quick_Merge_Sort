#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE_SIZE 1000 //where size considered as small
#define C 1.5 //constant size to determine if the alogorithm run more merge or more quick, 1-10
#define Num 1000000
#define SortingVal 1
#define swap(a, b) {int temp = *(a); *(a) = *(b); *(b) = temp;}

int callQuick = 0; // count the number of time calling partition
int callMerge = 0; // count the number of time calling merge function

void insertionSort(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;

        // Move elements of arr[low..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Merge two halves into a sorted array
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *leftArr = (int *)malloc(n1 * sizeof(int));
    int *rightArr = (int *)malloc(n2 * sizeof(int));

    // Copy data to temporary arrays leftArr[] and rightArr[]
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    // Copy the remaining elements of leftArr[]
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    // Copy the remaining elements of rightArr[]
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

void mergeQuickSort(int arr[], int left, int right, int N, int depthLimit) {
    if (left < right) {
        int n = right - left + 1;
        if (n < BASE_SIZE){
            insertionSort(arr, left, right);
            return;
        }
        else{
            int mid = left + (right - left) / 2;
            callMerge ++;
            quickMergeSort(arr, left, mid, N, depthLimit);
            quickMergeSort(arr, mid + 1, right, N, depthLimit);
            merge(arr, left, mid, right);
        }
        return;
    }
}

int partition(int arr[], int low, int high) {
    // Below two lines are for selecting random pivot
    //int randomPivotIndex = low + rand() % (high - low + 1);
    // Swap the pivot element with the last element
    //swap(&arr[randomPivotIndex], &arr[high]);

    int pivot = arr[high];  // Pivot is the last element
    int i = low - 1;        // Index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// We want to track the worst case and switch to merge sort to avoid worst running time, mostly still quick sort
void quickMergeSort(int arr[], int low, int high, int N, int depthLimit) {
    if (low < high) {
        int n = high - low + 1; //size of the partition
        if (n < BASE_SIZE){
            insertionSort(arr, low, high);
            return;
        }
        else{
            //check for imbalance, if is imbalance, run merge step
            if(n > (C * N/(2*depthLimit))){
                mergeQuickSort(arr, low, high, N, depthLimit);
            }
            else{
                callQuick ++;
                int pi = partition(arr, low, high);  // Partitioning index
                quickMergeSort(arr, low, pi - 1, N, depthLimit+1);// Sort elements before
                quickMergeSort(arr, pi + 1, high, N, depthLimit +1);// Sort elements after
            }
        }
        return;
    }
}

// orginial quick sort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int n = high - low + 1;
        if (n < BASE_SIZE){
            insertionSort(arr, low, high);
            return;
        }
        else{
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
}

// orginial merge sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int n = right - left + 1;
        if (n < BASE_SIZE){
            insertionSort(arr, left, right);
            return;
        }
        else{
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
}

// just to check if the output is fully sorted
void verifyArray(int arr[], int size) {
    for (int i = 0; i < size - 1; i++)
        //compare two adjacent
        if(arr[i] > arr[i+1]){
            printf("\nNot Sorted.\n");
            return;
        }
        printf("\nSorted.\n");
}

int main() {
    srand(time(0));
    int *arr = (int *)malloc(Num * sizeof(int));
    int *duplicateArr = (int *)malloc(Num * sizeof(int));

    // CASE I : Generate sorted list
    for (int i = 0; i < Num; i++) {
        arr[i] = i + 1;
        duplicateArr[i] = arr[i];  // Duplicate the original array, for resetting the array
    }
    int n = Num;

    //CASE II: By changing SortingVal, we can control how much sorted is the input array, adjust to add numbers of swap
    for (int i = 0; i < Num/SortingVal; i++) {
        int index1 = rand() % n;
        int index2 = rand() % n;
        
        // Swap elements at index1 and index2
        int temp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = temp;
    }
    //copy
    for (int i = 0; i < n; i++) {
        duplicateArr[i] = arr[i];
    }
    
    clock_t start = clock();
    quickMergeSort(arr, 0, n - 1, n, 1); //need to start depth at 1 to avoid divide by 0
    clock_t end = clock();
    double timeRevised = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Revised Quick Sort time: %f seconds\n", timeRevised);
    printf("Quick Sort is called %d times.\n", callQuick);
    printf("Merge Sort is called %d times.\n", callMerge);
    verifyArray(arr, Num);

     // Reset arr
    for (int i = 0; i < n; i++) {
        arr[i] = duplicateArr[i];
    }

    // Measure time for the original Quick Sort
    start = clock();
    quickSort(arr, 0, n - 1);
    end = clock();
    double timeOriginal = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Original Quick Sort time: %f seconds\n", timeOriginal);
    verifyArray(arr, Num);

    // Reset arr
    for (int i = 0; i < n; i++) {
        arr[i] = duplicateArr[i];
    }

    // Measure time for the original Merge Sort
    start = clock();
    mergeSort(arr, 0, n - 1);
    end = clock();
    double timeOriginal2 = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Original Merge Sort time: %f seconds\n", timeOriginal2);
    verifyArray(arr, Num);
    
    free(arr);
    free(duplicateArr);
    return 0;
}
