#include<bits/stdc++.h>

#include "qna_tool.h"



using namespace std;

std::string get_paragraph(int book_code, int page, int paragraph){

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

int main(){

    QNA_tool qna_tool;

    for(int i = 1; i <= 98; i++){
        

        std::cout << "Inserting book " << i << std::endl;

        std::string filename = "corpus/mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
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
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            assert(metadata[0] != 0);
            qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
            
        }

        inputFile.close();

    }

    // open queries.txt
    std::ifstream inputFile("queries.txt");

    // if output.txt does not exist, create it
    remove("output.txt");
    

    int cnt = 1;
    while(!inputFile.eof()){
        std::string question;
        std::getline(inputFile, question);

        if(question == ""){
            break;
        }

        // open in append mode
        std::ofstream outputFile("output.txt", std::ios_base::app);
        outputFile << "Processing Query: " << question << std::endl;
        outputFile << "==========================================\n";

        
        int k = 5;

        // Let's try to ask a simple question to the qna_tool
        Node* head = qna_tool.get_top_k_para(question, k);

        // write paragraph to output.txt
        outputFile << "Top " << k << " paragraphs:\n";
        while(head != NULL){
            outputFile << head->book_code << " " << head->page << " " << head->paragraph << std::endl;
            // outputFile << get_paragraph(head->book_code, head->page, head->paragraph) << std::endl;
            // outputFile << "=======================================================\n";
            head = head->right;
        }
        // qna_tool.query(question, "api_call.py");
        cnt++;
        outputFile.close(); 
    }

    inputFile.close();
    cout<<"finished";
    return 0;
}