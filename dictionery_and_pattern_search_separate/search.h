// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

const int ALPHABET_SIZE = 95;


struct Nodee {
    Nodee* children[ALPHABET_SIZE]; 
    int start; 
    int* end; 
    Nodee* suffixLink;
    int index;
    bool isleaf=true;
    
    Nodee(int s, int* e, int in) : start(s), end(e), index(in), suffixLink(nullptr) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) children[i] = nullptr;
    }
    
    int edgeLength() const {
        return *end - start + 1;
    }
    
};

class SuffixTree { 
    private: 
    string text; 
    Nodee* root; 
    Nodee* activeNode; 
    int activeEdge = -1, activeLength = 0; 
    int remainder = 0; 
    int pos = -1; 
    int leafEnd = -1; 
    Nodee* lastCreatedInternalNode = nullptr;
    
    char activeEdgeChar() {
        return text[activeEdge];
    }
    
    int charIndex(char c) { return c- 32; }
    void extendTree(int i) {
        pos++;
        leafEnd = pos;
        remainder++;
        lastCreatedInternalNode = nullptr;
        
        while (remainder > 0) {
            if (activeLength == 0)
            activeEdge = i;
            
            int idx = charIndex(activeEdgeChar());
            if (activeNode->children[idx] == nullptr) {
                activeNode->children[idx] = new Nodee(i, &leafEnd, i-remainder+1);
                activeNode->isleaf = false;
                
                if (lastCreatedInternalNode) {
                    lastCreatedInternalNode->suffixLink = activeNode;
                    lastCreatedInternalNode = nullptr;
                }
            } else {
                Nodee* next = activeNode->children[idx];
                int edgeLen = next->edgeLength();
                if (activeLength >= edgeLen) {
                    activeEdge += edgeLen;
                    activeLength -= edgeLen;
                    activeNode = next;
                    continue;
                }
                
                if (text[next->start + activeLength] == text[i]) {
                    activeLength++;
                    if (lastCreatedInternalNode) {
                        lastCreatedInternalNode->suffixLink = activeNode;
                        lastCreatedInternalNode = nullptr;
                    }
                    break;
                }
                
                int* splitEnd = new int(next->start + activeLength - 1);
                Nodee* split = new Nodee(next->start, splitEnd, -1);
                activeNode->children[idx] = split;
                
                split->children[charIndex(text[i])] = new Nodee(i, &leafEnd, i-remainder+1);
                next->start += activeLength;
                split->children[charIndex(text[next->start])] = next;
                split->isleaf = false;
                
                if (lastCreatedInternalNode)
                lastCreatedInternalNode->suffixLink = split;
                
                lastCreatedInternalNode = split;
                split->suffixLink = root;
            }
            
            remainder--;
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = i - remainder + 1;
            } else if (activeNode != root) {
                activeNode = activeNode->suffixLink ? activeNode->suffixLink : root;
            }
        }
    }
    
    void collectLeafIndices(Nodee* node, vector<int>& indices) {
        
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i]) {
                collectLeafIndices(node->children[i], indices);
            }
        }
        if (node->isleaf)
        indices.push_back(node->index);
    }
    
    public: SuffixTree(const string& s) : text(s + "$") { 
        root = new Nodee(-1, new int(-1), -1); 
        activeNode = root;
        
        for (int i = 0; i < text.size(); i++) {
            if(text[i]>='A'&&text[i]<='Z'){
                text[i] = text[i]-'A'+'a';
            }
            extendTree(i);

        }
    }
    
vector<int> searchAll(const string& pattern) {
    Nodee* current = root;
    int i = 0;

    while (i < pattern.size()) {
        int idx = charIndex(pattern[i]);
        if (!current->children[idx])
            return {};  // Not found

        Nodee* next = current->children[idx];
        int edgeLen = next->edgeLength();

        for (int j = 0; j < edgeLen && i < pattern.size(); ++j, ++i) {
            if (text[next->start + j] != pattern[i])
                return {}; // Mismatch
        }

        current = next;
    }

    vector<int> result;
    collectLeafIndices(current, result);
    return result;
}

};



class SearchEngine {

private:
    vector<pair<SuffixTree*, vector<int>>> alltrres;
    // You can add attributes/helper functions here

public:
    
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};