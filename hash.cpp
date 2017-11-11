#include <string>
#include <iostream>
using namespace std;

// the most things we can store in the table
const int MAX_SIZE = 50000;

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
		unsigned int index = hash(word);

		// look for them at this linked list
		Node* current = inventory[index];
		int number = 0;
		while (current->word != word) {
			number++;
			current = current->next;
		}

		cout << "Found " << word << " in slot " << index << ", node " << number << endl;
	}

	unsigned int hash(string word) {
		int value = 0;
		// add up all the ASCII values
		for (unsigned int i = 0; i < word.length(); i++) {
			value = value * 31 + word[i];
		}

		// mod by size to prevent overflow
		return value % MAX_SIZE;
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

void main() {
	HashTable a;
	unsigned int number = 0;
	for (unsigned int i = 0; i < 25000; i++) {
		number = i;
		a.insert(to_string(i));
	}
	cout << "done!";
}