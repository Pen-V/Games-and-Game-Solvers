/* 
Solve game pigeon word hunt map, gives out a word map of how to construct the word
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

void getAllWords(int i, int j, const int& m, const int& n, vector<TrieNode*>& sequence, 
const vector<vector<char> >& board,  string& word, vector<vector<int> >& wordMap, 
vector<vector<vector<vector<int> > > >& result, vector<vector<string> >& words){
    if(wordMap.at(i).at(j) != 0){
        return;
    }
    char C = board.at(i).at(j);
//std::cout << C << endl; 
    TrieNode* temp = sequence.back()->v.at(C - A);
    if(temp == nullptr){
        return; 
    }
    else{
        sequence.push_back(temp); 
    }
    word.push_back(C); 
//cout << word << endl; 
    wordMap.at(i).at(j) = word.size(); 
    if(sequence.back()->isWord){
//cout << word << " : " <<  s << endl; 
        result.at(word.size()).push_back(wordMap); 
        words.at(word.size()).push_back(word); 
    }
    char tempC = '0'; 
    if(i - 1 >= 0){ //going up
        //UP
        getAllWords(i - 1, j, m, n, sequence, board, word, wordMap, result, words);

        if(j - 1 >= 0){
            //UPLEFT
            getAllWords(i - 1, j - 1, m, n, sequence, board, word, wordMap, result, words);
        }
        if(j + 1 < n){
           //UPRIGHT
            getAllWords(i - 1, j + 1, m, n, sequence, board, word, wordMap, result, words);
        }
    }
    if(i + 1 < m){ //going down
        //DOWN
        getAllWords(i + 1, j, m, n, sequence, board, word, wordMap, result, words);

        if(j - 1 >= 0){
         //DOWN LEFT
            getAllWords(i + 1, j - 1, m, n, sequence, board, word, wordMap, result, words);

        }
        if(j + 1 < n){
            //DOWN RIGHT
            getAllWords(i + 1, j + 1, m, n, sequence, board, word, wordMap, result, words);
        }
    }

    if(j - 1 >= 0){ //going left
        getAllWords(i, j - 1, m, n, sequence, board, word, wordMap, result, words);
    }
    if(j + 1 < n){ //going right
        //DOWN RIGHT
        getAllWords(i, j + 1, m, n, sequence, board, word, wordMap, result, words);
    }
    sequence.pop_back(); 
    word.pop_back();
    wordMap.at(i).at(j) = 0; 
}
void printWordMap(vector<vector<int> >& wordMap){

    for(int i = 0; i < wordMap.size(); i++){
        for(int j = 0; j < wordMap.at(i).size(); j++){
            cout << wordMap.at(i).at(j) << "\t"; 
        }
        cout << endl; 
    }
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
    
    vector<vector<vector<vector<int> > > > result(17, vector<vector<vector<int> > >(0)); //result also contains instructions, result.at(i).at(j), i is length of string
    vector<vector<string> > words(17, vector<string>(0)); //words.at(i).at(j), i is length of string, word has only words

    string word = ""; 
    vector<vector<int> > wordMap(dimensionOfBoard, vector<int>(dimensionOfBoard, 0)); 
    //string s = "";
    vector<TrieNode*> sequence; 
    sequence.push_back(root); 
    for(int i = 0; i < dimensionOfBoard; i++){ 
        for(int j = 0; j < dimensionOfBoard; j++){
            //the check here is not really necessary since every letter has some word that starts with it; 
            getAllWords(i, j, dimensionOfBoard, dimensionOfBoard, sequence, board, word, wordMap, result, words); 
            
            for(int i = 0; i < wordMap.size(); i++){
                for(int j = 0; j < wordMap.at(i).size(); j++){
                    if(wordMap.at(i).at(j) != 0){
                        cerr << i << " : " << j << endl; 
                        cerr << "wordMap not reset correctly" << endl; 
                        exit(1); 
                    }
                }
            }
            if(!word.empty()){
                cerr << "word is not empty: " << word << endl; 
            }
            if(sequence.size() != 1){
                cerr << "sequence size wrong, size: " << sequence.size() << endl; 
            }
            if(!word.empty() || sequence.size() != 1 ){
                exit(2); 
            }
        }
    }
    if(result.size() != words.size()){
        cerr << "result size != words size, result.size(): " << result.size() << " words.size(): " << words.size() << endl; 
        exit(3);
    }
    for(int i = result.size() - 1; i >= 0; i--){
        if(result.at(i).size() != words.at(i).size()){
            cerr << "At i = " << i << " result size != words size, result.at(i).size(): " << result.at(i).size() << " words.at(i).size(): " << words.at(i).size() << endl; 
            exit(4);
        }
    }
    unordered_set<string> seen; 
    cout << "Starting to print word and word maps, press enter for next, enter c to finish printing without more prompts" << endl; 
    char command = 'x'; 
    int count = 0; 
    int intervals = 3; //how many words to print at a time
    char temp = '0'; 
    cin.get(temp); 

    for(int i = result.size() - 1; i >= 0; i--){
        if(result.at(i).empty()){
            continue;
        }
        cout << "---------- LENGTH OF " << i << " ----------" << endl; 
        for(int j = 0; j < result.at(i).size(); j++){
            if(seen.find(words.at(i).at(j)) == seen.end()){
                cout << words.at(i).at(j) << ": " << endl; 
                count++; 
                printWordMap(result.at(i).at(j));
                seen.insert(words.at(i).at(j));  
                if(count % intervals == 0){
                    cout << "--------------------------------" << endl; 
                    if(command != '\n'){
                        cin.get(command); 
                        if(command == 'c'){
                            command = '\n';
                        }
                        else{
                            command = 'x'; 
                        }
                    }
                }
            }
        }
    }
    cout << "Found in total " << count << " words." << endl; 
    return 0; 


}

/*
SHAR
PING
RAIL
MEDO

test board SHARPINGRAILMEDO

   // cout << "0\t12\t13\t14" << endl; 
    //  cout << "1\t2\t13\t14" << endl; 
    //   cout << "4\t7\t18\t11" << endl; 
    //    cout << "5\t6\t19\t10"<< endl; 
*/
