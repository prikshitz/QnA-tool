#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "heap.h"
// #include "syntrie.h"
// #include "tracktrie.h"

using namespace std;





class QNA_tool {
    int  thisparawords=0;
    double paracount = 0;
    Dict<MetaNode>* d;                           //stores number of all book's words together
    
    Dict<BasicNode>* unigramfreq;                  // stores count of word in unigramfreq.txt file
    Dict<BasicNode>* frequentwords;                // stores common used words which do not convey meaning
    // synttrie* synonym;
    // TrackTrie * numberofdoc;
    double average_para_length;
    Node* x;
    // vector<pair<Node*,Dict*>> paras;

    

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor
    
    void help(heap* &hi, BasicNode* root1, string &s, int&k);
    char getchr(int i);
    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    Node* get_top_k_para_tfidf(string question, int k);
    Node* get_top_k_para_bm25(string questio, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};