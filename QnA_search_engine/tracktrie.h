#include <string>
#include <fstream>
using namespace std;

class TrackTrie {
public:
    struct node {
        node* child[36];
        double count = 0;
        int lastbook = -1;
        int lastpage = -1;
        int lastparagraph = -1;

        node(){for (int i = 0; i < 36; i++) child[i] = nullptr;}
        ~node(){for (int i = 0; i < 36; i++) if(child[i])  delete(child[i]);}
    };

    TrackTrie();
    ~TrackTrie();

    
    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    
    
    int get_word_count(std::string word);
    
    
    private:
    void insert(std::string word, int book, int page, int para);
    node* root;

    int getindex(char c);
    char getchar(int& x);
    void deletehelper(node* root1);
};


