

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#include "spellchecker.h"

#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif

// TST node in memory
static int g_count = 0;

struct ST_MEM_NODE {
	char ch;
	unsigned count;
	unsigned hash;
	unsigned no;
	struct ST_MEM_NODE *left;
	struct ST_MEM_NODE *middle;
	struct ST_MEM_NODE *right;
};
typedef struct ST_MEM_NODE ST_MEM_NODE;

// ==================================
// Tree creation
// ==================================

// Create a new node for character
static  ST_MEM_NODE* new_node(char ch) {
	ST_MEM_NODE* node = (ST_MEM_NODE*)malloc(sizeof(ST_MEM_NODE));
	if (!node) {
		puts("Out of memory\n");
		abort();
	}
	node->ch = ch;
	node->left = node->middle = node->right = NULL;
	g_count++;
	return node;
}

// Recursively free the subtree
static void free_node(ST_MEM_NODE* node) {
	if(!node)
		return;
	free_node(node->left);
	free_node(node->right);
	free_node(node->middle);
	free(node);
	g_count--;
}

static ST_MEM_NODE* g_root = NULL;

// Insert a string in subtree
static ST_MEM_NODE* insert_node(ST_MEM_NODE* node, char* str) {
	// If the node with character does not exist, insert it
	if(node == NULL)
		node = new_node(*str);
		
	// Look for the character in the left or right subtree
	if(*str < node->ch)
		node->left = insert_node(node->left, str);
	else if(*str > node->ch)
		node->right = insert_node(node->right, str);
	
	// If found the right character, insert the next character
	else {
		if(*str != '\0')
			node->middle = insert_node(node->middle, str + 1);
	}
	return node;
}

static void ST_insert(char* str) {
	g_root = insert_node(g_root, str);
}


// ====================================
// Balance BSTs forming the TST
// ====================================

// Set the number of child nodes on the same level
static unsigned set_count(ST_MEM_NODE* node) {
	if(NULL == node)
		return 0;
	node->count = set_count(node->left) + set_count(node->right) + 1;
	set_count(node->middle);
	return node->count;
}

static ST_MEM_NODE* rotate_right(ST_MEM_NODE* node) {
	ST_MEM_NODE *x = node->left;
	// Move the subtree between x and node
	node->left = x->right;
	// Swap x and node
	x->right = node;
	// Restore count field
	node->count = (node->left ? node->left->count : 0) +
			  (node->right ? node->right->count : 0) + 1;
	x->count = (x->left ? x->left->count : 0) +
			x->right->count + 1;
	return x;
}

static ST_MEM_NODE* rotate_left(ST_MEM_NODE* node) {
	ST_MEM_NODE *x = node->right;
	// Move the subtree between x and node
	node->right = x->left;
	// Swap x and node
	x->left = node;
	// Restore count field
	node->count = (node->left ? node->left->count : 0) +
			  (node->right ? node->right->count : 0) + 1;
	x->count = x->left->count +
		  (x->right ? x->right->count : 0) + 1;
	return x;
}

static ST_MEM_NODE* divide(ST_MEM_NODE* node, int dividing_count) {
	int left_count = (node->left == 0) ? 0: node->left->count;
	// If the dividing node is in the left subtree, go down to it
	if(dividing_count < left_count) {
		node->left = divide(node->left, dividing_count);
		// On the way back from the dividing node to the root, do right rotations
		node = rotate_right(node);
	}
	// Ditto for the right subtree
	else if(dividing_count > left_count) {
		node->right = divide(node->right, dividing_count - left_count - 1);
		node = rotate_left(node);
	}
	return node;
}

// Balance a level of TST (which is a BST)
static ST_MEM_NODE* balance_level(ST_MEM_NODE* node) {
	if(node == NULL || node->count == 1)
		return node;
	// Make middle node the root
	node = divide(node, node->count / 2);
	// Balance subtrees recursively
	node->left = balance_level(node->left);
	node->right = balance_level(node->right);
	return node;
}

// Balance the whole TST
static void balance_tree(ST_MEM_NODE* node) {
	if(node == NULL)
		return;
	node->middle = balance_level(node->middle);
	balance_tree(node->middle);
	balance_tree(node->left);
	balance_tree(node->right);
}

void ST_balance(void) {
	set_count(g_root);
	g_root = balance_level(g_root);
	balance_tree(g_root);
}

// ===================
// Compress TST to DAG
// ===================
static bool equal(ST_MEM_NODE* h1, ST_MEM_NODE* h2) {
	if(h1 == NULL || h2 == NULL) {
		return h1 == NULL && h2 == NULL;
	}
	// Two nodes are equal if their characters and their children are equal
	return h1->ch == h2->ch &&
		equal(h1->left, h2->left) && equal(h1->middle, h2->middle) &&
		equal(h1->right, h2->right);
}

#define TABLE_SIZE 262144
static ST_MEM_NODE* g_table[TABLE_SIZE];

static unsigned calculate_hash(ST_MEM_NODE* node) {
	if(node == NULL)
		return 0;
	// Calculate the hashes for children recursively and use them for the current hash
	node->hash = (node->ch - 'a') + 31 * calculate_hash(node->middle);
	node->hash ^= calculate_hash(node->left);
	node->hash ^= calculate_hash(node->right);
	node->hash ^= (node->hash >> 16);
	// hash must be unsigned for the correct modulo calculation
	node->hash %= TABLE_SIZE;
	return node->hash;
}

// Find the node in hash table. If it does not exists, add a new one and return true.
// If it does exists, return false.
static bool check_and_remove_duplicate(ST_MEM_NODE** node_ptr) {
	ST_MEM_NODE * node = *node_ptr;
	unsigned hash = node->hash;
	while(g_table[hash] != 0) {
		if(equal(g_table[hash], node)) {
			// This node already exists in the table. Remove the duplicate
			free_node(node);
			*node_ptr = g_table[hash];
			return false;
		}
		hash = (hash + 1) % TABLE_SIZE;
	}
	g_table[hash] = node;
	return true;
}

// Remove duplicating suffixes starting from the longest ones
static void remove_duplicates(ST_MEM_NODE* node) {
	if(node->left) {
		// If the node already exists in the table (check_and_remove_duplicate returned false),
		// its children were checked for duplicates already, so we don't check them twice
		if(check_and_remove_duplicate(&node->left))
			remove_duplicates(node->left);
	}
	if(node->right) {
		if(check_and_remove_duplicate(&node->right))
			remove_duplicates(node->right);
	}
	if(node->middle) {
		if(check_and_remove_duplicate(&node->middle))
			remove_duplicates(node->middle);
	}
}

void ST_remove_duplicates(void) {
	calculate_hash(g_root);
	remove_duplicates(g_root);
}

void ST_free(void) {
	// Remove all nodes in the hash table
	for(int i = 0; i < TABLE_SIZE; ++i)
		if(g_table[i])
			free(g_table[i]);
	free(g_root);
}

// ===================================
// Dump the hash table (for debugging)
// ===================================

static char g_dump_buff[ST_MAX_WORD];

static void dump_tree(ST_MEM_NODE * n, char * next) {
	do {
		if (n->left)
			dump_tree(n->left, next);
		if (n->right)
			dump_tree(n->right, next);
		*next++ = n->ch;
		n = n->middle;
	} while(n);
	printf("%s\n", g_dump_buff);
}

void ST_dump(void) {
	for(int i = 0; i < TABLE_SIZE; ++i)
		if(g_table[i])
		{
			dump_tree(g_table[i], g_dump_buff);
			printf("\n");
		}
}

// ==========================
// Save the DAG into a file
// ==========================

// Set numbers for all nodes
static unsigned set_no(void) {
	// Root nodes is not in the hash table, so handle it separately
	g_root->no = 1;
	
	// Set the numbers for other nodes
	unsigned no = 2;
	for(int i = 0; i < TABLE_SIZE; ++i)
		if(g_table[i])
			g_table[i]->no = no++;
	return no;
}

static unsigned short get_link_value(ST_MEM_NODE* node) {
	const unsigned short NULL_LINK = 0;
	if(node) {
		assert(node->no <= 0xFFFF);
		return (unsigned short)node->no; // Use node number instead of the pointer
	}
	else
		return NULL_LINK; // Null pointer
}

static void create_file_node(ST_FILE_NODE* file_node, const ST_MEM_NODE* node) {
	// Copy information from the memory node to a file node
	file_node->ch = node->ch;
	file_node->left = get_link_value(node->left);
	file_node->right = get_link_value(node->right);
	file_node->middle = get_link_value(node->middle);
	file_node->_padding = 0;
}

static void write_to_buff(ST_FILE_NODE* buff) {
	// Set the zero node (signature and, at the same time, a null pointer)
	memcpy(buff, ST_DICT_SIGNATURE, sizeof(ST_FILE_NODE));
	
	// Convert the root node
	create_file_node(buff + g_root->no, g_root);
	
	// Convert other nodes
	for(int i = 0; i < TABLE_SIZE; ++i)
		if(g_table[i])
			create_file_node(buff + g_table[i]->no, g_table[i]);
}

bool ST_write_to_file(void) {
	// Set the node numbers and calculate the total count
	unsigned nodes_count = set_no();

	// Allocate memory for the file content
	ST_FILE_NODE* file_data = (ST_FILE_NODE*)malloc(nodes_count * sizeof(ST_FILE_NODE));
	if (!file_data) {
		puts("Out of memory\n");
		abort();
	}
	write_to_buff(file_data);
	
	// Write to the file
	FILE* f = fopen("spellchk.dat", "wb");
	bool res = false;
	if(f) {
		if(fwrite(file_data, sizeof(ST_FILE_NODE), nodes_count, f) == nodes_count)
			res = true;
	}
	free(file_data);
	return res;
}

// ====================
// Main function
// ====================

int main() {
#ifdef _DEBUG
	_CrtSetDbgFlag( // Find memory leaks
	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |
	_CRTDBG_LEAK_CHECK_DF
	);
#endif

	// Read the word list and add the words to TST
	FILE* dic = fopen("Downloads/dic.txt", "r");
	if (!dic) {
		puts("Open dictionary failed\n");
		return 1;
	}
	while(!feof(dic)) {
		char line[ST_MAX_WORD];
		fgets(line, sizeof(line), dic);
		// Skip trailing CRLF
		size_t i = strcspn(line, "\r\n");
		line[i] = '\0';
		ST_insert(line);
	}
	fclose(dic);

	printf("Count: %d\nBalancing...\n", g_count);
	ST_balance();
	puts("Removing duplicates...\n");
	ST_remove_duplicates();
	//ST_dump();
	printf("Without duplicates: %d\n", g_count);
	puts("Saving file...\n");
	if(!ST_write_to_file()) {
		puts("Saving file failed\n");
		return 1;
	}
	ST_free();
	return 0;
}
