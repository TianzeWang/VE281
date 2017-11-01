#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <ctime>
#include "sort.h"

using namespace std;

int main() {
    int sort_num, len;
    double dur;
    clock_t start, end;
    cin >> sort_num;
    cin >> len;
    int *arr = new int[len];
    int i;
    for (i = 0; i < len; i++) {
        cin >> arr[i];
    }
    start = clock();
    if (sort_num == 0) {
        bubble(arr, len);
    } else if (sort_num == 1) insertion(arr, len);
    else if (sort_num == 2) selection(arr, len);
    else if (sort_num == 3) merge(arr, 0, len - 1, len);
    else if (sort_num == 4) quick_extra(arr, len);
    else if (sort_num == 5) quick_inPlace(arr, 0, len - 1);
    end = clock();

//    for (i = 0; i < len; i++) {
//        cout << arr[i] << endl;
//    }
    dur = (double) end - start;
    cout << dur / CLOCKS_PER_SEC << endl;
//    cout << len;

    delete[] arr;
    return 0;
}

void bubble(int arr[], int length) {
    int i, j;
    for (i = 0; i < length; i++) {
        for (j = 0; j < length - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void insertion(int arr[], int length) {
    int i, j;
    for (i = 1; i < length; i++) {
        auto temp = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void selection(int arr[], int length) {
    int i, j, temp, temp_id;
    for (i = 0; i < length - 1; i++) {
        temp = arr[i];
        temp_id = i;
        for (j = i + 1; j < length; j++) {
            if (arr[j] < temp) {
                temp = arr[j];
                temp_id = j;
            }
        }
        swap(arr[i], arr[temp_id]);
    }
}


//void merge_sort(int arr[], int left, int mid, int right) {
//    int i = 0, j = 0, k = 0;
//    //int arr2[right - left + 1];
//    auto *arr2 = new int[right-left+1];
//    while (i < mid - left + 1 && j < right - mid) {
//        if (arr[i] <= arr[j + mid]) arr2[k++] = arr[i++];
//        else arr2[k++] = arr[mid + (j++)];
//    }
//    if (i == mid - left) {
//        while (k < right) {
//            arr2[k] = arr[mid + j];
//            k++;
//            j++;
//        }
//    } else {
//        while (k < right) {
//            arr2[k] = arr[mid + i];
//            i++;
//            k++;
//        }
//    }
//    delete[] arr2;
//}


void merge_sort(int arr[], int left, int mid, int right, int len) {
    auto arr2 = new int[len];

    int i;

    i = left;
    int j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            arr2[k++] = arr[i++];
        } else {
            arr2[k++] = arr[j++];
        }
    }
    int j1;
    if (i > mid) {
        for (j1 = j; j1 <= right; j1++) {
            arr2[k++] = arr[j1];
        }
    } else {
        for (j1 = i; j1 <= mid; j1++) {
            arr2[k++] = arr[j1];
        }
    }

    for (i = left; i <= right; i++) {
        arr[i] = arr2[i];
    }
    delete[] arr2;
}


void merge(int arr[], int left, int right, int len) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    merge(arr, left, mid, len);
    merge(arr, mid + 1, right, len);
    merge_sort(arr, left, mid, right, len);
}

//Partition with external array

int partition_ex(int arr[], int len) {
    int rand1 = rand() % len;
    swap(arr[0], arr[rand1]);
    int i = 0;
    int k = 0;
    int j = len - 1;
    auto arr2 = new int[len];

    for (i = 1; i < len; i++) {
        if (arr[i] < arr[0]) {
            arr2[k++] = arr[i];
        } else arr2[j--] = arr[i];
    }
    arr2[k] = arr[0];
    for (i = 0; i < len; i++) {
        arr[i] = arr2[i];
    }
    delete[] arr2;
    return k;
}

//Partition in place

 int partition_ip(int *arr, int left, int right) { // referred to page 4 of slide 6
     int pivot_chosen = (rand() % (right - left + 1)) + left;
     int len = right - left + 1;
     int i = left + 1;
     int j = right;
     while (1) {
         while (arr[i] < arr[pivot_chosen] && i < right) { i++; }
         while (arr[j] >= arr[pivot_chosen] && j > left) { j--; }
         if (i < j) { swap(arr[i], arr[j]); }
         else break;
     }
     swap(arr[left], arr[j]);
     return j;
 }

//int partition_ip(int arr[], int left, int right) { // referred to page 4 of slide 6
////    int pivot_chosen = (rand() % (right - left + 1)) + left;
//    int len = right - left + 1;
//    int i = left + 1;
//    int j = right-1;
////    swap(arr[pivot_chosen],arr[left]);
//    while (1) {
//        while (arr[i] < arr[left] && i < right) { i++; }
//        while (arr[j] >= arr[left] && j > left) { j--; }
//        if (i < j) { swap(arr[i], arr[j]); }
//        else break;
//    }
//    swap(arr[left], arr[j]);
//    return j;
//}

//void quick_extra(int arr[], int left, int right) {
//    int pivotat;
//    if (left >= right) {
//        return;
//    }
//    pivotat = partition_ex(arr, left, right);
//    swap(arr[left], arr[pivotat]);
//    quick_extra(arr, left, pivotat - 1);
//    quick_extra(arr+pivotat+1, pivotat + 1, right);
//}
void quick_extra(int arr[], int len) {
    if (len <= 1) return;
    int pivotat;
    pivotat = partition_ex(arr, len);
    quick_extra(arr, pivotat);
    quick_extra(arr + pivotat + 1, len - pivotat - 1);

}

void quick_inPlace(int arr[], int left, int right) {
    int i;
    int j = left, k = right;
    if (j < k) {
        for (i = j; i <= k - 1; i++) {
            if (arr[i] < arr[k]) {
                swap(arr[i], arr[j]);
                j++;
            }
        }
        swap(arr[j], arr[k]);
        quick_inPlace(arr, left, j - 1);
        quick_inPlace(arr, j + 1, k);
    } else return;
}

