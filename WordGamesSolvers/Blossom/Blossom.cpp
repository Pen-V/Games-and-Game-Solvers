#include <iostream>
#include <string> 
#include <vector>
#include <fstream> 
//#include <algorithm>


using namespace std; 


int find(const vector<string>& list, string s, int start = 0){
  /*return index of a string in a vector of strings 
    if not found return -1 */
  if(list.size() <= 0){
    return -1; 
  }
  for(int i = start; i < list.size(); i++){ //going through everything in the list to see if the string s exists in there
    if(list.at(i).compare(s) == 0){
      return i; 
    }
  }
  return -1; 
}
//find for char vector
int find(const vector<char>& list, char c, int start = 0){
  /*return index of a string in a vector of strings 
    if not found return -1 */
  if(list.size() <= 0){
    return -1; 
  }
  for(int i = start; i < list.size(); i++){ //going through everything in the list to see if the string s exists in there
    if(list.at(i) == c){
      return i; 
    }
  }
  return -1; 
}

void playBlossom(vector<char> list, char key){
  ifstream file; 
  string input = ""; 
  vector<string> wordList; 
  vector<vector<string> > listByLength (58, wordList);
  bool validWord = true; 
  //vector<vector<string> > listByLength;
  //listByLength.push_back(wordList);
  file.open("wordListALL.txt"); 

  //always check if the file is open 
  if(!file.is_open()) {
      cerr << "Could not open file" << endl;
      // 1 indicates error
  }

/* //int max = 0; 
//longest word in english 57 letters? 
  while(!file.fail()){
    getline(file, input); 
    input = input.substr(0, input.size() - 1); 
    if(input.size() > max){
      max = input.size(); 
    }
    cout << max << endl; 
  }*/ 

  //until ending 
  while(!file.fail()){
    validWord = true; 
    getline(file, input); //get each line from the txt document into a string
    if(input.size() <= 4 || input.find(key) == string::npos){
      continue; 
    }
    input = input.substr(0, input.size() - 1);
    /*the above line will ends with \n so needs to take off the ending character 
    with substr to get the full word */
    for(int i = 0; i < input.size() && validWord; i++){
      if(find(list, input.at(i)) < 0){
        validWord = false; 
        break; 
      }
    }
    if(!validWord){
      continue; 
    }
    else{
      if(input.size() > listByLength.size()){
        for(int i = 1; i <= input.size() - listByLength.size(); i++){
          listByLength.push_back(wordList);
        }
      }
      listByLength.at(input.size()).push_back(input);
    }
    //wordList.push_back(input); 
  }
  //always close file
  file.close();

  for(int i = listByLength.size() - 1; i >= 5; i--){
    if(listByLength.at(i).empty()){
      continue; 
    }
    cout << "Group: " << i << endl; 
    cout << "--------------------" << endl;
    for(int j = 0; j < listByLength.at(i).size(); j++){
      cout << listByLength.at(i).at(j) << " -*- " << flush;
    }
    cout << endl; 
    cout << "--------------------" << endl;
  }
}
 

int main() {
  vector<string> wordList; 
  vector<char> charList; 
  string s; 
  char key; 
  cout << "Enter the peripheral letters in series first, then enter the central letter (the key) --- these letter should be ALL CAPITALIZED: " << endl; 
  getline(cin, s); 
  for(int i = 0; i < s.size(); i++){
    charList.push_back(s.at(i));
  }
  key = charList.back();
  playBlossom(charList, key);

}

// ./Blossom