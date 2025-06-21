#include <iostream>
#include "search.h"
// #include "Node.h"
using namespace std;

int main() {
    SearchEngine engine;
    // Insert a sample sentence
    engine.insert_sentence(1, 1, 1, 1, "abcabcabcd abc abc dkvje.kvbjekb abckjenn");

    int matchCount = 0;
    Node* head = engine.search("abc", matchCount);  // Expect offsets [0,3,6]
    cout << "Pattern: \"abc\"" << matchCount << " match(es)\n";
    Node* cur = head;
    int safety = 0;
    while (cur && safety < 100) { // safety guard against infinite loops
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
