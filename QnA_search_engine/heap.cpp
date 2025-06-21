
#include <assert.h>
#include <sstream>
#include <vector>
#include "heap.h"
using namespace std;

    heap::heap( vector<pair<double, string>>&arr2){
        arr = arr2;
        
        size = arr.size();
        buildheap();
    }
    bool heap:: compare(pair<double, string> p1, pair<double, string> p2){
        return p1.first<= p2.first;
    }
    void heap:: buildheap(){
        for(int i =(size-1)/2;i>=0; i--){
            downheap(i);
        }
    }
    int heap::parent(int index){
        if (index==0)return -1;
        return (index-1)/2;
    }
    int heap::left(int index){
        return 2* index +1;
    }
    int heap::right(int i){
        return 2*i+2;
    }
    void heap::downheap(int i){
        int leftc = left(i);
        int rightc = right(i);
        int smaller;
        while(rightc<size){
            if(compare(arr[leftc], arr[rightc])) smaller = leftc; else smaller = rightc;
            if(compare(arr[smaller], arr[i])){
                swap(arr[smaller], arr[i]);
                i = smaller;
                leftc = left(i);
                rightc = right(i);
            }else break;
        }
        if(leftc<size&&compare(arr[leftc], arr[i])){
            swap(arr[leftc], arr[i]);
        }
    }
    pair<double, string> heap::top(){
        if (size>0){
            return arr[0];
        }
        
    }
    pair<double, string> heap::extract(){
        if(size>0){
            pair<double, string> toreturn = arr[0];
            swap(arr[0], arr[size-1]);
            arr.pop_back();
            size--;
            downheap(0);
            return toreturn;
        }
        
    }
    void heap::heapup(int i){
        int par = parent(i);
        while(par>=0){
            if(compare(arr[i], arr[par])) {
                swap(arr[i], arr[par]);
                i = par;
                par = parent(i);
            }else break;

        }
    }
    void heap::insert(pair<double, string> p){
        arr.push_back(p);
        size++;
        heapup(size-1);
    }
