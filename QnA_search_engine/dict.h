// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
#include "Node.h"


using namespace std;

struct BasicNode {
    BasicNode* child[36];
    double count = 0;

    BasicNode() {
        for (int i = 0; i < 36; i++) child[i] = nullptr;
    }

    ~BasicNode() {
        for (int i = 0; i < 36; i++) {
            if (child[i]) delete child[i];
        }
    }
};

struct MetaNode {
    MetaNode* child[36];
    double count = 0;
    vector<pair<Node*, int>> paras;

    MetaNode() {
        for (int i = 0; i < 36; i++) child[i] = nullptr;
    }

    ~MetaNode() {
        for (int i = 0; i < 36; i++) {
            if (child[i]) {
                delete child[i];
            }
        }
    }
};
template <typename NodeType>

class Dict {
public:

    // You can add attributes/helper functions here
    

    NodeType* root ;
    void insert(string word);
    void insert(string word, double counts);
    void deletehepler(NodeType* root2);
    int getindex(char c);
    char getchar(int &x);
    void helper(string &word, NodeType* root1, ofstream& fout);
    vector<pair<Node* , int>>& getparas(string word);
    
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();
    
    ~Dict();
    
    void insert_sentence(Node* &x,  string sentence, int &sentence_no);
    
    long long get_word_count(string word);
    
    void dump_dictionary(string filename);
    
    /* -----------------------------------------*/
};

template<>
vector<pair<Node*, int>>& Dict<MetaNode>::getparas(string word);
template<>
void Dict<MetaNode>:: insert_sentence(Node* &x,  string sentence, int &sentence_no);