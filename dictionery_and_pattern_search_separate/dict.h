// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Dict {
public:
    struct node{
        // char c;
        node* child[36];
        int count =0;
        node() {
            for (int i = 0; i < 36; i++) child[i] = nullptr;
        }
        ~node() {
            for (int i = 0; i < 36; i++) if(child[i])  delete child[i];
        }
    };

node* root ;

int getindex(char&c);
char getchar(int &x);
void insert(string word);
void helper( string &word, node* root1, ofstream& fout);

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */
    // You can add attributes/helper functions here

       

    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};