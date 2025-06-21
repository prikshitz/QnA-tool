// synttrie.cpp
#include "syntrie.h"
#include <cassert>

// ----------------- node ------------------

synttrie::node::node()
    : count(false)
{
    for (int i = 0; i < 26; ++i) child[i] = nullptr;
    syns = nullptr;    
}

synttrie::node::~node() {
    for (int i = 0; i < 26; ++i) {
        delete child[i];
    }
}

// ----------------- synttrie ------------------

synttrie::synttrie()
    : root(new node())
{}

synttrie::~synttrie() {
    delete root;
}

int synttrie::getIndex(char c) {
    if (c >= 'a' && c <= 'z')return c - 'a';
    else {
        assert(false && "Invalid character for trie");
        return -1;
    }
}

char synttrie::getChar(int idx) {
    if (idx >= 0 && idx < 26)
        return 'a' + idx;
    else {
        assert(false && "Invalid index for reverse lookup");
        return '?';
    }
}

void synttrie::insert( std::vector<std::string>& group) {
    // Allocate a new group and store pointer
    std::vector<std::string>* shared = new std::vector<std::string>(group);
    syn_storage.push_back(shared);  // for lifetime management

    for (const std::string &w : group) {
        node* cur = root;
        for (char ch : w) {
            char c = (ch >= 'A' && ch <= 'Z') ? ch + 'a' - 'A' : ch;
            int idx = getIndex(c);
            if (!cur->child[idx])
                cur->child[idx] = new node();
            cur = cur->child[idx];
        }

        // Only assign if not already assigned
        if (!cur->count) {
            cur->syns = shared;  // shared pointer to same vector
            cur->count = true;
        }
    }
}

std::vector<std::string> *synttrie::search( std::string& word) const {
    node* cur = root;
    for (char &ch : word) {
        if(ch>='A'&&ch<='Z') ch += ('a' - 'A');
        int idx = getIndex(ch);
        if (!cur->child[idx])
            return nullptr;
        cur = cur->child[idx];
    }
    return (cur->syns);
}
