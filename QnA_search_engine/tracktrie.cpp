
#include "tracktrie.h"

using namespace std;


// Constructor for Trie
TrackTrie::TrackTrie() {
    root = new node();
}

// Destructor for Trie
TrackTrie::~TrackTrie() {
    delete root;
}

// Helper to delete nodes
void TrackTrie::deletehelper(node* root1) {
    if (!root1) return;
    for (int i = 0; i < 36; i++) {
        if (root1->child[i]) {
            deletehelper(root1->child[i]);
        }
    }
    delete root1;
}

// Get index of character
int TrackTrie::getindex(char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a';
    }
    return c - '0' + 26;
}

// Get character from index
char TrackTrie::getchar(int& x) {
    if (x >= 0 && x <= 25) {
        return 'a' + x;
    }
    return '0' + (x - 26);
}

// Insert with book/page/para logic
void TrackTrie::insert(string word, int book, int page, int para) {
    node* root1 = root;
    for (char& ch : word) {
        int a = getindex(ch);
        if (root1->child[a]) {
            root1 = root1->child[a];
        } else {
            node* x = new node();
            root1->child[a] = x;
            root1 = root1->child[a];
        }
    }
    if (root1->lastparagraph != para || root1->lastpage != page || root1->lastbook != book) {
        root1->count++;
        root1->lastbook = book;
        root1->lastpage = page;
        root1->lastparagraph = para;
    }
}

int TrackTrie::get_word_count(string word) {
    for(char &c: word){
        if(c>='A'&&c<='Z'){
            c = c-'A'+'a';
        }
    }
    node* root1 = root;
    for (char& c : word) {
        int a = getindex(c);
        if (root1->child[a]) {
            root1 = root1->child[a];
        } else {
            return 0;
        }
    }
    return root1->count;
}

void TrackTrie::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    
    int i = 0, n = sentence.size();

    while (i < n) {
        // Skip leading spaces
        while (i < n && !isalnum(sentence[i])) i++;

        int start = i;

        // Find end of word
        while (i < n && isalnum(sentence[i])) i++;
        if(start==i) {
            i++;
            continue;
        }
        if (start < i) {
            string word = sentence.substr(start, i - start);
            
            for(char &c: word){
                if(c>='A'&&c<='Z'){
                    c = c-'A'+'a';
                }
            }
            insert(word, book_code, page, paragraph);
        }
    }
}
