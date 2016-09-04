// File hashDriver.cpp:
//
// Brian Taylor
// 52311859
// z2b0b
// Lab L0D

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define TABLE_SIZE 59          // default table size for empty tables
#define TOMBSTONE "^^RIP^^^"
#define MAX_CHARS_PER_LINE 512
#define MAX_TOKENS_PER_LINE 2
#define DELIMITER " "

int numprobes;

// ----- Helper Functions -----

// modified from an algorithm written by Francesco Balena
// d/l from http://www.devx.com/vb2themax/Tip/19051
// pre:  x is a positive integer
// post: return true if x is prime, false otherwise
const bool isPrime( unsigned int x) {
    if (x == 1) return false;
    if (x == 2 || x == 3 || x == 5) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    unsigned int incr = 4;
    unsigned int maxFact = (unsigned int) sqrt( x );
    for (int fact = 5; fact <= maxFact; fact += incr) {
        if (x % fact == 0) return false;
        incr = 6 - incr;
    }
    return true;
}

// Modular exponentiation -- used by goodHash()
// pre:  base is an integer > 1, exponent is an integer > 0, modulus is an integer > 1
// post: function returns base^exponent mod modulus
int modular_pow(int base, int exponent, int modulus) {
    int c = 1;
    for (int e_prime = 1; e_prime <= exponent; e_prime++)
        c = (c * base) % modulus;
    return c;
}

// pre:  filename is a valid file name (including path, if necessary)
//       requires non-empty file
// post: returns the number of lines in the file
int file_size(string filename) {
    // create a file-reading object
    ifstream infile;
    infile.open(filename.c_str()); // open a file
    if (!infile.good()) {
        cout << filename << " not found." << endl;
        exit (EXIT_FAILURE);  // exit if file not found
    }
    
    int numlines = 0;
    // read each line of the file
    while (!infile.eof()) {
        // read an entire line into memory
        char buffer[MAX_CHARS_PER_LINE];
        infile.getline(buffer, MAX_CHARS_PER_LINE);
        numlines++;
    }
    infile.close();
    return numlines - 1;
}

// ------ Hash Functions ------

// pre:  key is a string of any length, size is a positive integer
// post: returns the sum of the ASCII values of the characters of key modulus the
//         hashTableSize
int poorHash(string key, int size) {
    int index = 0;
    for (int i = 0; i < key.length(); i++) {
        index += (int) key.at(i);
    }
    return index % size;
}

// pre:  key is a string of any length, size is a positive integer
// post: returns an index based on the function described on page 532 of
//       Objects, Abstraction, Data Structures and Design: Using C++
int goodHash(string key, int size) {
    int hash = 0;
    int length = (int)key.length();
    for (int i = 0; i < length; i++) {
        hash = ( hash + (int) key.at(i) * modular_pow(31, length - i - 1, size) );
    }
    return hash % size;
}

// An implementation of the djb2 hash function, as described at www.cse.yorku.ca/~oz/hash.html
// pre:  key is a string of any length, size is a positive integer
// post: returns an index based on the djb2 hash function
int djb2Hash(string key, int size) {
    unsigned long hash = 5381;
    for (int i = 0; i < key.length(); i++) {
        hash = hash * 33 + (int)key.at(i);
    }
    return hash % size;
}

// An implementation of the sdbm hash function, as described at www.cse.yorku.ca/~oz/hash.html
// pre:  key is a string of any length, size is a positive integer
// post: returns an index based on the sdbm hash function
int sdbmHash(string key, int size) {
    unsigned long hash = 0;
    for (int i = 0; i < key.length(); i++) {
        hash = (int)key.at(i) + (hash << 6) + (hash << 16) - hash;
    }
    return hash % size;
}





class TableEntry {
private:
    string key;
    int value;
public:
    TableEntry(string key, int value) {
        this->key = key;
        this->value = value;
    }
    
    string getKey() { return key; }
    int getValue() { return value; }
    
};



class Hasher {
private:
    TableEntry** table;
    
    char hash_function;
    char probe_type;
    int numkeys;
    int size;
    bool full;
    
public:
    
    Hasher(char type, char crp);
    
    Hasher(char type, char crp, float load_factor, string filename);
    
    bool search(string key, int& subscript);
    
    bool search(string key, int& subscript, char intent);
    
    bool insert(string key, int value);
    
    bool remove(string key);
    
    void populateTable(string filename);
    
    bool isFull() { return full; }
    
    int getSize() { return size; }
    
    int getNumkeys() { return numkeys; }
    
    void printTable();
    
    void printFullTable();
    
    // Destructor-- do not alter.
    ~Hasher() 
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL)
                delete table[i];
        delete[] table;
    }
    
};

// constructor for empty table of default size
// pre:  type must be one of 'g' or 'b'
//       crp must be one of 'd' or 'q'
// post: a hash table of the default size is created and initialized
Hasher::Hasher(char type, char crp) {
    table = new TableEntry* [TABLE_SIZE];
    
    hash_function = type;
    probe_type = crp;
    numkeys = 0;
    full = false;
    
    size = TABLE_SIZE;
    
    cout << "initializing empyty hash table" << endl;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = new TableEntry("", 0);
    }
}

// constructor for table from file with given load factor
// pre:  type is one of 'g' or 'b'
//       crp is one of 'd' or 'q'
//       load_factor is between 0 and 1
//       filename is a valid file with records to populate the hash table
// post: a hash table of the appropriate size is created so that the load factor
//       is as given, and it's populated with the records from a file
Hasher::Hasher(char type, char crp, float load_factor, string filename) {
    
    hash_function = type;
    probe_type = crp;
    numkeys = 0;
    full = false;
    
    // determine how many records are in the file
    int filesize = file_size(filename);
    
    // determine the appropriate table size given the load factor
    size = (int)(filesize / load_factor);
    cout << "number of records in the file: " << filesize << endl;
    cout << "initial table size: " << size << endl;
    
    // make it slightly bigger so that it's a prime
    while (!(isPrime(size))) {
        size++;
    }
    cout << "new prime table size: " << size << endl;
    
    // creat the table
    table = new TableEntry* [size];
    
    // initialize the table
    for (int i = 0; i < size; i++) {
        table[i] = new TableEntry("" , 0);
    }
    
    // populate the table with records from the file
    populateTable(filename);
}

// pre:  requires a valid filename (including path if necessary)
// post: populates the table with the keys and values from the file
void Hasher::populateTable(string filename) {
    ifstream infile;
    infile.open(filename.c_str());             // open the file
    
    while (!infile.eof())              // read each line of the file
    {
        // read an entire line into memory
        char buffer[MAX_CHARS_PER_LINE];
        infile.getline(buffer, MAX_CHARS_PER_LINE);
        
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the tokens in buf
        const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
        
        // parse the line
        token[0] = strtok(buffer, DELIMITER); // first token
        if (token[0]) // zero if line is blank
        {
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER); // subsequent tokens
                if (!token[n]) break; // no more tokens
            }
        }
        if (!infile.eof())
            insert(token[0], atoi(token[1]));    // insert into the hash table
    }
}

// a basic search is assumed if only two parameters are used
// same pre and post conditions as search with three parameters
bool Hasher::search(string key, int& subscript) {
    return search(key, subscript, 's');
}

// pre:  key is a string to be searched for in the hash table
//       subscript just needs to be an integer
//       intent is 's' for a simple search, and 'i' for an insertion search
// post: returns true if key was successfully found, false otherwise
//       subscript is modified to reflect the index of the key that was found
//       numprobes is modified to reflect the number of probes that were required
bool Hasher::search(string key, int& subscript, char intent) {
    int index;
    if (hash_function == 'g')
        index = goodHash(key, size);
    else
        index = poorHash(key, size);
    
    numprobes = 1;
    int increment = djb2Hash(key, size);  // to be used in the double hashing
    if (increment == 0)
        increment++;                      // obviously an increment of 0 won't be good
    
    while (true) {
        // four options -- we find either:
        //      a) the key we're looking for
        //      b) a different key
        //      c) an empty slot
        //      d) a tombstone
        // actions vary depending on intent
        
        // found what we were looking for
        // in either case ('s' or 'i') we stop here and return true
        // if it's a search for insertion purposes it means we're updating a key
        if (table[index]->getKey() == key) {
            subscript = index;
            return true;
        }
        
        // found an empty slot
        if (table[index]->getKey() == "") {
            if (intent == 's')
                return false;
            else {
                subscript = index;
                return true;
            }
        }
        
        // found a tombstone
        if (table[index]->getKey() == TOMBSTONE) {
            if (intent == 'i') {
                subscript = index;
                return true;
            }
        }
        
        // if we get this far it means we have to keep looking
        
        // is table full or probing sequence repeating itself?
        
        if (numprobes >= 2 * size) {
            if (intent == 's')
                return false;
            else {
                full = true;
                cout << "Table is full or essentially full" << endl;
                return false;
            }
        }
        
        // get next index to inspect
        if (probe_type == 'q')
            // quadratic probing
            index = (index + numprobes * 2 - 1) % size;
        else
            // double hashing
            index = (index + increment) % size;
        
        numprobes++;
    }
}

// pre:  key must be a string, value an integer
// post: if there is room the key, value pair is inserted according to the
//       hashing rules for the particular table
//       returns true is successfully inserted, false otherwise
//       outputs a statement if the table becomes full
bool Hasher::insert(string key, int value) {
    
    if (full)
        return false;
    
    int index = -1;
    
    if (search(key, index, 'i')) {
        cout << "inserting ..." << endl;
        table[index] = new TableEntry(key, value);
        cout << "index: " << index << " key: " << table[index]->getKey() << " value: " << table[index]->getValue() << endl;
        cout << "numprobes: " << numprobes << endl;
        numkeys++;
        
        if (numkeys == size) {
            full = true;
            cout << "Table is now full." << endl;
        }
        
        return true;
    }
    return false;
}

// pre:  key is a string
// post: removes key and returns true, if key was actually in the table
//       returns false otherwise
bool Hasher::remove(string key) {
    int index = -1;
    
    if (search(key, index, 's')) {
        
        table[index] = new TableEntry(TOMBSTONE, 0);
        
        cout << "removing " << key << " from index " << index << endl;
        numkeys--;
        full = false;
        return true;
    }
    return false;
}


// Simply outputs the current contents of the table and the indices
// E.g.
//    table->printTable() might generate:
//        25  HBZEJKGA   1
//        32  RHJMIVTA   2
//
void Hasher::printTable() {
    cout << endl << "----- Table -----" << endl;
    for (int i = 0; i < size; i++) {
        if (table[i]->getKey() != "" && table[i]->getKey() != TOMBSTONE)
            cout << i << "  " << table[i]->getKey() << "  " << table[i]->getValue() << endl;
    }
}

// prints out table with empty spots and tombstones (easier for debugging)
void Hasher::printFullTable() {
    cout << endl << "----- Full Table -----" << endl;
    for (int i = 0; i < size; i++) {
        cout << i << "  " << table[i]->getKey() << "  " << table[i]->getValue() << endl;
    }
}




int main( int argc, char* argv[])
{
    // choose which test case to run:
    int test = 3;
    
    if (test == 1) {
        string key;
        
        // Generate empty hash tables:
        Hasher* goodHashRP1 = new Hasher('g', 'd');
        cout << "numkeys: " << goodHashRP1->getNumkeys() << " size: " << goodHashRP1->getSize() << endl << endl;
        Hasher* goodHashQP1 = new Hasher('g', 'q');
        cout << "numkeys: " << goodHashQP1->getNumkeys() << " size: " << goodHashQP1->getSize() << endl << endl;
        Hasher* poorHashRP1 = new Hasher('b', 'd');
        cout << "numkeys: " << poorHashRP1->getNumkeys() << " size: " << poorHashRP1->getSize() << endl << endl;
        Hasher* poorHashQP1 = new Hasher('b', 'q');
        cout << "numkeys: " << poorHashQP1->getNumkeys() << " size: " << poorHashQP1->getSize() << endl << endl;
        
        
        string keys[4];
        keys[0] = "ABCDEFGH";
        keys[1] = "BBCDEFGH";
        keys[2] = "CBCDEFGH";
        keys[3] = "DBCDEFGH";
        
        
        for (int i = 0; i < 4; i++) {
            if(goodHashRP1->insert(keys[i], i))
                cout << "Inserted" << endl;
            else
                cout << "Failed to insert" << endl;
            
            if(goodHashQP1->insert(keys[i], i))
                cout << "Inserted" << endl;
            else
                cout << "Failed to insert" << endl;
            
            if(poorHashRP1->insert(keys[i], i))
                cout << "Inserted" << endl;
            else
                cout << "Failed to insert" << endl;
            
            if(poorHashQP1->insert(keys[i], i))
                cout << "Inserted" << endl;
            else
                cout << "Failed to insert" << endl;
        }
        
        cout << "Table goodHashRP1: " << endl;
        goodHashRP1->printTable();
        cout << "Table goodHashQP1: " << endl;
        goodHashQP1->printTable();
        cout << "Table poorHashRP1: " << endl;
        poorHashRP1->printTable();
        cout << "Table poorHashRP1: " << endl;
        poorHashQP1->printTable();
    }
    
    if (test == 2) {
        
        // Generate hash tables that are systematically loaded from file.
        // Note that if you cannot fit an element you should stop inserting elements
        // and set a flag to full.
        string filename = "./small.txt";
        
        Hasher* goodHashRP2 = new Hasher('g', 'd', 0.25, filename);
        Hasher* goodHashQP2 = new Hasher('g', 'q', 0.50, filename);
        Hasher* poorHashRP2 = new Hasher('b', 'd', 0.75, filename);
        Hasher* poorHashQP2 = new Hasher('b', 'q', 0.25, filename);
        
        cout << endl << "Table goodHashRP2: " << endl;
        goodHashRP2->printTable();
        cout << endl << "Table goodHashQP2: " << endl;
        goodHashQP2->printTable();
        cout << endl << "Table poorHashRP2: " << endl;
        poorHashRP2->printTable();
        cout << endl << "Table poorHashQP2: " << endl;
        poorHashQP2->printTable();
        
    }
    
    if (test == 3) {
        
        // create a random key, insert it, count the number of probes, remove it and repeat
        
        string randkey;
        int totalprobes = 0;
        int number_of_insertions = 100;
        float average = 0.0;
        
        string filename = "./medium.txt";
        
        // To get the different results, as needed:
        // first param:  choose 'g' for good hash or 'b' for poor hash
        // second param: choose 'q' for quadratic probing or 'd' for double hashing
        // third param:  between zero and one for load factor
        // fourth param: change filename above
        Hasher* goodHashTest1 = new Hasher('g', 'q', 0.5, filename);
        
        cout << endl << endl;
        
        for (int j = 0; j < number_of_insertions; j++) {
            
            randkey = "";
            for (int i = 0; i < 8; i++) {              // create the 8-letter key
                randkey = randkey + (char) (rand() % 26 + 65);
            }
            
            goodHashTest1->insert(randkey, j);
            
            totalprobes += numprobes;
            
            average = (float)totalprobes / (float)(j+1);
            cout << "average number of probes per insertion: " << average << endl;
            
            goodHashTest1->remove(randkey);
        }
        
        goodHashTest1->printFullTable();
        
        cout << "totalprobes: " << totalprobes << endl;
        average = (float)totalprobes / (float)number_of_insertions;
        cout << "average number of probes per insertion: " << average << endl;
        cout << "numkeys: " << goodHashTest1->getNumkeys() << " size: " << goodHashTest1->getSize() << endl;
        
    }
    
    return 0;
}