/* 
Solve game pigeon word hunt map, gives out text direction of how to construct the word
*/

#include <iostream>
#include <string> 
#include <vector>
#include <fstream> 
#include <unordered_set>
#include <algorithm> 
using namespace std; 

char A = 'A';
int instructLength = 8; 

// int main() {
//   //create the file wordListLimited from wordList all
//   //chose the words length from 3 to 6
//   ifstream inputFile; 
//   ofstream outputFile;
//   string output = ""; 
//   cout << "Opening file wordListALL.txt." << endl;
  
//   inputFile.open("wordListALL.txt");
//   if (!inputFile.is_open()) {
//       cerr << "Could not open file" << endl;
//       return 1; // 1 indicates error
//   }

//   outputFile.open("wordListLimited.txt"); 
//   if (!outputFile.is_open()) {
//       cerr << "Could not open " << endl;
//       return 1;
//   }

//   int count = 0; 

// //write words with length 3 to 6 into another file 
//   while (!inputFile.fail()) {
//     getline(inputFile, output); 
//     //target length 3 to 6 inclusive, but there's the \0 at the end 
//     if(output.length() >= 4 && output.length() <= 17){ 
//         cout << output << endl; 
//         outputFile << output << endl; 
//         count ++; 
//     }
//   }
//   cout << "Written into file: " << count << " words." << endl; 

//   inputFile.close(); 
//   outputFile.close(); 
// }
struct TrieNode{
    bool isWord; 
    vector<TrieNode*> v;
    TrieNode(){
        isWord = false; 
        v = vector<TrieNode*>(26, nullptr);
    }
};

void insert(string& word, TrieNode* root){
    TrieNode* ptr = root; 
    for(int i = 0; i < word.size(); i++){
        char c = word.at(i); 
        if(!isalpha(c)){
            cerr << "insertion: not a letter, line: " << word << " char: " << c << endl; 
            break; 
        }
        if(ptr->v.at(c - A) == nullptr){
            ptr->v.at(c - A) = new TrieNode; 
        }
        ptr = ptr->v.at(c - A); 
    }
    ptr->isWord = true; 
}

bool checkWord(string word, TrieNode* root){
    TrieNode* ptr = root; 
    for(int i = 0; i < word.size(); i++){
        char c = word.at(i); 
        if(!isalpha(c)){
            cerr << "checkWord: not a letter, line: " << word << " char: " << c << endl; 
            return false; 
        }
        if(ptr->v.at(c - A) == nullptr){
            return false;  
        }
        ptr = ptr->v.at(c - A); 
    }
    return (ptr->isWord);
}
//   UpLeft(UL)            UP(UP)          UpRight(UR)
//   Left (LT)              000             Right (RT)
//   DownLeft(DL)         DOWN(DO)        DownRight (DR)

void getAllWords(int i, int j, const int& m, const int& n, vector<TrieNode*>& sequence, const vector<vector<char> >& board, 
vector<vector<bool> >& checkList, string& word, string& s, vector<vector<string> >& result, vector<vector<string> >& words){
    if(checkList.at(i).at(j)){
        return;
    }
    else{
        checkList.at(i).at(j) = true; 
    }
    char C = board.at(i).at(j);
//std::cout << C << endl; 
    TrieNode* temp = sequence.back()->v.at(C - A);
    if(temp == nullptr){
        checkList.at(i).at(j) = false; 
        return; 
    }
    else{
        sequence.push_back(temp); 
    }
    word.push_back(C); 
//cout << word << endl; 
    s.push_back(C); 
    if(sequence.back()->isWord){
//cout << word << " : " <<  s << endl; 
        result.at(word.size()).push_back(s); 
        words.at(word.size()).push_back(word); 
    }
    char tempC = '0'; 
    if(i - 1 >= 0){ //going up
        s += " -*UP*- "; //UP
        getAllWords(i - 1, j, m, n, sequence, board, checkList, word, s, result, words);
        for(int i = 0; i < instructLength; i++){
            s.pop_back(); 
        }

        if(j - 1 >= 0){
            s += " -*UL*- "; //UPLEFT
            getAllWords(i - 1, j - 1, m, n, sequence, board, checkList, word, s, result, words);
            for(int i = 0; i < instructLength; i++){
                s.pop_back(); 
            }
        }
        if(j + 1 < n){
            s += " -*UR*- "; //UPRIGHT
            getAllWords(i - 1, j + 1, m, n, sequence, board, checkList, word, s, result, words);
            for(int i = 0; i < instructLength; i++){
                s.pop_back(); 
            }
        }
    }
    if(i + 1 < m){ //going down
        s += " -*DO*- "; //DOWN
        getAllWords(i + 1, j, m, n, sequence, board, checkList, word, s, result, words);
        for(int i = 0; i < instructLength; i++){
            s.pop_back(); 
        }
        if(j - 1 >= 0){
            s += " -*DL*- "; //DOWN LEFT
            getAllWords(i + 1, j - 1, m, n, sequence, board, checkList, word, s, result, words);
            for(int i = 0; i < instructLength; i++){
                s.pop_back(); 
            }
        }
        if(j + 1 < n){
            s += " -*DR*- "; //DOWN RIGHT
            getAllWords(i + 1, j + 1, m, n, sequence, board, checkList, word, s, result, words);
            for(int i = 0; i < instructLength; i++){
                s.pop_back(); 
            }
        }
    }

    if(j - 1 >= 0){ //going left
        s += " -*LE*- "; //LEFT
        getAllWords(i, j - 1, m, n, sequence, board, checkList, word, s, result, words);
        for(int i = 0; i < instructLength; i++){
            s.pop_back(); 
        }
    }
    if(j + 1 < n){ //going right
        s += " -*RT*- "; //DOWN RIGHT
        getAllWords(i, j + 1, m, n, sequence, board, checkList, word, s, result, words);
        for(int i = 0; i < instructLength; i++){
            s.pop_back(); 
        }
    }
    sequence.pop_back(); 
    word.pop_back();
    s.pop_back(); 
    checkList.at(i).at(j) = false; 
}


int main(){
    ifstream inputFile; 
    inputFile.open("wordListLimited.txt");
    string line = ""; 
    TrieNode* root = new TrieNode; 
//int wordCount = 0; 
    string input = ""; 
    int dimensionOfBoard = 4; 
    if (!inputFile.is_open()) {
        cerr << "Could not open file" << endl;
        return 1; 
    }
    while (!inputFile.fail()) {
        getline(inputFile, line); 
        if(line.size() > 17 || line.size() < 3){ continue; }
        line.pop_back();  
//wordCount ++; 
        insert(line, root);  //total of 279370 word length from 3 to 16 letters
        if(!checkWord(line, root)){
            cerr << "Invalid word check: " << line << endl; 
        }
    }
    // cout << checkWord("SHARN", root) << endl; 
    // return 0; 
//cout << wordCount << endl; 
    inputFile.close(); 

    vector<vector<char> > board; 
    cout << "Enter the board in CAPITALIZED FORM, from left to right, top to bottom: " << endl; 
    cin >> input; 
    while(input.size() != (dimensionOfBoard * dimensionOfBoard)){
        cout << "Input size is not 16, please try again. Enter the board in CAPITALIZED FORM, from left to right, top to bottom: " << endl; 
        cin >> input; 
    }
    transform(input.begin(), input.end(), input.begin(), ::toupper); 
    int index = 0; 
    for(int i = 0; i < dimensionOfBoard; i++){
        vector<char> row; 
        for(int j = 0; j < dimensionOfBoard; j++){
            if(!isalpha(input.at(index))){
                cerr << "Not an alphabetic character, input: " << input << endl; 
                exit(1); 
            }
            row.push_back(input.at(index++));  
        }
        board.push_back(row); 
    }
    
    vector<vector<string> > result(17, vector<string>(0)); //result also contains instructions, result.at(i).at(j), i is length of string
    vector<vector<string> > words(17, vector<string>(0)); //words.at(i).at(j), i is length of string, word has only words
    vector<vector<bool> > checkList(dimensionOfBoard, vector<bool>(dimensionOfBoard, false));

    string word = ""; 
    string s = "";
    vector<TrieNode*> sequence; 
    sequence.push_back(root); 
    for(int i = 0; i < dimensionOfBoard; i++){ 
        for(int j = 0; j < dimensionOfBoard; j++){
            //the check here is not really necessary since every letter has some word that starts with it; 
            getAllWords(i, j, dimensionOfBoard, dimensionOfBoard, sequence, board, checkList, word, s, result, words); 
            
            for(int i = 0; i < checkList.size(); i++){
                for(int j = 0; j < checkList.at(i).size(); j++){
                    if(checkList.at(i).at(j)){
                        cerr << i << " : " << j << endl; 
                        cerr << "checkList not reset correctly" << endl; 
                        exit(3); 
                    }
                }
            }
            if(!word.empty()){
                cerr << "word is not empty: " << word << endl; 
            }
            if(!s.empty()){
                cerr << "s is not empty: " << s << endl; 
            }
            if(sequence.size() != 1){
                cerr << "sequence size wrong, size: " << sequence.size() << endl; 
            }
            if(!word.empty() || !s.empty() || sequence.size() != 1 ){
                exit(1); 
            }
        }
    }
    if(result.size() != words.size()){
        cerr << "result size != words size, result.size(): " << result.size() << " words.size(): " << words.size() << endl; 
        exit(1);
    }
    for(int i = result.size() - 1; i >= 0; i--){
        if(result.at(i).size() != words.at(i).size()){
            cerr << "At i = " << i << " result size != words size, result.at(i).size(): " << result.at(i).size() << " words.at(i).size(): " << words.at(i).size() << endl; 
            exit(1);
        }
    }
    unordered_set<string> seen; 
    
    for(int i = 0; i < result.size(); i++){
        if(result.at(i).empty()){
            continue;
        }
        cout << "---------- LENGTH OF " << i << " ----------" << endl; 
        for(int j = 0; j < result.at(i).size(); j++){
            if(seen.find(words.at(i).at(j)) == seen.end()){
                cout << words.at(i).at(j) << ": " << result.at(i).at(j) << endl; 
                seen.insert(words.at(i).at(j));
            }
        }
    }
    return 0; 
}

/*
SHAR
PING
RAIL
MEDO

SHARPINGRAILMEDO
*/
