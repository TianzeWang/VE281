//
// Created by 大泽 on 2017/9/19.
//

#ifndef PROJECT1_SORT_H
#define PROJECT1_SORT_H

void bubble(int arr[], int length);

void insertion(int arr[], int length);

void selection(int arr[], int length);

void merge(int arr[], int left, int right, int len);

void merge_sort(int arr[], int left, int mid, int right, int len);

int partition_ex(int arr[], int left, int right);

void quick_extra(int arr[], int len);

//int partition_ip(int arr[], int left, int right);

void quick_inPlace(int arr[], int left, int right);
#endif //PROJECT1_SORT_H
