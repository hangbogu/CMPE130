#include <string>
#include <iostream>
using namespace std;

// the most things we can store in the table
const int MAX_SIZE = 600000;

class HashTable {
public:
	// set all linked lists to empty
	HashTable() {
		for (int i = 0; i < MAX_SIZE; i++) {
			inventory[i] = NULL;
		}
	}

	// insert a word
	void insert(string p) {
		// hash the word
		unsigned int index = hash(p);

		// make a new node at the start of this list
		Node* node = new Node;
		node->word = p;
		node->next = inventory[index];
		inventory[index] = node;

		cout << "Inserted " << p << " at " << index << endl;
	}

	void search(string word) {
		// hash the word
<<<<<<< HEAD
		unsigned int index = hash(word);
=======
		int index = hash(word);
>>>>>>> dac75fdb063db04665f5ace9753bc283d0c94937

		// look for them at this linked list
		Node* current = inventory[index];
		int number = 0;
		while (current->word != word ) {
			number++;
			current = current->next;
		}

		cout << "Found " << word << " in slot " << index << ", node " << number << endl;
	}

private:
	// a node in one of our linked lists
	struct Node {
		string word;
		Node* next;
	};

	// the words are all stored in an array
	Node* inventory[MAX_SIZE];
};
