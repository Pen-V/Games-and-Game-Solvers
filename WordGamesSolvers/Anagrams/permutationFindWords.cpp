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

void permutation(string s, int x, vector<string>& v, string perm = ""){ 
  /*put all permutation (with length x) of a string into vector v
  perm is default = "" if not empty return perm + permutation stuff (with reduced length)
  */

  // if(s.size() < x || x < 0){
  //   cerr << "Error: permuting too few or too many letters" << endl; 
  // }

  //the latter requirement eliminates repetitions in the print out result 
  //based case
  if(perm.length() == x && find(v, perm) < 0){
      v.push_back(perm); //put the result into vector perm 
  }
  //classic permutation with a string by putting characters into an empty string recursively
  for(int i = 0; i < s.length(); i++){
    perm += s.at(i); 
    s.erase(s.begin() + i); 
    permutation(s, x, v, perm);
    s.insert(s.begin() + i, perm.back());
    perm.erase(perm.length() - 1);
  }
  
}

int main() {
  //create the file wordListLimited from wordList all
  //chose the words length from 3 to 6
  /*
  ifstream inputFile; 
  ofstream outputFile;
  string output = ""; 
  cout << "Opening file wordListALL.txt." << endl;
  
  inputFile.open("wordListALL.txt");
  if (!inputFile.is_open()) {
      cerr << "Could not open file" << endl;
      return 1; // 1 indicates error
  }

  outputFile.open("wordListLimited.txt"); 
  if (!outputFile.is_open()) {
      cerr << "Could not open " << endl;
      return 1;
  }

  int count = 0; 

//write words with length 3 to 6 into another file 
  while (!inputFile.fail()) {
    getline(inputFile, output); 
    //target length 3 to 6 inclusive, but there's the \0 at the end 
    if(output.length() >= 4 && output.length() <= 7){ 
      outputFile << output << endl; 
      count ++; 
    }
  }
  cout << "Written into file: " << count << " words." << endl; 

  inputFile.close(); 
  outputFile.close(); */
  /*
  vector<string> wordList; 
  wordList.push_back("abc1");
  wordList.push_back("def1");
  wordList.push_back("ghi1"); 
  cout << find(wordList, "ghi") << endl; //testing find function 
  cout << find(wordList, "abcs") << endl; 
  cout << find(wordList, "abc") << endl; */


  //open the file to get the legit words into a vector --> checklist 
  //everything in checklist will be in UPPER case becasue the dictionary list is all in UPPER case
  //--> when permuting use upper case letters


//for length 7 words perm
/*
ifstream file; 
string input = ""; 
vector<string> checklist; 
file.open("wordListALL.txt"); 

//always check if the file is open 
if(!file.is_open()) {
    cerr << "Could not open file" << endl;
    return 1; // 1 indicates error
}
//until ending 
while(!file.fail()){
  getline(file, input); //get each line from the txt document into a string
  if(input.size() <= 8 && input.size() >= 5){
    checklist.push_back(input.substr(0, input.size() - 1));
  }
   //that line will ends with \n so needs to take off the ending character with substr to get the full word 
  
}

//always close file
file.close();*/





/*    //test if input into checklist works 
  for(int i = 0; i < checklist.size(); i++){
    cout << checklist.at(i) << " " << checklist.at(i).length() << endl; 
  }*/

//just for breaking apart with the other printing test
/*
  cout << "------------" << endl; 
  cout << "------------" << endl;
  cout << "------------" << endl;
  cout << "------------" << endl;
  cout << "------------" << endl;
  cout << "------------" << endl;
  cout << "------------" << endl;
*/

  ifstream file; 
  string input = ""; 
  vector<string> checklist; 
  file.open("wordListLimited.txt"); 

  //always check if the file is open 
  if(!file.is_open()) {
      cerr << "Could not open file" << endl;
      return 1; // 1 indicates error
  }
  //until ending 
  while(!file.fail()){
    getline(file, input); //get each line from the txt document into a string
    checklist.push_back(input.substr(0, input.size() - 1)); //that line will ends with \n so needs to take off the ending character with substr to get the full word 
    
  }

  //always close file
  file.close(); //everything in checklist will be in upper case --> when permuting use upper case letters

  vector<string> permList; //temp vector to store permutation with each length 
  string letters; 
  cout << "Enter the letters you want to permute (ALL CAPITALIZED): "; 
  cin >> letters; 
  cout << endl; 
  vector< vector<string> > parentList; 
  //permutate with length from full length to length == 3 (for anagram)
  for(int i = letters.size(); i >= 3; i--){//all perm with length including all letters to size 3 
    permutation(letters, i, permList); //get all the permutation with a length into a permList
    parentList.push_back(permList);  //put the permList into the parentList
    permList.clear(); //clear permList
  }

 //testing if permutation works  
  /*
  permutation(letters, 6, permList); 
  permutation(letters, 5, permList); 
  permutation(letters, 4, permList);
  permutation(letters, 3, permList); */ 

//testing if the permLists worked --> WONT WORK ANYMORE since permList replaced by parentList
//permList now will always be empty 
  /*
  for(int i = 0; i < permList.size(); i++){
    cout << permList.at(i) << " " << permList.at(i).size() << endl; 
  }*/

  for(int len = 0; len < parentList.size(); len++){
   
   //make output more readable by breaking apart each group of words by length 
    
    cout << "Group: " << (letters.length() - len) << endl;
    cout << "------------------" << endl;  
    
    for(int i = 0; i < parentList.at(len).size(); i++){
      //function find is just trying to see if the permutation is a word according to checklist
        if(find(checklist, parentList.at(len).at(i)) >= 0){
          //make each word clearly separated
          //also immediately print out after confirming it's valid with -- flush 
          cout << parentList.at(len).at(i) << " -*- " << flush; 
        }
      }
    //clear line for next group to be printed 
    cout << endl; 
    cout << "------------------" << endl;
  }

}
//copy and paste -- for compiling and running the program easier 
  /*
    permutationFindWords
    make permutationFindWords
    ./permutationFindWords
  */             