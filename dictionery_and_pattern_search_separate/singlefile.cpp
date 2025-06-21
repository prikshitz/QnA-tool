#include <bits/stdc++.h>
using namespace std;

/*
 * Single-file implementation combining:
 *  - Node (for linked-list of search results)
 *  - SuffixTree (Ukkonen’s algorithm over printable ASCII)
 *  - SearchEngine (stores multiple suffix trees, one per sentence)
 *  - main() (example usage)
 */

// ─────────────────────────────────────────────────────────────────────────────
//                                  Node
// ─────────────────────────────────────────────────────────────────────────────
struct Node {
    Node* left;
    Node* right;
    int book_code;
    int page;
    int paragraph;
    int sentence_no;
    int offset;
    
    Node() {
        left = nullptr;
        right = nullptr;
        book_code = page = paragraph = sentence_no = offset = 0;
    }
    Node(int b_code, int pg, int para, int s_no, int off) {
        left = nullptr;
        right = nullptr;
        book_code = b_code;
        page = pg;
        paragraph = para;
        sentence_no = s_no;
        offset = off;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//                                SuffixTree
// ─────────────────────────────────────────────────────────────────────────────
// Working over printable ASCII (characters 32..126), total ALPHABET_SIZE = 95.
static const int ALPHABET_SIZE = 95;

struct Nodee {
    Nodee* children[ALPHABET_SIZE];
    int start;
    int* end;
    Nodee* suffixLink;
    int suffixIndex; // >=0 if this node is a leaf, stores the starting index of that suffix

    Nodee(int s, int* e) : start(s), end(e), suffixLink(nullptr), suffixIndex(-1) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            children[i] = nullptr;
        }
    }
    int edgeLength() const {
        return *end - start + 1;
    }
};

class SuffixTree {
private:
    string text;          // the input string + "$"
    Nodee* root;          // root of the suffix tree
    Nodee* activeNode;    // Ukkonen’s active point
    int activeEdge;       // index into text
    int activeLength;     // how many characters down from activeEdge
    int remainder;        // how many suffixes remain to be added in current phase
    int pos;              // current last index in text
    int leafEnd;          // shared end index for all leaf edges
    Nodee* lastCreatedInternalNode;

    // Map printable ASCII (32..126) to [0..94]
    inline int charIndexi(char c) {
        return c - 32;
    }
    inline char activeEdgeChar() {
        return text[activeEdge];
    }

    // Ukkonen’s extension for position i
    void extendTree(int i) {
        pos++;
        leafEnd = pos;
        remainder++;
        lastCreatedInternalNode = nullptr;

        while (remainder > 0) {
            if (activeLength == 0) {
                activeEdge = i;
            }
            int idx = charIndexi(activeEdgeChar());
            if (activeNode->children[idx] == nullptr) {
                // Create new leaf
                Nodee* leaf = new Nodee(i, &leafEnd);
                leaf->suffixIndex = i;
                activeNode->children[idx] = leaf;
                if (lastCreatedInternalNode) {
                    lastCreatedInternalNode->suffixLink = activeNode;
                    lastCreatedInternalNode = nullptr;
                }
            } else {
                Nodee* next = activeNode->children[idx];
                int edgeLen = next->edgeLength();
                if (activeLength >= edgeLen) {
                    activeEdge += edgeLen;
                    activeLength -= edgeLen;
                    activeNode = next;
                    continue;
                }
                if (text[next->start + activeLength] == text[i]) {
                    activeLength++;
                    if (lastCreatedInternalNode) {
                        lastCreatedInternalNode->suffixLink = activeNode;
                        lastCreatedInternalNode = nullptr;
                    }
                    break;
                }
                // Split edge
                int* splitEnd = new int(next->start + activeLength - 1);
                Nodee* split = new Nodee(next->start, splitEnd);
                activeNode->children[idx] = split;

                // New leaf from split
                Nodee* leaf = new Nodee(i, &leafEnd);
                leaf->suffixIndex = i;
                split->children[ charIndexi(text[i]) ] = leaf;

                // Remainder of old edge
                next->start += activeLength;
                split->children[ charIndexi(text[next->start]) ] = next;

                if (lastCreatedInternalNode) {
                    lastCreatedInternalNode->suffixLink = split;
                }
                lastCreatedInternalNode = split;
                split->suffixLink = root;
            }
            remainder--;
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = i - remainder + 1;
            } else if (activeNode != root) {
                activeNode = (activeNode->suffixLink) ? activeNode->suffixLink : root;
            }
        }
    }

    // Collect all leaf‐suffixIndices under 'node'
    void collectLeafIndices(Nodee* node, vector<int>& indices) {
        if (!node) return;
        if (node->suffixIndex >= 0) {
            indices.push_back(node->suffixIndex);
            return;
        }
        for (int c = 0; c < ALPHABET_SIZE; ++c) {
            if (node->children[c]) {
                collectLeafIndices(node->children[c], indices);
            }
        }
    }

public:
    // Build suffix tree for s + "$"
    SuffixTree(const string& s) : text(s + "$") {
        root = new Nodee(-1, new int(-1));
        activeNode = root;
        activeEdge = -1;
        activeLength = 0;
        remainder = 0;
        pos = -1;
        leafEnd = -1;
        lastCreatedInternalNode = nullptr;
        for (int i = 0; i < (int)text.size(); ++i) {
            extendTree(i);
        }
    }

    // Search for 'pattern'. Return a vector of starting indices of all matches.
    vector<int> searchAll(const string& pattern) {
        Nodee* current = root;
        int i = 0;
        while (i < (int)pattern.size()) {
            int idx = charIndexi(pattern[i]);
            if (!current->children[idx]) {
                return {};  // no match
            }
            Nodee* next = current->children[idx];
            int edgeLen = next->edgeLength();
            for (int j = 0; j < edgeLen && i < (int)pattern.size(); ++j, ++i) {
                if (text[next->start + j] != pattern[i]) {
                    return {};  // mismatch
                }
            }
            current = next;
        }
        vector<int> result;
        collectLeafIndices(current, result);
        return result;
    }

    ~SuffixTree() {
        // (Optional) Recursively delete all Nodee* to avoid memory leaks.
        // Omitted for brevity in this single-file example.
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//                              SearchEngine
// ─────────────────────────────────────────────────────────────────────────────
class SearchEngine {
private:
    // Each pair: (pointer to a suffix tree for one sentence, metadata)
    vector<pair<SuffixTree*, vector<int>>> book;

public:
    ~SearchEngine() {
        for (auto &pr : book) {
            delete pr.first;
        }
        book.clear();
    }

    // Build a suffix tree for this sentence and store metadata
    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
        SuffixTree* st = new SuffixTree(sentence);
        book.push_back({st, {book_code, page, paragraph, sentence_no}});
    }

    // Search across all stored sentences. Build a doubly-linked list of Node results.
    Node* search(string pattern, int& n_matches) {
        Node* dummy = new Node();
        Node* tail = dummy;
        n_matches = 0;
        for (auto &entry : book) {
            SuffixTree* st = entry.first;
            vector<int> offs = st->searchAll(pattern);
            for (int off : offs) {
                Node* x = new Node(
                    entry.second[0],
                    entry.second[1],
                    entry.second[2],
                    entry.second[3],
                    off
                );
                n_matches++;
                tail->right = x;
                x->left = tail;
                tail = x;
            }
        }
        Node* resultHead = dummy->right;
        if (resultHead) resultHead->left = nullptr;
        delete dummy;
        return resultHead;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//                                    main()
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    SearchEngine engine;
    // Example: insert a single sentence "abcabcabc"
    engine.insert_sentence(1, 1, 1, 1, "abcabcabc");

    int matchCount = 0;
    Node* head = engine.search("abc", matchCount);  // expecting offsets [0, 3, 6]

    cout << "Pattern: \"abc\" → " << matchCount << " match(es)\n";
    Node* cur = head;
    int safety = 0;
    while (cur && safety < 100) {
        safety++;
        cout << "  (Book " << cur->book_code
             << " | Page " << cur->page
             << " | Para " << cur->paragraph
             << " | Sent " << cur->sentence_no
             << " | Offset " << cur->offset
             << ")\n";
        cur = cur->right;
    }
    cout << "\n";

    return 0;
}
