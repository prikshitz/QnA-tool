#include <assert.h>
#include <sstream>
#include "qna_tool.h"
#include <math.h>


using namespace std;


QNA_tool::QNA_tool(){
    // Implement your function here 
    d = new Dict<MetaNode>();             // stores number of all book's words together

    frequentwords = new Dict<BasicNode>(); // stores common used words which do not convey meaning

    average_para_length = 0.0;  

    unigramfreq = new Dict<BasicNode>();   // stores count of word in unigramfreq.txt file

    // stores count of paras for a word (number of unique paras a word is in)
    x = new Node(-1, -1, -1, -1, -1);

    ifstream file("unigram_freq.csv");
    string line;
    getline(file, line);
    while(getline(file, line)){
        string word;
        double count;
        int length =0;
        while(line[length]!=',') length++;
        word = line.substr(0, length);
        count = stod(line.substr(length+1, line.size()-length-1));
        unigramfreq->insert(word, count);
    }
    vector<string> commonwords = {
    "the", "of", "and", "to", "a", "in", "for", "is", "on", "that", "by", "this", "with", 
    "i", "you", "it", "not", "or", "be", "are", "from", "at", "as", "all", "have", "more", 
    "an", "was", "we", "will","but", "if", "about", "can", "would", "there", "their", "what", "so", "no", "up", "out",
     "get", "which", "when", "had", "do", "been", "may", "my", "your", "them", "me",
    "into", "than", "some", "could", "how", "then", "its", "also", "only", "now", "such",
    "just", "over", "like", "did", "he", "she", "they", "because", "too", "while", "where",
    "most", "even", "back", "any", "our", "us", "were", "s"};
    for(string & s: commonwords){
        frequentwords->insert(s);
    }
    // synonym = new synttrie();
    // std::ifstream inFile2("synonyms.txt");
    // if (!inFile2.is_open()) {
    //     std::cerr << "Failed to open input file." << std::endl;
    //     return ;
    // }
    // // string line;
    // while (std::getline(inFile2, line)) {
    //     if (line.empty()) {
    //         continue;
    //     }
    //     vector<string> words;
    //     string word;
    //     int i=0, n = line.size();
    //     int start =0;

    //     while(i<n){
    //         while(i<n&&line[i]!=','){
    //             if(line[i]<='Z'&&line[i]>='A'){
    //                 line[i] = line[i] - 'A' + 'a';
    //             }
    //             i++;
    //         }
    //         word = line.substr(start,i-start);
    //         words.push_back(word);
    //         start = i+1;
    //         i++;
    //     }
    //     if(words.size()<=1) continue;
    //     synonym->insert(words);
    // }

}

QNA_tool::~QNA_tool(){
    // Implement your function here  
    delete d;
    delete unigramfreq;
    delete frequentwords;

    
    
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    sentence_no = 0;
    // cout<<book_code<<" "<<page<<" "<<paragraph<<"\n";
    if(x->paragraph!=paragraph||x->page!=page||x->book_code!=book_code){
        x->sentence_no = thisparawords;
        x = new Node(book_code, page, paragraph, 0, 0);
        d->insert_sentence(x, sentence, sentence_no);
        paracount++;
        average_para_length = (average_para_length*(paracount-1.0) + thisparawords)/paracount;
        
        thisparawords = 0;
    }else{
        d->insert_sentence(x, sentence, sentence_no);
    }
    thisparawords+= sentence_no;

    return;
}

Node* QNA_tool::get_top_k_para_tfidf(string question, int k){
    int i =0; int n = question.size();
    stringstream ss;
    Dict<BasicNode>* queryparas = new Dict<BasicNode>();
    
    while(i<n){
        while(i<n&&!isalnum(question[i])) i++;
        int start = i;
        while(i<n&& isalnum(question[i]))i++;
        string word = question.substr(start, i-start);
        vector<pair<Node*, int>>& paras = d->getparas(word);   

        for(pair<Node*, int>& para: paras){
            if(para.first->sentence_no<200) para.first->sentence_no = 200;
            double termfreq = para.second;
            if(termfreq>0){
                termfreq /= para.first->sentence_no;
                ss.str(""); ss.clear();
                ss << para.first;
                string s = ss.str();
                int document_containg_term_word = paras.size();
                double idf = log10(paracount/(1.0+document_containg_term_word));
                double parascore = termfreq*idf;
                queryparas->insert(s, parascore);
            }
        }

    }
    // cout<<"worked on query paras and inserted paras address in dict named queryparas with their score\n";
    vector<pair<double, string>> v;
    heap* hip = new heap(v);
    string s= "";
    help(hip, queryparas->root, s, k);
    // cout<<"worked on heap and collected top k para node addresses as string with thier score in hip\n";
    Node* tail = new Node();
    Node* tail2 = tail;
    while(hip->size>0){
        
        s = hip->top().second;
        
        unsigned long long ptr_val;
        stringstream ss2(s);
        ss2 >>std::hex>> ptr_val;
        Node* some = (Node*)(ptr_val);
        
        some->right = tail;
        tail->left = some;
        tail = some;
        hip->extract();
        
    }
    tail2->left->right = nullptr;
    return tail;


}

Node* QNA_tool::get_top_k_para_bm25(string question, int k){
    int i =0; int n = question.size();
    stringstream ss;
    Dict<BasicNode>* queryparas = new Dict<BasicNode>();
    
    while(i<n){
        while(i<n&&!isalnum(question[i])) i++;
        int start = i;
        while(i<n&& isalnum(question[i]))i++;
        string word = question.substr(start, i-start);
        vector<pair<Node*, int>> paras = d->getparas(word);        
        
        for(pair<Node*, int>& para: paras){
            double k2 = 2.5; // 2.2 to 3
            int termfreq = para.second;
            double b = 0.75;
            if(termfreq>0){
                termfreq = termfreq*k2/(termfreq+(k2-1)*(1-b+b*(para.first->sentence_no/average_para_length))) ;
                ss.str(""); ss.clear();
                ss << para.first;
                string s = ss.str();
                int document_containg_term_word = paras.size();
                double idf = log10((paracount-document_containg_term_word+0.5)/(0.5+document_containg_term_word) + 1);
                double parascore = termfreq*idf;
                queryparas->insert(s, parascore);
            }
        }

    }
    // cout<<"worked on query paras and inserted paras address in dict named queryparas with their score\n";
    vector<pair<double, string>> v;
    heap* hip = new heap(v);
    string s= "";
    help(hip, queryparas->root, s, k);
    // cout<<"worked on heap and collected top k para node addresses as string with thier score in hip\n";
    Node* tail = new Node();
    Node* tail2 = tail;
    while(hip->size>0){
        
        s = hip->top().second;
        
        unsigned long long ptr_val;
        stringstream ss2(s);
        ss2 >>std::hex>> ptr_val;
        Node* some = (Node*)(ptr_val);
        
        some->right = tail;
        tail->left = some;
        tail = some;
        hip->extract();
        
    }
    tail2->left->right = nullptr;
    return tail;


}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    int i =0; int n = question.size();
    stringstream ss;
    Dict<BasicNode>* queryparas = new Dict<BasicNode>();
    while(i<n){
        while(i<n&&!isalnum(question[i])) i++;
        int start = i;
        while(i<n&& isalnum(question[i]))i++;
        if(start==i){
            i++;
            continue;
        }
        string word = question.substr(start, i-start);

        double wordscore = (d->get_word_count(word)+1.0)/(unigramfreq->get_word_count(word) + 1.0);
        vector<pair<Node*, int>> paras = d->getparas(word); 
        for(pair<Node*,int> &para: paras){            
                ss.str(""); ss.clear();

                ss << para.first;
                string s = ss.str();
                double parascore = wordscore*para.second;
                queryparas->insert(s, parascore);   
        }
    }
    cout<<"worked on query paras and inserted paras address in dict named queryparas with their score\n";
    vector<pair<double, string>> v;
    heap* hip = new heap(v);
    string s= "";
    help(hip, queryparas->root, s, k);
    cout<<"worked on heap and collected top k para node addresses as string with thier score in hip\n";
    Node* tail = new Node();
    Node* tail2 = tail;
    while(hip->size>0){
        
        s = hip->top().second;
        
        unsigned long long ptr_val;
        stringstream ss2(s);
        ss2 >>std::hex>> ptr_val;
        Node* some = (Node*)(ptr_val);
        
        some->right = tail;
        tail->left = some;
        tail = some;
        hip->extract();
        
    }
    tail2->left->right = nullptr;
    return tail;
}

void QNA_tool::help(heap* &hi, BasicNode* root1, string &s, int&k){
    
    for(int i =0; i<36; i++){
        if(root1->child[i]){
            s.push_back(getchr(i));
            
            if(root1->child[i]->count>0){
                
                hi->insert({root1->child[i]->count, s});
                
                if(hi->size>k){
                    
                    hi->extract();
                }
            }
            help(hi, root1->child[i], s, k);
            s.pop_back();
        }
    }
    
}

char QNA_tool::getchr(int i){
    if(i<26){
        return 'a' + i;
    }
    else return '0'+(i - 26);
}


void QNA_tool::query(string question, string filename){
    // Implement your function here  
    string dd = "";
    int i =0; int n = question.size();
    while(i<n){
        while(i<n&&!isalnum(question[i])) i++;
        int start = i;
        while(i<n&& isalnum(question[i])){
            i++;

        }
        if(start==i) {i++;continue;}
        string word = question.substr(start, i-start);
        if(frequentwords->get_word_count(word)) continue;
        // vector<string> *syn = synonym->search(word);
        // if(syn!=nullptr && syn->size()>1){
            
        //     for(string othersyns: *syn){
                
        //         x.append(othersyns);
        //         x.push_back(' ');
        //     }
        // }else{
           
        //     x.append(word);
        //     x.push_back(' ');
        // }

        dd.append(word);
        dd.push_back(' ');
    }
    
    int k = 3;
    cout<<"question i  searched for:"<<dd<<"\n";

    Node * head = get_top_k_para_bm25(dd, k);
    
    
    
    std::cout << "Q: " << question << std::endl;
    query_llm("api_call.py", head, k, question);
    

    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "corpus/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;
    
    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }
    
    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python ";
    command += filename;           // api_call.py
    command += " ";
    command += to_string(k);       // num paragraphs
    command += " ";
    command += "query.txt";        // the query file
    system(command.c_str());
    return;
}