#pragma once
#include<vector>
#include<list>
#include<string>

using namespace std;

//Hash function (based on the string folding method in OpenDSA Ch. 15.3.3)
unsigned int stringHash(string theString) {
	unsigned int hashValue = 0;
	for (unsigned int i = 0; i < theString.length(); i = i + 4) {
		//treat a group of 4 characters as a single long value, then add it to the sum
		for (unsigned int j = 0; j < 4; j++) {
			if (i + j < theString.length()) {
				hashValue += (unsigned int)pow(256, j) * theString.at(i + j);
			}
		}
	}
	return hashValue;
}

//**********************HashMap class declaration******************************//

template<typename T>
class HashMap {

	//Collisions are resolved with chaining. Each entry of the table stores a vector of key-value pairs.
	struct Table {
		vector<list<pair<string, list<T>>>> theTable;
		void insert(string key, T value); //insert a single key-value pair
		void insertAll(string key, list<T> valList); //insert a key with a list of all its values 
		list<pair<string, list<T>>> listAtKey(string key);
		int size();

		Table();
		Table(int size);

	};

	double loadFactor = 0.0;
	
	vector<string> keys;
	Table* hashTable;

	void resize();


public:
	HashMap();
	HashMap(int size);
	void insert(string key, T value);
	list<T> retrieve(string key); //If a value is not in the map, retrieve throws an out_of_range exception.
	vector<string>& getAllKeys();
	int bucketCount();
	void print(string key);
	void print();
};


//**********************Table function definitions******************************//

template<typename T>
HashMap<T>::Table::Table() {
	theTable = vector<list<pair<string, list<T>>>>(100); //default value for table size is 100
}

template<typename T>
HashMap<T>::Table::Table(int size) {
	theTable = vector<list<pair<string, list<T>>>>(size);
}

template<typename T>
void HashMap<T>::Table::insert(string key, T value) {
	int hashVal = stringHash(key) % theTable.size(); //calculate table index

	list <pair<string, list<T>>>& pairsAtValue = theTable.at(hashVal);
	bool wasFound = false;
	for (auto iter = pairsAtValue.begin(); iter != pairsAtValue.end(); iter++) {
		string keyToCheck = iter->first;
		if (keyToCheck == key) { //case 1: if the key is already in the table, push the value to insert onto the list of values associated with it
			(iter->second).push_back(value);
			wasFound = true;
		}
	}
	if (!wasFound) { //case 2: if the key is not already in the table, add in a new pair associated with it
		list<T> newKeyList;
		newKeyList.push_back(value);
		pair<string, list<T>> keyValPair = make_pair(key, newKeyList);
		theTable.at(hashVal).push_back(keyValPair);
	}
}

template<typename T>
void HashMap<T>::Table::insertAll(string key, list<T> valList) {
	int hashVal = stringHash(key) % theTable.size(); //calculate table index
	pair<string, list<T>> keyValPair = make_pair(key, valList);
	theTable.at(hashVal).push_back(keyValPair);
	
}

template<typename T>
list<pair<string, list<T>>> HashMap<T>::Table::listAtKey(string key) {
	return theTable.at(stringHash(key) % theTable.size());
}

template<typename T>
int HashMap<T>::Table::size() {
	return theTable.size();
}

//**********************HashMap function definitions******************************//

template<typename T>
HashMap<T>::HashMap() {
	hashTable = new Table();
}

template<typename T>
HashMap<T>::HashMap(int size) {
	hashTable = new Table(size);
}

template<typename T>
void HashMap<T>::insert(string key, T value) {
	hashTable->insert(key, value);
	keys.push_back(key);
	double loadFactor = (double)keys.size() / (double)(hashTable->size());
	if (loadFactor > 0.7) this->resize();
}

template<typename T>
list<T> HashMap<T>::retrieve(string key) {
	list<pair<string, list<T>>> pairList = hashTable->listAtKey(key);
	for (auto iter = pairList.begin(); iter != pairList.end(); iter++) {
		pair<string, list<T>> thePair = *iter;
		if (thePair.first == key) return thePair.second; //check whether the pair actually corresponds to the given key (as opposed to another key that hashes to the same value)
	}
	throw std::out_of_range("Key not found");
}

template<typename T>
void HashMap<T>::resize() {
	Table* newTable = new Table(hashTable->size() * 2);
	Table* tmpTable = hashTable;

	keys.clear(); //Reset the vector of keys, as each key will be added again when the pairs are inserted into the new table

	for (int i = 0; i < tmpTable->size(); i++) { //Recalculate the hash function and insert each key-value pair into the new table
		list<pair<string, list<T>>> tableEntry = hashTable->theTable.at(i);
		for (auto iter = tableEntry.begin(); iter != tableEntry.end(); iter++) { //Iterate through all key-value pairs that hash to i
			newTable->insertAll(iter->first, iter->second);
			keys.push_back(iter->first);
		}
	}
	hashTable = newTable;
	delete tmpTable;
}

template<typename T>
int HashMap<T>::bucketCount() {
	return hashTable->size();
}

template<typename T>
vector<string>& HashMap<T>::getAllKeys() {
	return keys;
}

//Print functions(for debugging)
template<typename T>
void HashMap<T>::print(string key) {
	cout << "Keyword: " << key << "(hashes to " << stringHash(key) % hashTable->size() << ")" << endl; //print the key
	list<T> vals = this->retrieve(key);
	cout << "List of values corresponding to this key: ";
	for (auto iter = vals.begin(); iter != vals.end(); iter++) { //iterate through the vector of pairs stored at the hash value of the key
		cout << *iter << " ";
	}
	cout << endl;
}

template<typename T>
void HashMap<T>::print() {
	cout << "Bucket count: " << this->bucketCount() << endl;
	cout << "Load factor: " << (double)keys.size() / (double)this->bucketCount() << endl;
	for (string s : keys) {
		this->print(s);
	}
}