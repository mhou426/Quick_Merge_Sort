#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE_SIZE 100 //where size considered as small
#define Num 100000
#define swap(a, b) {int temp = *(a); *(a) = *(b); *(b) = temp;}

int callQuick = 0;
int callMerge = 0;

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

// Partition function (Lomuto partition scheme)
int partition(int arr[], int low, int high) {
    // // Choose random pivot
    // int randomPivotIndex = low + rand() % (high - low + 1);
    // // Swap the pivot element with the last element
    // swap(&arr[randomPivotIndex], &arr[high]);

    int pivot = arr[high];  // Pivot element is last
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

void verifyArray(int arr[], int size) {
    for (int i = 0; i < size - 1; i++)
        //compare two adjacent
        if(arr[i] > arr[i+1]){
            printf("\nNot Sorted.\n");
            return;
        }
        printf("\nSorted.\n");
}

// Main function to test the Quick Sort implementation
int main() {
    srand(time(0));  // For random pivot selection
    int *arr = (int *)malloc(Num * sizeof(int));
    int *duplicateArr = (int *)malloc(Num * sizeof(int));

    // CASE I : Sorted list
    for (int i = 0; i < Num; i++) {
        arr[i] = i + 1;
        duplicateArr[i] = arr[i];  // Duplicate the original array
    }
    int n = Num;

    //CASE II: Mostly sorted but some will be swap to form a list
    for (int i = 0; i < Num/1; i++) {
        int index1 = rand() % n;
        int index2 = rand() % n;
        
        // Swap elements at index1 and index2
        int temp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = temp;
    }
    
    // Measure time for the original Quick Sort
    clock_t start = clock();
    quickSort(arr, 0, n - 1);
    clock_t end = clock();
    double timeOriginal = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Original Quick Sort time: %f seconds\n", timeOriginal);

    verifyArray(arr, Num);

    free(arr);
    free(duplicateArr);
    return 0;
}
