#pragma once
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class heap {
public:
    int size;
    vector<pair<double, string>> arr;

    heap(vector<pair<double, string>>& arr2);

    bool compare(pair<double, string> p1, pair<double, string> p2);
    void buildheap();
    int parent(int index);
    int left(int index);
    int right(int index);
    void downheap(int i);
    pair<double, string> top();
    pair<double, string> extract();
    void heapup(int i);
    void insert(pair<double, string> p);
};