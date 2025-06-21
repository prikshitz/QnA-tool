#pragma once
#include <vector>
#include <string>

class synttrie {
public:
    // Builds an empty trie
    synttrie();

    // Cleans up all allocated nodes
    ~synttrie();

    // Insert `word` with its synonyms `v` into the trie
    void insert( std::vector<std::string>& v);

    // Return the synonym list for `word`, or empty vector if not found
    std::vector<std::string> *search( std::string& word) const;

    std::vector<std::vector<std::string>*> syn_storage;
private:
    struct node {
        node* child[26];
        bool count;
        std::vector<std::string>*syns = nullptr;

        node();              // ctor: zeroes out `child[]`
        ~node();             // dtor: recursively deletes children
    };

    node* root;

    // Map 'a'–'z'→0–25, 
    static int  getIndex(char c);
    // Reverse of getIndex (if needed)
    static char getChar(int idx);
};
 // SYNTRIE_H
