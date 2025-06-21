// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){
    // Implement your function here    
    root = new node();
}

Dict::~Dict(){
    // Implement your function here  
    delete root;  
}

int Dict::getindex(char&c){
    if(c>='a'&&c<='z') return c-'a';
    else return c-'0'+26;
}

char Dict::getchar(int &x){
    if(x>=0&&x<=25){
        return 'a' + x;
    }
    else return '0'+(x - 26);
}


void Dict::insert(string word) {
    node*root1 =root;
    for(char &ch: word){
        if(ch>='A'&&ch<='Z'){
            ch = ch-'A'+'a';
        }
        int a = getindex(ch);
        if (root1->child[a]){
            root1 = root1->child[a];
        }else{
            node* x = new node();
            root1->child[a] = x;
            // x->c = ch;
            root1 = root1->child[a];
        }
    }
    root1->count++;
}


void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    
    int i = 0, n = sentence.size();

    while (i < n) {
       
        while (i < n && !isalnum(sentence[i])) i++;

        int start = i;
        while (i < n && isalnum(sentence[i])) i++;

        if (start < i) {
            string word = sentence.substr(start, i - start);
            insert(word);
        }else{i++;}
    }
}


int Dict::get_word_count(string word){
    node*root1 = root;
    for(char &c:word){
        if(c>='A'&&c<='Z'){
            c = c-'A'+'a';
        }
        int a = getindex(c);
        if(root1->child[a]){
            root1 = root1->child[a];
        }else{
            return 0;
        }
    }
    return root1->count;
    
}

void Dict::dump_dictionary(string filename) {
    ofstream fout(filename);
    if (!fout.is_open()) return;
    
    string word = "";
    node* root1 = root;
    helper(word, root1, fout);
    fout.close();
}

void Dict::helper( string &word, node* root1, ofstream& fout){
    for(int i =0; i<36; i++){
        if(root1->child[i]){
            word.push_back(getchar(i));
            if(root1->child[i]->count>0){
                fout<<word<<", "<<root1->child[i]->count<<"\n";
            }
            helper(word, root1->child[i], fout);
            word.pop_back();
        }
    }
}
