// Do NOT add any other includes
#include "search.h"

SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    // Implement your function here 
    for (auto &pr : alltrres) {
        delete pr.first;
    }
    alltrres.clear(); 
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    SuffixTree* s = new SuffixTree(sentence);
    
    alltrres.push_back({s, {book_code, page, paragraph, sentence_no}});
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here  
    Node* dummy = new Node();      // dummy head
    Node* tail = dummy;
    n_matches = 0;

    for (auto &entry : alltrres) {
        SuffixTree* st = entry.first;
        vector<int> offs = st->searchAll(pattern);
        n_matches+= offs.size();
        for (int off : offs) {
            Node* x = new Node(
                entry.second[0],
                entry.second[1],
                entry.second[2],
                entry.second[3],
                off
            );
            tail->right = x;
            x->left = tail;
            tail = x;
        }
    }
    // Skip dummy
    Node* resultHead = dummy->right;
    if (resultHead) {
        resultHead->left = nullptr;
    }
    delete dummy;
    return resultHead;
    
}



