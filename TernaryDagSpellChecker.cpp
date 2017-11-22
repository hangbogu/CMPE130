#include "TernaryDagDictionary.h"

TernaryDagFileNodePtr getLeftChild(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary) {
  return dictionary + node->_leftChild;
}

TernaryDagFileNodePtr getRightChild(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary) {
  return dictionary + node->_rightChild;
}

TernaryDagFileNodePtr getMiddleChild(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary) {
  return dictionary + node->_middleChild;
}

bool isNullLink(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary) {
  return node == dictionary;
}

//Open Ternary DAG Dictionary, returns NULL in case of error
TernaryDagFileNodePtr openTernaryDagDictionary() {
  FILE* InputFile = fopen("TernaryDagDictionary.dat", "rb");
	
	if(!InputFile)
		return NULL;
	
	//Get file size
	fseek(InputFile, 0, SEEK_END);
	size_t fileSize = ftell(InputFile);
	fseek(InputFile, 0L, SEEK_SET);
	
	//Read the whole file to the allocated buffer
	TernaryDagFileNodePtr dictionary = (TernaryDagFileNodePtr)malloc(fileSize);
	if(dictionary && ( fread(dictionary, 1, fileSize, InputFile) != fileSize ||
				 0 != memcmp(dictionary, DAG_DICTIONARY_SIGNATURE, sizeof(TernaryDagFileNode)) ) ) {
		free(dictionary);
		dictionary = NULL;
	}
	
	fclose(InputFile);
	return dictionary;
}

//Close the dictionary
void closeTernaryDagDictionary(TernaryDagFileNodePtr dictionary) {
  free(dictionary);
}

//Returns true if the word is an exact match in the dictionary; false, otherwise
bool isExactMatch(TernaryDagFileNodePtr dictionary, const char* word) {
  if( findExactMatch(dictionary + 1, word, dictionary) )
		return true;
	
	//Convert the first letter to lower-case
	if( !isupper(*word) )
		return false;
	
	char lowerCase[MAX_WORD_LENGTH];
	char* lowCase = lowerCase;

	*lowCase++ = (char)tolower(*word++);

	//One-letter words should match case-sensitively
	if(*word == '\0')
		return false;
	
	// Two case variations are allowed: all capital letters (SOMETHING) or the first
	// capital letter (Something). Other combinations should match exactly (MHz).
	if ( isupper(*word) ) {
	  // Other letters must be capital, too
		*lowCase++ = (char)tolower(*word++);
		while(*word) {
			if (!isupper(*word) && *word != '\'')
				return false;
			*lowCase++ = (char)tolower(*word++);
		}
	} else {
	  // Other letters must be small
		*lowCase++ = *word++;
		while(*word) {
			if (!islower(*word) && *word != '\'')
				return false;
			*lowCase++ = *word++;
		}
	}
	*lowCase = '\0';
	return findExactMatch(dictionary + 1, lowerCase, dictionary);
}

//Returns true if the word is in the dictionary; false, otherwise
bool findExactMatch(TernaryDagFileNodePtr node, const char* word, TernaryDagFileNodePtr dictionary) {
  while(!isNullLink(node, dictionary)) {
		//Move to the left of right subtree
		if(*word < node->_symbol)
			node = getLeftChild(node, dictionary);
		else if(*word > node->_symbol)
			node = getRightChild(node, dictionary);
		//Found node with the character; begin searching for the next character
		else {
			if(*word == '\0' )
				return true;
			node = getMiddleChild(node, dictionary);
			word++;
		}
	}
	return false;
}
