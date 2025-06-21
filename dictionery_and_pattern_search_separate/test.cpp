// tester.cpp

#include <bits/stdc++.h>
#include "search.h"
using namespace std;

/*
  Expected line format in mahatma-gandhi-collected-works-volume-86.txt:

  ('86', 0, 2, 3, 72) When Dr. Kumarappa has given such an excellent introduction, what more is there for me to say?
  ^^book^^ ^page^ ‾paragraph‾ ‾sent_no‾ ‾offset‾  ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ text...
  
  We need to parse out:
    book_code    = 86        (as int)
    page         = 0         (int)
    paragraph    = 2         (int)
    sentence_no  = 3         (int)
    offset       = 72        (int)
    sentence_txt = "When Dr. Kumarappa ... say?"

  Then call:
    engine.insert_sentence(book_code, page, paragraph, sentence_no, sentence_txt);

  Finally, we accept arbitrary pattern queries from stdin and print matches.
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SearchEngine engine;

    // 1) Open and read the full-text file:
    const char* filename = "mahatma-gandhi-collected-works-volume-86.txt";
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: Could not open " << filename << "\n";
        return 1;
    }

    string line;
    int i=0;
    while (std::getline(infile, line)) {
        // Skip empty lines
        cout<<"line no: "<<i<<"\n";
        i++;
        if (line.empty()) continue;
        // We expect something like:
        // ('86', 0, 2, 3, 72) This is the sentence text...
        // First, find the closing parenthesis ')'
        size_t closeParen = line.find(")");
        if (closeParen == string::npos) {
            // Malformed line—skip
            continue;
        }

        // Extract the tuple substring (including parentheses):
        //    tuplePart = "('86', 0, 2, 3, 72"
        string tuplePart = line.substr(1, closeParen - 2); 
        // Explanation: 
        //   line[0] == '(' 
        //   line[closeParen] == ')'
        // We want what's inside the parentheses, without the outer quotes around '86'.
        // Actually, because there's a leading single‐quote on '86', we can do a safer approach:
        //   1) drop the very first '(' and the very last ')' 
        //   2) then split by commas.

        // Let's do it step by step more robustly:
        {
            // Re‐parse so we correctly strip the outermost "(" and ")"
            size_t openParen = line.find("(");
            // substring between openParen+1 and closeParen-1
            tuplePart = line.substr(openParen + 1, closeParen - (openParen + 1));
        }

        // Now tuplePart should look like:
        //    "'86', 0, 2, 3, 72"
        // We'll split on commas, remembering that the first field is "'86'"
        vector<string> fields;
        {
            stringstream ss(tuplePart);
            string token;
            while (std::getline(ss, token, ',')) {
                // Trim whitespace from token
                size_t start = 0;
                while (start < token.size() && isspace((unsigned char)token[start])) start++;
                size_t end = token.size();
                while (end > start && isspace((unsigned char)token[end - 1])) end--;
                fields.push_back(token.substr(start, end - start));
                // cout<<" sss";
            }
        }

        if (fields.size() != 5) {
            // Malformed metadata—skip
            continue;
        }

        // fields[0] == "'86'"  (including single quotes)
        // fields[1] == "0"
        // fields[2] == "2"
        // fields[3] == "3"
        // fields[4] == "72"
        //
        // Parse out integers:

        // 1) Parse book_code: strip the single quotes
        string bookStr = fields[0];
        if (bookStr.front() == '\'' && bookStr.back() == '\'') {
            bookStr = bookStr.substr(1, bookStr.size() - 2);
        }
        int book_code    = stoi(bookStr);
        int page         = stoi(fields[1]);
        int paragraph    = stoi(fields[2]);
        int sentence_no  = stoi(fields[3]);
        long long offset       = stoll(fields[4]);

        // 2) The actual sentence text begins right after ") ":
        string sentence_txt;
        if (closeParen + 2 <= line.size()) {
            sentence_txt = line.substr(closeParen + 2);
        } else {
            sentence_txt = "";
        }

        // Insert into the SearchEngine:
        engine.insert_sentence(book_code, page, paragraph, sentence_no, sentence_txt);
        
        
    }
    
    infile.close();
    cout << "Finished inserting all lines from\n ";
    // cout << "Total sentences indexed: " << engine.get_total_sentences() << "\n\n";
    // (Assuming you have a helper get_total_sentences() or similar; if not, just omit that line.)

    // 2) Now accept pattern queries from stdin until EOF. For each pattern, we run engine.search(...)
    //    and print matches exactly as before.

    cout << "Enter search patterns (one per line). Press Ctrl+D (or Ctrl+Z + Enter on Windows) to stop.\n";
    string pattern;
    while (true) {
        if (!std::getline(cin, pattern))
            break;                // EOF reached
        if (pattern.empty()) {
            cout << "[Empty pattern—skipped]\n";
            continue;
        }

        int n_matches = 0;
        Node* head = engine.search(pattern, n_matches);

        cout << "Pattern: \"" << pattern << "\" → " << n_matches << " match(es)\n";
        Node* cur = head;
        while (cur) {
            cout << "  (Book " << cur->book_code
                 << " | Page " << cur->page
                 << " | Para " << cur->paragraph
                 << " | Sent " << cur->sentence_no
                 << " | Offset " << cur->offset
                 << ")\n";
            cur = cur->right;
        }
        cout << "\n";
    }

    cout << "Exiting.\n";
    return 0;
}
