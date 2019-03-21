#include "TernaryDagDictionary.h"

#define _HASH_TABLE_SIZE 12000000
TernaryDagNodePtr hashTable[_HASH_TABLE_SIZE];

TernaryDagDictionary::TernaryDagDictionary(TernaryDagNodePtr head, string longWord, unsigned longWordLength, unsigned wordCount, unsigned nodeCount) : root(head), longestWord(longWord), longestWordLength(longWordLength), numOfWords(wordCount), numOfNodes(nodeCount) {
}

TernaryDagNodePtr TernaryDagDictionary::getRootValue() const {
  return root;
}

void TernaryDagDictionary::downloadDictionary() {
  FILE* InputFileObj;
  //CURL object declaration
  CURL* curlHandle;
  //Result of the whole process
  CURLcode result;
  //URL of file to be downloaded
  const char* url = "https://raw.githubusercontent.com/dwyl/english-words/master/words.txt";
  //FILENAME_MAX is a constant int macro expression defined in stdio.h that represents the max length of a file name string under POSIX.1 standard
  char outputFilename[FILENAME_MAX] = "dic.txt";
  //Returns a CURL easy handle for reuse in other easy functions
  curlHandle = curl_easy_init();
  
  //Easy handle has no problems
  if (curlHandle)
  {
    //Create and open words_alpha.txt for writing
    InputFileObj = fopen(outputFilename,"wb");
    //Download words_alpha.txt
    curl_easy_setopt(curlHandle, CURLOPT_URL, url);
    //Tell libcurl to get ready to redirect contents of words_alpha.txt from stdout
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, NULL);
    //libcurl redirects contents of the downloaded words_alpha.txt to the created file words_alpha.txt
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, InputFileObj);
    //Perform and store the expected result
    result = curl_easy_perform(curlHandle);
    //Check for errors
    if(result != CURLE_OK) {
      //Show what is wrong with result
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
      return;
    }
  }
  //Easy handle has problems
  else {
    fprintf(stderr, "Curl init failed!\n");
    return;
  }
  //Cleanup since curl_easy_init has been used
  curl_easy_cleanup(curlHandle);
  fclose(InputFileObj);
}

void TernaryDagDictionary::dictionaryDimensions(TernaryDagDictionary &dictionary, TernaryDagDictionaryPtr &dictionaryPtr) {
  string word;
  ifstream InputFile;
  ofstream OutputFile;
  
  InputFile.open("dic.txt");
  if(!InputFile) {
    cerr << "Cannot open input file!\n";
    abort();
  }
  else {
    while(getline(InputFile, word)) {
      if(dictionaryPtr->longestWordLength < (unsigned)word.length()) {
        dictionaryPtr->longestWordLength = (unsigned)word.length();
        dictionaryPtr->longestWord = word;
      }
      dictionaryPtr->insert(word);
      dictionary.insert(word);
    }
    InputFile.close();
  }
  
  OutputFile.open("dictDim.txt");
  if(!OutputFile) {
    cerr << "Cannot create output file!\n";
    return;
  }
  else {
    OutputFile << "\nDOWNLOADED DICTIONARY DIMENSIONS\n"
               << "--------------------------------\n"
               << "Longest Word:\t\t" << dictionaryPtr->longestWord
               << "\nLongest Word Length:\t" << dictionaryPtr->longestWordLength
               << "\nNumber of Words:\t" << dictionaryPtr->numOfWords
               << "\n\n\n"
               << "TERNARY TRIE DICTIONARY DIMENSIONS\n"
               << "-----------------------------------\n"
               << "Longest Word:\t\t" << dictionary.longestWord
               << "\nLongest Word Length:\t" << dictionary.longestWordLength
               << "\nNumber of Words:\t" << dictionary.numOfWords
               << "\nNumber of Nodes:\t" << dictionary.numOfNodes
               << "\n\n\n";
    OutputFile.close();
  }
}

void TernaryDagDictionary::insert(const string &word) {
  root = addAWord(root, word, 0);
}

TernaryDagNodePtr TernaryDagDictionary::newTernaryDagNode(char sym) {
  TernaryDagNodePtr currentNode = new TernaryDagNode;
  currentNode->symbol = sym;
  currentNode->isAWord = false;
  currentNode->leftChild = NULL;
  currentNode->middleChild = NULL;
  currentNode->rightChild = NULL;
  return currentNode;
};

TernaryDagNodePtr TernaryDagDictionary::addAWord(TernaryDagNodePtr root, const string &word, int index) {
  if(!root) {
    root = newTernaryDagNode(word.at(index));
    numOfNodes++;
  }
  if(root->symbol > word.at(index))
    root->leftChild = addAWord(root->leftChild, word, index);
  else if(root->symbol < word.at(index))
    root->rightChild = addAWord(root->rightChild, word, index);
  else if(index < word.length() - 1)
    root->middleChild = addAWord(root->middleChild, word, index + 1);
  else {
    root->isAWord = true;
    numOfWords++;
    if(longestWordLength < word.length()) {
      longestWordLength = word.length();
      longestWord = word;
    }
  }
  return root;
}

void TernaryDagDictionary::balance() {
  root->count = setCount(root);
  root = balanceLevel(root);
  balanceTernaryTrieDictionary(root);
}

unsigned TernaryDagDictionary::setCount(TernaryDagNodePtr root) {
  if(!root)
    return 0;
  root->count = setCount(root->leftChild) + setCount(root->rightChild) + 1;
  setCount(root->middleChild);
  
  return root->count;
}

//Balance a level of the Ternary Trie
TernaryDagNodePtr TernaryDagDictionary::balanceLevel(TernaryDagNodePtr root) {
  if( (!root) || (root->count == 1) )
    return root;
  //Make middle node the root
  root = divide( root, (root->count / 2) );
  //Recursively balance subtrees
  root->leftChild = balanceLevel(root->leftChild);
  root->rightChild = balanceLevel(root->rightChild);
  
  return root;
}

//Balance all levels of the Ternary Trie
void TernaryDagDictionary::balanceTernaryTrieDictionary(TernaryDagNodePtr root) {
  if(!root)
    return;
  root->middleChild = balanceLevel(root->middleChild);
  balanceTernaryTrieDictionary(root->middleChild);
  balanceTernaryTrieDictionary(root->leftChild);
  balanceTernaryTrieDictionary(root->rightChild);
}

TernaryDagNodePtr TernaryDagDictionary::divide(TernaryDagNodePtr root, int dividingCount) {
  int leftChildCount = (!root->leftChild) ? 0: root->leftChild->count;
  
  //If the pivot node is in the left subtree, then traverse it with pivot node being the root
  if(dividingCount < leftChildCount) {
    root->leftChild = divide(root->leftChild, dividingCount);
    //On the way back from pivot node to the root of the Ternary Trie, rotate right
    root = rotateRight(root);
  }
  //Same procedure applies to the right subtree
  else if(dividingCount > leftChildCount)  {
    root->rightChild = divide(root->rightChild, dividingCount - leftChildCount - 1);
    root = rotateLeft(root);
  }
  
  return root;
}

TernaryDagNodePtr TernaryDagDictionary::rotateRight(TernaryDagNodePtr root) {
  TernaryDagNodePtr nodeX = root->leftChild;
  
  //Move the subtree mounted at root between nodeX and root
  root->leftChild = nodeX->rightChild;
  //Swap nodeX and root
  nodeX->rightChild = root;
  //Restore count field of root
  root->count = (root->leftChild ? root->leftChild->count : 0) +
			  (root->rightChild ? root->rightChild->count : 0) + 1;
	nodeX->count = (nodeX->leftChild ? nodeX->leftChild->count : 0) +
			  nodeX->rightChild->count + 1;
	
	return nodeX;
}

TernaryDagNodePtr TernaryDagDictionary::rotateLeft(TernaryDagNodePtr root) {
  TernaryDagNodePtr nodeX = root->rightChild;
	
	//Move the subtree mounted at root between nodeX and root
	root->rightChild = nodeX->leftChild;
	//Swap nodeX and root
	nodeX->leftChild = root;
	//Restore count field of root
	root->count = (root->leftChild ? root->leftChild->count : 0) +
			  (root->rightChild ? root->rightChild->count : 0) + 1;
	nodeX->count = nodeX->leftChild->count +
	      (nodeX->rightChild ? nodeX->rightChild->count : 0) + 1;
	
	return nodeX;
}

//Append balanced dictionary dimensions to dictionary dimensions file
void TernaryDagDictionary::compressedDictionaryDimensions() {
  ofstream OutputFile;
  OutputFile.open("dictDim.txt", ios_base::app);
  if(!OutputFile) {
    cerr << "Cannot open file in order to append to it!\n";
    return;
  }
  else {
    OutputFile << "TERNARY DAWG DICTIONARY DIMENSIONS\n"
               << "----------------------------------\n"
               << "Longest Word:\t\t" << longestWord
               << "\nLongest Word Length:\t" << longestWordLength
               << "\nNumber of Words:\t" << numOfWords
               << "\nNumber of Nodes:\t" << numOfNodes
               << "\n\n\n";
  }
}

void TernaryDagDictionary::compressTernaryTrieDictionary() {
  root->hash = computeHash(root);
  removeDuplicateSuffixes(root);
}

unsigned TernaryDagDictionary::computeHash(TernaryDagNodePtr root) {
  if(!root)
    return 0;
  //Compute the hashes for the children of a node recursively and use them for the current hash
	root->hash = (root->symbol - 'a') + 31 * computeHash(root->middleChild);
	root->hash ^= computeHash(root->leftChild);
	root->hash ^= computeHash(root->rightChild);
	root->hash ^= (root->hash >> 16);
	//Resulting hash field must be unsigned to facilitate the correct modulo computation
	root->hash %= _HASH_TABLE_SIZE;
	return root->hash;
}

//Remove duplicate suffixes beginning with the longest one
void TernaryDagDictionary::removeDuplicateSuffixes(TernaryDagNodePtr root) {
  if(root->leftChild) {
		//If the node already exists in the hash table, then checkAndRemoveDuplicateSuffixes
		//returns false: its children were checked for duplicates already, so there's no need to
		//check for duplicate children twice
		if( checkAndRemoveDuplicateSuffixes(&root->leftChild) )
			removeDuplicateSuffixes(root->leftChild);
	}
	if(root->rightChild) {
		if( checkAndRemoveDuplicateSuffixes(&root->rightChild) )
			removeDuplicateSuffixes(root->rightChild);
	}
	if(root->middleChild) {
		if( checkAndRemoveDuplicateSuffixes(&root->middleChild) )
			removeDuplicateSuffixes(root->middleChild);
	}
}

bool TernaryDagDictionary::checkAndRemoveDuplicateSuffixes(TernaryDagNodePtr *root) {
  TernaryDagNodePtr node = *root;
	unsigned hashIndex = node->hash;
	
	while(hashTable[hashIndex] != 0) {
		if( isEqual(hashTable[hashIndex], node) ) {
			//This node already exists in the hash table. So, remove the duplicate.
			freeNode(node);
			*root = hashTable[hashIndex];
			return false;
		}
		hashIndex = (hashIndex + 1) % _HASH_TABLE_SIZE;
	}
	hashTable[hashIndex] = node;
	
	return true;
}

bool TernaryDagDictionary::isEqual(TernaryDagNodePtr oldNode, TernaryDagNodePtr newNode) {
  if(!oldNode || !newNode)
		return ( (!oldNode) && (!newNode) );
	//Two nodes are equal if their characters and their children are equal
	return ( (oldNode->symbol == newNode->symbol) &&
		( isEqual(oldNode->leftChild, newNode->leftChild) ) && ( isEqual(oldNode->middleChild, newNode->middleChild) ) && ( isEqual(oldNode->rightChild, newNode->rightChild) ) );
}

//Free the subtree recursively
void TernaryDagDictionary::freeNode(TernaryDagNodePtr root) {
  if(!root)
		return;
	freeNode(root->leftChild);
	freeNode(root->rightChild);
	freeNode(root->middleChild);
	delete root;
	root = NULL;
	numOfNodes--;
}

//Remove all nodes in the hash table
void TernaryDagDictionary::freeHashTable() {
  for(int index = 0; index < _HASH_TABLE_SIZE; ++index) {
    if(hashTable[index]) {
      delete *(hashTable + index);
      numOfNodes--;
    }
  }
  delete root;
  root = NULL;
  numOfNodes--;
}

string TernaryDagDictionary::correct(const string &word) {
  if(search(root, word, 0))
    return word;
  else
    return "";
}

void TernaryDagDictionary::correct(const string &word, vector<string> candidates) {
  bool suggestions = false;
  vector<string> result;
  vector<string> subResult;
  
  edits(word, result);
  
  for(string::size_type index = 0; index < result.size(); ++index) {
    if(search(root, result[index], 0)) {
      candidates.push_back(result[index]);
      suggestions = true;
    }
  }
  
  if(suggestions) {
    sort( candidates.begin() , candidates.end() );
    candidates.erase( unique( candidates.begin(), candidates.end() ), candidates.end() );
    cout << "\nDid you mean:\n";
    for(string::size_type index = 0; index < candidates.size(); ++index)
      cout << ">>>  " << candidates[index] << '\n';
    return;
  }
  else {
    for(unsigned index = 0; index < result.size(); ++index)
      edits(result[index], subResult);
      
    for(string::size_type index = 0; index < subResult.size(); ++index) {
      if(search(root, subResult[index], 0)) {
        candidates.push_back(subResult[index]);
        suggestions = true;
      }
    }
    
    if(suggestions) {
      sort( candidates.begin() , candidates.end() );
      candidates.erase( unique( candidates.begin(), candidates.end() ), candidates.end() );
      cout << "\nDid you mean:\n";
      for(string::size_type index = 0; index < candidates.size(); ++index)
        cout << ">>>  " << candidates[index] << '\n';
      return;
    }
  }
  cout << ">>>  NOT FOUND: cannot generate correction suggestions\n";
}

void TernaryDagDictionary::edits(const string &word, vector<string> &result) {
  //Deletions
  for(string::size_type index = 0; index < word.length(); index++)
    result.push_back(word.substr(0, index) + word.substr(index + 1));
  
  //Transpositions
  for(string::size_type index = 0; index < word.length() - 1; index++)
    result.push_back(word.substr(0, index) + word[index + 1] + word[index] + word.substr(index + 2));

  for(char j = 'a'; j <= 'z'; ++j)
  {
    //Alterations
    for(string::size_type index = 0; index < word.length(); index++)
      result.push_back(word.substr(0, index) + j + word.substr(index + 1));
    
    //Insertions
    for(string::size_type index = 0; index < word.length() + 1; index++)
      result.push_back(word.substr(0, index) + j + word.substr(index));
  }
}

bool TernaryDagDictionary::search(TernaryDagNodePtr root, const string &word, unsigned index) {
  if(!root)
    return false;
    
  if(word.at(index) < root->symbol)
    return search(root->leftChild, word, index);
    
  else if(word.at(index) > root->symbol)
    return search(root->rightChild, word, index);
    
  else if(index < word.length() - 1)
    return search(root->middleChild, word, index + 1);
    
  else
      return root->isAWord;
}

void TernaryDagDictionary::print() {
  char buffer[MAX_WORD_LENGTH];
  printLexicographically(root, buffer, 0);
}

void TernaryDagDictionary::printLexicographically(TernaryDagNodePtr root, char* buffer, int depth) {
  if(!root)
    return;
    
  printLexicographically(root->leftChild, buffer, depth);
  buffer[depth] = root->symbol;
  if(root->isAWord) {
      buffer[depth + 1] = '\0';
      cout << buffer << '\n';
  }
  printLexicographically(root->middleChild, buffer, depth + 1);
  printLexicographically(root->rightChild, buffer, depth);
}

void TernaryDagDictionary::writeToScreen() {
  ifstream inFile;
  string line;
  
  inFile.open("dictDim.txt");
  if(!inFile) {
    cerr << "\nCannot open file!\n";
    abort();
  }
  else {
      while(getline(inFile, line)) {
        cout << "\t\n" << line;
      }
    inFile.close();
  }
}