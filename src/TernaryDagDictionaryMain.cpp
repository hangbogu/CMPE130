#include "TernaryDagDictionary.h"

int main() {
  TernaryDagDictionaryPtr dictionaryPtr = new TernaryDagDictionary;
  TernaryDagDictionary dictionary;
  bool done = true;
  string word;
  vector<string> candidates;
  
  cout << "\nDownloading Word List Dictionary...\n";
  dictionaryPtr->downloadDictionary();
  
  //cout << "\nWord List Dictionary download complete...\n";
  cout << "\nBuilding Ternary Search Tree Dictionary from downloaded Word List Dictionary...\n";
  dictionaryPtr->dictionaryDimensions(dictionary, dictionaryPtr);
  
  //cout << "\nTernary Search Tree Dictionary build complete...\n";
  cout << "\nBalancing Ternary Search Tree Dictionary...\n";
  dictionaryPtr->balance();
  dictionary.balance();
  
  cout << "\nCompressing Ternary Search Tree Dictionary to form a Ternary Directed Acyclic Word Graph Dictionary...\n";
  dictionary.compressTernaryTrieDictionary();

  dictionary.compressedDictionaryDimensions();
  dictionary.writeToScreen();
 
  cout << "\nEnter a search query:\n>>>  ";
  cin  >> word;
    
  while(word != "quit") {
    string correctWord = dictionaryPtr->correct(word);
    
    if(correctWord == word)
      cout << ">>>  FOUND: no correction suggestion\n";
    else {
      dictionaryPtr->correct(word, candidates);
    }
      
    cout << "\nEnter a search query:\n>>>  ";
    cin  >> word;
  }
  
  if(!dictionaryPtr->getRootValue()) {
    dictionaryPtr->freeHashTable();
    dictionary.freeHashTable();
  }
  delete dictionaryPtr;
  dictionaryPtr = NULL;
  done = false;
  
  cout << endl;
  return 0;
}