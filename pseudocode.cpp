/*********************************************************************************
 * DICTIONARY DOWNLOAD
 *********************************************************************************/

/*Downloads a list of english words at a specified url
 *Input: url of word list
 *Output: words.txt file with the word list
*/
TST-Download(const char* url):
  FILE* InputFileObj
  //CURL object declaration
  CURL* curlHandle
  //Result of the whole process
  CURLcode result
  //FILENAME_MAX is a constant int macro expression defined in stdio.h that
  //represents the max length of a file name string under POSIX.1 standard
  outputFilename[FILENAME_MAX] <-- url
  //Returns a CURL easy handle for reuse in other easy functions
  curlHandle <-- curl_easy_init()
  //Easy handle has no problems
  if curlHandle is not NULL then:
    //Create and open words_alpha.txt for writing
    InputFileObj <-- fopen(outputFilename,"wb")
    //Download words_alpha.txt
    curl_easy_setopt(curlHandle, CURLOPT_URL, url)
    //Tell libcurl to get ready to redirect contents of words_alpha.txt from stdout
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, NULL)
    //libcurl redirects contents of the downloaded words_alpha.txt to the created file words_alpha.txt
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, InputFileObj)
    //Perform and store the expected result
    result = curl_easy_perform(curlHandle)
    //Check for errors
    if result is not equal to CURLE_OK then:
      //Show what is wrong with result
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result))
      return
  //Easy handle has problems
  else curlHandle is NULL then:
    fprintf(stderr, "Curl init failed!\n")
    return
  //Cleanup since curl_easy_init has been used
  curl_easy_cleanup(curlHandle)
  //close file
  fclose(InputFileObj)
END TST-Download


/*********************************************************************************
 * BUILD TST
 *********************************************************************************/

/*Initialize a newly created node
  *Input: a character
  *Output: a pointer to a newly created TST node
*/
TST-NewNode(char ch):
  curr <-- new TSTnode
  
  curr->symbol = ch
  curr->isAWord = false
  curr->leftChild = NULL
  curr->middleChild = NULL
  curr->rightChild = NULL

  return curr
END TST-NewNode

/*Inserts a word into a newly created TST node
  *Input: a string
  *Output: string is inserted in the TST
*/
TST-INSERT(TSTnode* root, char *word):
  //Tree/subtree is empty
  if root is NULL then:
    root <-- TST-NewNode(*word)

  //Current char of word is smaller than char at the root:
  //insert word in the left subtree
  if *word is less than root->ch then:
    root->leftChild = TST-INSERT(root->leftChild, word)

  //Current char of word is larger than char at the root:
  //insert word in the left subtree
  else if *word is greater than root->ch then:
    root->rightChild = TST-INSERT(root->leftChild, word)

  //Current char of word is same as char at the root:
  //insert word in the middle subtree
  else
    do:
      if *(word + 1) is not NULL char then:
        root->middleChild = TST-INSERT(root->middleChild, word + 1)
      //Mark the existence of a word
      else
        do:
          root->isAWord = true
END TST-INSERT

/***********************************************************************************************
 * BALANCE TST
 * ********************************************************************************************/

/*Balance TST: sets the count field of each node, balances a level of the TST, as well
 *balance all levels of the TST
 *Input: root of unbalanced TST
 *Output: a balanced TST with a new root
*/
TST-BALANCE(TSTnode* root):
  //Set number of children nodes on the same level
  root->count = TST-SetCount(root)
  //Balance a level of the TST
  root = TST-BalanceALevel(root)
  //Balance all levels of the TST
  TST-BalanceAllLevels(root)
END TST-BALANCE

/*Sets the number of children nodes on the same level
 *Input: root of unbalanced TST
 *Output: count field of each node in the TST is set
*/
TST-SetCount(TSTnode* root):
  if root is NULL then: return 0

  root->count = TST-SetCount(root->leftChild) + TST-SetCount(root->rightChild) + 1
  TST-SetCount(root->middleChild)
  
  return root->count
END TST-SetCount

/*Balances a level of the TST
 *Input: root of a subtree of the TST
 *Output: balanced subtree and its left and right children subtrees
*/
TST-BalanceALevel(TSTnode* root):
  if root is NULL or root->count is 1 then: return root
  //Make middle node the root
  root <-- divide( root, (root->count / 2) )
  //Recursively balance subtrees
  root->leftChild = balanceLevel(root->leftChild)
  root->rightChild = balanceLevel(root->rightChild)
  return root
END TST-BalanceALevel

/*Finds the pivot node: that is the subtree whose root node will be the new root of the TST
 *Input: root of unbalanced TST; a precomputed pivot position
 *Output: pointer to the new pivot node
*/
TST-Divide(TSTnode* root, unsigned pivotPos):
  leftChildCount <-- (root->leftChild is NULL) ? 0: root->leftChild->count
  
  //If the pivot node is in the left subtree, then traverse it with pivot node being the root
  if pivotPos is less than leftChildCount then:
    root->leftChild = TST-Divide(root->leftChild, pivotPos)
    //On the way back from pivot node to the root of the Ternary Trie, rotate right
    root <-- TST-RotateRight(root)
  
  //Same procedure applies to the right subtree
  else if pivotPos is greater than leftChildCount then:
    root->rightChild = TST-Divide(root->rightChild, pivotPos - leftChildCount - 1)
    root <-- TST-RotateLeft(root)
  
  return root
END TST-Divide

/*Balances all levels of the TST
 *Input: root of unbalanced TST
 *Output: balanced TST
*/
TST-BalaneAllLevels(TSTnode* root):
  if root is NULL then: return
  root->middleChild = TST-BalaneALevel(root->middleChild)
  TST-BalaneAllLevels(root->middleChild)
  TST-BalaneAllLevels(root->leftChild)
  TST-BalaneAllLevels(root->rightChild)
}

/*Performs right rotation on TST to preserve the BST order property
 *Input: root of subtree of unbalanced TST
 *Output: BST order property restored if the right subtree of a subtree
 *does not conform to the BST order property
*/
TST-RotateRight(TSTnode* root):
  nodeX <-- root->leftChild
  
  //Move the subtree mounted at root between nodeX and root
  root->leftChild = nodeX->rightChild
  //Swap nodeX and root
  nodeX->rightChild = root
  //Restore count field of root
  root->count = (root->leftChild ? root->leftChild->count : 0) +
			  (root->rightChild ? root->rightChild->count : 0) + 1
	nodeX->count = (nodeX->leftChild ? nodeX->leftChild->count : 0) +
			  nodeX->rightChild->count + 1
	
	return nodeX
END TST-RotateRight

/*Perform left rotation on TST to preserve the BST order property
 *Input: root of subtree of unbalanced TST
 *Output: BST order property restored if the left subtree of a subtree
 *does not conform to the BST order property
*/
TST-RotateLeft(TSTnode* root):
  nodeX <-- root->rightChild
	
	//Move the subtree mounted at root between nodeX and root
	root->rightChild = nodeX->leftChild
	//Swap nodeX and root
	nodeX->leftChild = root
	//Restore count field of root
	root->count = (root->leftChild ? root->leftChild->count : 0) +
			  (root->rightChild ? root->rightChild->count : 0) + 1;
	nodeX->count = nodeX->leftChild->count +
	      (nodeX->rightChild ? nodeX->rightChild->count : 0) + 1;
	
	return nodeX
END TST-RotateLeft

/***********************************************************************************************
 * COMPRESS TST TO FORM TST DAWG
 * ********************************************************************************************/

/*Compresses balanced TST to form TST DAWG
 *Input: root of balanced TST
 *Output: compressed TST or TST DAWG with duplicate suffixes removed
*/
TST-Compress():
  //Compute the hash field for all nodes except the root
  root->hash = TST-ComputeHash(root)
  //Remove duplicate suffixes beginning with the longest one
  TST-RemoveDuplicateSuffixes(root)
END TST-Compress

/*Computes the hash field for all nodes except the root
 *Input: root of balanced TST
 *Output: has field for each node in the balanced TST, save the root
*/
TST-ComputeHash(TSTnode* root):
  if root is NULL  then: return 0
  //Compute the hashes for the children of a node recursively and use them for the current hash
	root->hash = (root->symbol - 'a') + 31 * TST-ComputeHash(root->middleChild)
	root->hash ^= TST-ComputeHash(root->leftChild)
	root->hash ^= TST-ComputeHash(root->rightChild)
	root->hash ^= (root->hash >> 16)
	//Resulting hash field must be unsigned to facilitate the correct modulo computation
	root->hash %= HASH_TABLE_SIZE
	return root->hash
END TST-ComputeHash

/*Removes duplicate surfixes starting from the longest one
 *Input: root of balanced TST
 *Output: removal of nodes of balanced TST consisting of duplicate suffixes
*/
TST-RemoveDuplicates(TSTnode* root):
  if root->leftChild is not NULL then:
		//If the node already exists in the hash table, then TST-CheckAndRemoveDuplicateSuffixes
		//returns false: its children were checked for duplicates already, so there's no need to
		//check for duplicate children twice
		if TST-CheckAndRemoveDuplicates(&root->leftChild) is not NULL then:
			TST-RemoveDuplicates(root->leftChild)
	
	if root->rightChild is not NULL then:
		if TST-CheckAndRemoveDuplicates(&root->rightChild) is not NULL then:
			TST-RemoveDuplicates(root->rightChild)
	
	if root->middleChild is not NULL then:
		if TST-CheckAndRemoveDuplicates(&root->middleChild) is not NULL then:
			TST-RemoveDuplicates(root->middleChild)
END TST-RemoveDuplicates

/*Checks if a node of the balanced TST has been hashed
  *Input: root of balanced TST
  *Output: true if node is already hashed and subsequently removed; false, otherwise
*/
TST-CheckAndRemoveDuplicates(TSTnode* root):
  node <-- *root;
	hashIndex <-- node->hash
	
	while hashTable[hashIndex] is not equal to zero
	  do:
		  if TST-IsEqual(hashTable[hashIndex], node) is true then:
			//This node already exists in the hash table. So, remove the duplicate.
			freeNode(node)
			*root <-- hashTable[hashIndex]
			return false
		
		hashIndex <-- (hashIndex + 1) % HASH_TABLE_SIZE
	
	 hashTable[hashIndex] <-- node
	
	return true
END TST-CheckAndRemoveDuplicates

/***********************************************************************************************
 * SEARCHING TST DAWG
 * ********************************************************************************************/
 
 /*Searches for a given string in the TST DAWG
  *Input: root of TST DAWG; query string
  *Output: true if query string is found; false, otherwise
*/
TST-SEARCH(TSTnode* root, char* word):
  //root is null: word does not exist
  if root is equal to NULL then: return false
  
  //current char of string is less than char at the root: traverse left subtree of root
  if *word is less than root->symbol then:
    return TST-SEARCH(root->leftChild, word)
    
  //current char of string is greater than char at the root: traverse right subtree of root
  else if *word is greater than root->symbol:
    return TST-SEARCH(root->rightChild, word)
  
  //next char of string is equal to the char at the root: traverse middle subtree of root
  else if *(word + 1) is not equal to the NULL char then:
    return TST-SEARCH(root->middleChild, word + 1)
  
  //word is found
  else
      return root->isAWord
END TST-SEARCH

/***********************************************************************************************
 * SPELLING CORRECTOR
 * ********************************************************************************************/
 
/*Returns word if word is correct: that is found in dictionary; empty string
 *Input: search query
 *Output: search query or empty string
*/
TST-Correct(char* word):
  //search for word in TST DWAG: if found return word, else return empty string
  ( TST-Search(root, word) ) ? return word : return ""
END TST-Correct

/*Attempts to generate correct spelling suggestions if word is incorrect:
 *Input: search query, string vector
 *Output: correction suggestions, if any
*/
TST-Correct(const char* word, vector<char*> result):
  //do a first edit on word: that is, insertions, deletions, alterations, and transpositions
  TST-Edits(word, result)
  
  //check that strings in result are in TST DAWG
  for i <-- 1 to candidates.size() inclusive do:
    if TST-Search(root, result[i]) then:
      candidates.push_back(result[i])
      suggestions <-- true
  
  //at least a string in result is found in TST DAWG: display correction suggestions
  if suggestions is true then:
    display strings in candidates
  //no string in result is found in TST DAWG
  else
    do:
      //a second edit on word: that is, insertions, deletions, alterations, and transpositions
      for <-- 1 to result.size() inclusive do:
        TST-Edits(result[i], subResult)
        
      //check that strings in subResult are in TST DAWG
      for i <-- 1 to subResult.size() inclusive do:
        if TST-Search(root, subResult[i]) then:
          candidates.push_back(subResult[i])
          suggestions <-- true
          
      //if at least one string in subResult is found in TST DAWG: display correction suggestions
      if suggestions is true then:
        display strings in candidates
      
  //no string in subResult is found in TST DAWG
    display word is not found in TST DAWG
END TST-Correct

/*Performs four edits on a given string: insertions, deletions, alterations, and transpositions
 *Input: a string, and a vector
 *Output: a vector of edited strings
*/
TST-Edits(const char* word, vector<char*> result):
  //let n be the length of word
  
  //perform n deletions on word
  for index <-- 0 to word.length() exclusive do:
    result.push_back(word.substr(0, index) + word.substr(index + 1))
  
  //perform n-1 transpositions on word
  for index <-- 0 to word.length() - 1 exclusive do:
    result.push_back(word.substr(0, index) + word[index + 1] + word[index] + word.substr(index + 2))

  for j <-- 'a' to 'z' inclusive do:
    //perform 36n alterations on word
    for index <-- 0 to word.length() exclusive do:
      result.push_back(word.substr(0, index) + j + word.substr(index + 1))
    
    //perform 36(n+1) insertions on word
    for index <-- 0 to word.length() +  1 exclusive do:
      result.push_back(word.substr(0, index) + j + word.substr(index))
END TST-Edits