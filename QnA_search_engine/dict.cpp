// Do NOT add any other includes
#include "dict.h"

template <typename NodeType>


Dict<NodeType>::Dict() {
    root = new NodeType();
}

template <typename NodeType>
Dict<NodeType>::~Dict() {
    delete root;
}
template <typename NodeType>
void Dict<NodeType>::deletehepler(NodeType* root2){
    for(int i=0; i<36;i++){
        if(root2->child[i]){
            deletehepler(root2->child[i]);
        }
    }
    delete root2;
}

template <>
void Dict<MetaNode>::insert_sentence(Node* &node,  string sentence, int& sentence_no){
    
    int i = 0, n = sentence.size();
    
    while (i < n) {
        // Skip leading spaces
        while (i < n && !isalnum(sentence[i])) i++;

        int start = i;

        // Find end of word
        while (i < n && isalnum(sentence[i])) {
            char&c= sentence[i];
            if(c>='A'&&c<='Z'){
                c = c-'A'+'a';
            }
            i++;
        }
        if(start==i) {
            i++;
            continue;
        }
        if (start < i) {
            string word = sentence.substr(start, i - start);
            sentence_no++;
            MetaNode* root1 = root;
            for (char &ch : word) {
                int a = getindex(ch);
                if (root1->child[a]) {
                    root1 = root1->child[a];
                } else {
                    MetaNode* nn = new MetaNode();
                    root1->child[a] = nn;
                    root1 = root1->child[a];
                }
            }
            root1->count++;
            int x = root1->paras.size()-1;

            if(x>=0){
                if(root1->paras[x].first->paragraph!=node->paragraph||root1->paras[x].first->page!=node->page||root1->paras[x].first->book_code!=node->book_code){
                    
                    root1->paras.push_back({node,1});
                }else{
                    root1->paras[x].second++;
                }

            }else{
                root1->paras.push_back({node, 1});
            }
        }
    }
}
template <typename NodeType>
void Dict<NodeType>:: insert(string word){
    
    NodeType* root1 = root;
    for (char &ch : word) {
        if(ch>='A'&&ch<='Z'){
            ch = ch-'A'+'a';
        }
        int a = getindex(ch);
        if (root1->child[a]) {
            root1 = root1->child[a];
        } else {
            NodeType* x = new NodeType();
            root1->child[a] = x;
            root1 = root1->child[a];
        }
    }
    root1->count++;
}
template <typename NodeType>
void Dict<NodeType>:: insert(string word, double counts){
    
    NodeType* root1 = root;
    for (char &ch : word) {
        if(ch>='A'&&ch<='Z'){
            ch = ch-'A'+'a';
        }
        int a = getindex(ch);
        if (root1->child[a]) {
            root1 = root1->child[a];
        } else {
            NodeType* x = new NodeType();
            root1->child[a] = x;
            root1 = root1->child[a];
        }
    }
    root1->count += counts;
}
template <typename NodeType>
long long Dict<NodeType>::get_word_count(string word){
    
    NodeType* root1 = root;
    for (char &c : word) {
        if(c>='A'&&c<='Z'){
            c = c-'A'+'a';
        }
        int a = getindex(c);
        if (root1->child[a]) {
            root1 = root1->child[a];
        } else {
            return 0;
        }
    }
    return root1->count;
}
template<> 
vector<pair<Node*, int>>& Dict<MetaNode>:: getparas(string word){
    MetaNode* root1 = root;
    for (char &c : word) {
        if(c>='A'&&c<='Z'){
            c = c-'A'+'a';
        }
        int a = getindex(c);
        if (root1->child[a]) {
            root1 = root1->child[a];
        } else {
            static vector<pair<Node*, int>> p;
            return p;
        }
    }
    return root1->paras;
}
template <typename NodeType>
void Dict<NodeType>::dump_dictionary(string filename) {
    ofstream fout(filename);
    if (!fout.is_open()) return;

    string word = "";
    NodeType* root1 = root;
    helper(word, root1, fout);
    fout.close();
}

template <typename NodeType>
int Dict<NodeType>::getindex(char c){
    if ('a' <= c && c <= 'z') {
        return c - 'a';
    }
    else return c - '0' + 26;
}

template <typename NodeType>
char Dict<NodeType>::getchar(int & x){
    if (x >= 0 && x <= 25) {
        return 'a' + x;
    }
    else return '0' + (x - 26);
}

template <typename NodeType>
void Dict<NodeType>::helper(string &word, NodeType* root1, ofstream& fout) {
    for (int i = 0; i < 36; i++) {
        if (root1->child[i]) {
            word.push_back(getchar(i));
            if (root1->child[i]->count > 0) {
                fout << word << ", " << root1->child[i]->count << "\n";
            }
            helper(word, root1->child[i], fout);
            word.pop_back();
        }
    }
}

template class Dict<BasicNode>;
template class Dict<MetaNode>;