#include <iostream>
#include <ctime>

using namespace std;

int R_Select(int A[], int n, int i);

int D_Select(int A[], int n, int i);

void insertion(int arr[], int length);

int partition_ip(int A[], int n, int pivotat);

int main() {
    srand((unsigned) time(NULL));

    int sort_num, len, pos;
    double dur;
    clock_t start, end;
    cin >> sort_num;
    cin >> len;
    cin >> pos;
    auto *arr = new int[len];
    int i;
    for (i = 0; i < len; i++) {
        cin >> arr[i];
    }
    int ans;
    start = clock();

    if (sort_num == 0) {
        ans = R_Select(arr, len, pos);
    } else {
        ans = D_Select(arr, len, pos);
    }
    end = clock();

//    cout << "The order-" << pos << " item is " << ans << endl;
    dur = (double) end - start;
    cout << dur / CLOCKS_PER_SEC << endl;
    delete[] arr;
    return 0;

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


int partition_ip(int A[], int n, int pivotat_pos) {
    swap(A[0], A[pivotat_pos]);
    int i = 1;
    int j = n - 1;
    while (1) {
        while (i < n - 1 && A[i] < A[0]) {
            i++;
        }
        while (j > 0 && A[j] >= A[0]) {
            j--;
        }
        if (i < j) {
            swap(A[i], A[j]);
        } else break;
    }
    swap(A[0], A[j]);
    return j;
}

int R_Select(int A[], int n, int i) {
    if (n <= 1) return A[0];
    int pivotat_pos = rand() % n;
    int pivotat = A[pivotat_pos];
    int j = partition_ip(A, n, pivotat_pos);
    if (j == i) return pivotat;
    else if (j > i) return R_Select(A, j, i);
    else return R_Select(A + j + 1, n - j - 1, i - j - 1);
}

int D_Select(int A[], int n, int i) {
    if (n <= 1) return A[0];
    int groupnum = n / 5;
    int k;
    auto *A2 = new int[groupnum + 1];
    for (k = 0; k < groupnum; k++) {
        insertion(A + 5 * k, 5);
        A2[k] = A[5 * k + 2];
    }
    if (groupnum * 5 != n) {
        insertion(A + 5 * groupnum, n - 5 * groupnum);
        A2[groupnum] = A[5 * groupnum + (n - 5 * groupnum) / 2];
    }
    int p = D_Select(A2, n / 5, i / 10);
    int kk, pivpos=0;
    for (kk = 0; kk < n; kk++) {
        if (A[kk] == p) {
            pivpos = kk;
            break;
        }
    }
    int j = partition_ip(A, n, pivpos);
    delete[] A2;
    if (j == i) return p;
    else if (j > i) return D_Select(A, j, i);
    else return D_Select(A + j + 1, n - j - 1, i - j - 1);
}

