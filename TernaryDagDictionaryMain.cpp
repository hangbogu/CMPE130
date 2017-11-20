#include "TernaryDagDictionary.h"

int main() {
  TernaryDagDictionary dictionary, trieDictionary;
  
  //Download dictionary from web
  dictionary.downloadDictionary();
  
  //Determine dictionary dimensions
  dictionary.dictionaryDimensions(trieDictionary);
  
  //Print Ternary Trie Dictionary
  // trieDictionary.print();
  // cout << endl;
  //trieDictionary.testPrint();
  trieDictionary.balance();
  // //cout << endl;
  // //trieDictionary.testPrint();
  // //cout << endl;
  // // trieDictionary.print();
  // // cout << endl;
  trieDictionary.compressTernaryTrieDictionary();
  trieDictionary.balancedDictionaryDimensions();
  // trieDictionary.print();
  // cout << endl;
  return 0;
}