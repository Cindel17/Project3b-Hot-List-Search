#pragma once
#include<unordered_map>
#include<string>
#include<queue>
using namespace std;

//The search and insert functions are more efficient when called an a queue of characters as opposed to a string, as the first letter is removed from the key with every recursive call.
queue<char> createStringQueue(string theString) {
	queue<char> theQueue;
	for (char c : theString) theQueue.push(c);
	return theQueue;
}

template<typename T>
class TrieNode {
	char letter;
	T data;
	bool isLeaf;
	unordered_multimap<char, TrieNode*> children;
public:
	TrieNode();
	T getData();
	TrieNode* search(string key); //search for the given string, treating the current node as the root. Returns a pointer to the required node if found, returns nullptr if not found.
	void insert(string key, T data);
	TrieNode* searchQueue(queue<char>& key);
	void insertQueue(queue<char>& key, T data);
};

template<typename T>
TrieNode<T>::TrieNode() {
	letter = '-';
	isLeaf = true;
	data = T();
}

template<typename T>
T TrieNode<T>::getData() {
	return data;
}

template<typename T>
void TrieNode<T>::insertQueue(queue<char>& key, T data) {

	//Base case: The key length is 1. Insert a new leaf node with the given letter as one of root's children.
	if (key.size() == 1) {
		TrieNode* newNode = new TrieNode();
		newNode->letter = key.front();
		newNode->data = data;
		newNode->isLeaf = true;
		this->children.emplace(key.front(), newNode);
	}
	//If the key length is not 1, insert the new node as an intermediate node.
	else {
		char theLetter = key.front();
		TrieNode* nextNode = nullptr; //nextNode will be the node used in the next recursive call.

		//Case 1: The root node already has a child corresponding to the given letter.
		auto iter = this->children.find(theLetter);
		if (iter != this->children.end()) {
			nextNode = iter->second; 
		}
		//Case 2: The root node does not have a child corresponding to the given letter, so create one.
		else {
			nextNode = new TrieNode();
			nextNode->letter = theLetter;
			nextNode->isLeaf = false;
			this->children.emplace(theLetter, nextNode);
		}

		key.pop(); //remove the first letter
		nextNode->insertQueue(key, data);

	}
}

template<typename T>
TrieNode<T>* TrieNode<T>::searchQueue(queue<char>& key) {
	
	char theLetter = key.front();

	//Base case: The key length is 1, in which case the value is in one of the root's children if it exists in the tree at all.
	if (key.size() == 1) {
		auto iter = this->children.find(theLetter);
		if (iter != this->children.end()) {
			if (iter->second->isLeaf == true) return iter->second;
			else return nullptr;
		}
		else return nullptr;
	}
	else {
		auto iter = this->children.find(theLetter);
		if (iter == children.end()) return nullptr; //If the first letter is not one of root's children, we know the value is not in the tree.
		else {
			TrieNode* nextNode = iter->second;
			key.pop(); //Similar to the insert function, remove the first letter and recursively search.
			return nextNode->searchQueue(key);
		}
	}
}

template<typename T>
void TrieNode<T>::insert(string key, T data) {
	queue<char> theQueue = createStringQueue(key);
	queue<char>& qRef = theQueue;
	this->insertQueue(qRef, data);
}

template<typename T>
TrieNode<T>* TrieNode<T>::search(string key) {
	queue<char> theQueue = createStringQueue(key);
	queue<char>& qRef = theQueue;
	return this->searchQueue(qRef);
}
