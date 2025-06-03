/*
 * HuffmanCoder.cpp
 * Author: Caleb Johnson (cjohns34)
 * Date: 11/15/23
 * Purpose: Implementation of the HuffmanCoder class.
 */

#include "HuffmanCoder.h"
using namespace std;

#include <iostream>
#include <queue>
#include <utility>
#include <sstream>

/*
 * HuffmanCoder
 * Purpose: Default constructor for the class
 * Arguments: None
 * Return: None
 * Effects: None
 */
HuffmanCoder::HuffmanCoder() {
    encodings = new string[256];
    for (int i = 0; i < 256; i++) {
        encodings[i] = "";
    }
}

/*
 * ~HuffmanCoder
 * Purpose: Destructor for the HuffmanCoder class
 * Arguments: None
 * Return: None
 * Effects: Frees up all used space
 */
HuffmanCoder::~HuffmanCoder() {
    delete [] encodings;
}

/*
 * encoder
 * Purpose: Compress a given input file in binary and store that encoding and 
 * the corresponding Huffman Tree in a given output file
 * Arguments: Strings containing the text file being encoded and the file that
 * will hold the encoding and the created Huffman tree
 * Return: None
 * Effects: None
 */
void HuffmanCoder::encoder(const string &inputFile, const string &outputFile) {
    ifstream infile;
    open_and_check(infile, inputFile);

    int *counts = count_freqs(infile);
    if (check_empty_file(counts)) {
        cout << inputFile + " is empty and cannot be compressed.\n";
        return;
    }

    // make and serialize tree
    HuffmanTreeNode *root = make_tree(counts);
    string serialized = serialize_tree(root);

    // get encodings from tree
    get_encodings(root, "");
    close_file(infile);

    // create bitstring from encodings and write to zapped file
    string bitstring = get_bitstring(infile, inputFile);
    BinaryIO binary;
    binary.writeFile(outputFile, serialized, bitstring);
    cout << "Success! Encoded given text using " << bitstring.size() << 
    " bits.\n";
    
    // deallocate memory
    delete [] counts;
    delete_tree(root);
}

/*
 * decoder
 * Purpose: Decodes an encoding in a given input file and deserializes the given
 * serialized tree in the input file. Then writes the given encoding in english
 * to the given output file
 * Arguments: Strings containing the text file with the encoding and the file 
 * that will hold the decoding
 * Return: None
 * Effects: None
 */
void HuffmanCoder::decoder(const string &inputFile, const string &outputFile) {
    BinaryIO binary;
    pair<string, string> p = binary.readFile(inputFile);

    // create tree and get encodings
    istringstream serialized(p.first);
    HuffmanTreeNode *root = deserialize_tree(serialized);
    get_encodings(root, "");

    // decode bitstring using encodings and write decoded string to output file
    istringstream bitstring(p.second);
    string text = decode_bits(bitstring);
    write_output(text, outputFile);

    // deallocate memory
    delete_tree(root);
}

// HELPER FUNCTIONS
/* 
 * Name: open_and_check
 * Purpose: opens a given file and checks to make sure it opens properly
 * Argument: a reference to the streamtype of the file being read and a string
 * with the name of the file
 * Return: none
 * Effects: Throws a runtime error if the file does not open properly
 */
template<typename streamtype>
void HuffmanCoder::open_and_check(streamtype &stream, std::string file_name) {
    stream.open(file_name);
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file " + file_name);
    }
}

/*
 * Name: close_file
 * Purpose: closes a given file
 * Argument: a reference to the streamtype of the file being closed and the name
 * of the file being closed
 * Return: none
 * Effects: none
 */
template<typename streamtype>
void HuffmanCoder::close_file(streamtype &stream) {
    if (not stream.is_open()) {
        return;
    }
    stream.close();
}

/*
 * Name: count_freqs
 * Purpose: counts the frequencies of each character from a given input stream
 * Argument: a reference to the streamtype of the file being closed and the name
 * of the file being closed
 * Return: a pointer to an array of integers storing the counts of each 
 * character
 * Effects: none
 */
int *HuffmanCoder::count_freqs(std::istream &text) {
    int *counts = new int[256];
    for (int i = 0; i < 256; i++) {
        counts[i] = 0;
    }

    char currChar;
    while (text.get(currChar)) {
        counts[int(currChar)]++;
    }

    return counts;
}

/*
 * Name: make_tree
 * Purpose: Builds a Huffman Tree from a given array of character counts
 * Argument: A pointer to an array of ints that stores the counts of all 256
 * ASCII values
 * Return: A pointer to the root node of the created Huffman Tree
 * Effects: none
 */
HuffmanTreeNode *HuffmanCoder::make_tree(int *counts) {
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, NodeComparator> 
    nodeQueue;
    // add nodes with all used characters to the queue
    for (int i = 0; i < 256; i++) {
        if(counts[i] > 0) {
            HuffmanTreeNode *newNode = new HuffmanTreeNode(char(i), counts[i]);
            nodeQueue.push(newNode);
        }
    }
    // check for one character file
    if (nodeQueue.size() == 1) {
        HuffmanTreeNode *top = nodeQueue.top();
        nodeQueue.pop();
        HuffmanTreeNode *root = new HuffmanTreeNode('\0', top->get_freq(), 
        top, nullptr);
        nodeQueue.push(root);
    }

    // Huffman Algorithm
    while (nodeQueue.size() > 1) {
        HuffmanTreeNode *first = nodeQueue.top();
        nodeQueue.pop();
        HuffmanTreeNode *next = nodeQueue.top();
        nodeQueue.pop();
        HuffmanTreeNode *combined = new HuffmanTreeNode('\0', first->get_freq() 
        + next->get_freq(), first, next);
        nodeQueue.push(combined);
    }
    return nodeQueue.top();
}

/*
 * Name: serialize_tree
 * Purpose: Returns a string with the serialized version of a given Huffman Tree
 * Argument: A pointer to the root of the tree to be serialized
 * Return: A string with the serialized version of the tree
 * Effects: none
 */
string HuffmanCoder::serialize_tree(HuffmanTreeNode *root) {
    if (root == nullptr) {
        return "";
    } else if (root->isLeaf()) {
        string leafString = "";
        leafString+= root->get_val();
        return "L" + leafString;
    } else {
        return "I" + serialize_tree(root->get_left()) +
        serialize_tree(root->get_right());
    }
}

/*
 * Name: get_encodings
 * Purpose: Gets the encodings of all characters in a given tree and puts them
 * in the encodings array
 * Argument: A pointer to the root of the tree
 * Return: None
 * Effects: none
 */
void HuffmanCoder::get_encodings(HuffmanTreeNode *root, string e) {
    if (root == nullptr) {
        return;
    } else if (root->isLeaf()) {
        encodings[int(root->get_val())] = e;
    } else {
        get_encodings(root->get_left(), e + "0");
        get_encodings(root->get_right(), e + "1");
    }
}

/*
 * Name: get_bitstring
 * Purpose: Returns a string with the encoded version of the message in a given
 * input file
 * Argument: References to the input stream and input file that will be used to
 * read the given message
 * Return: A string with the encoded message
 * Effects: none
 */
string HuffmanCoder::get_bitstring(ifstream &infile, const std::string &file) {
    open_and_check(infile, file);
    string bitstring = "";
    char currChar;
    while (infile.get(currChar)) {
        bitstring+= encodings[int(currChar)];
    }
    close_file(infile);
    return bitstring;
}

/*
 * Name: deserialize_tree
 * Purpose: Creates and returns the root node of a huffman tree from a given
 * istream that can be read from to get the serialized tree
 * Argument: A reference to an istream that can be read from to get the
 * serialized tree
 * Return: A pointer to the root of the created tree
 * Effects: none
 */
HuffmanTreeNode *HuffmanCoder::deserialize_tree(istream &in) {
    char currChar;
    if (in.get(currChar)) {
        if (currChar == 'L') {
            in.get(currChar);
            HuffmanTreeNode *newNode = new HuffmanTreeNode(currChar, 0);
            return newNode;
        } else {
            HuffmanTreeNode *newNode = new HuffmanTreeNode('\0', 0);
            newNode->set_left(deserialize_tree(in));
            newNode->set_right(deserialize_tree(in));
            return newNode;
        }
    } else {
        return nullptr;
    }
}

/*
 * Name: decode_bits
 * Purpose: Decodes a given bitstring into english characters using Huffman
 * encodings
 * Argument: A reference to an istream that can be read from to get the
 * bitstring
 * Return: A string with the decoded text
 * Effects: none
 */
string HuffmanCoder::decode_bits(istream &in) {
    char currChar;
    string currBits = "";
    string decoded = "";
    while (in.get(currChar)) {
        currBits+= currChar;
        for (int i = 0; i < 256; i++) {
            if (currBits != "" and currBits == encodings[i]) {
                decoded+= char(i);
                currBits = "";
            }
        }
    }

    if (currBits != "") {
        throw runtime_error("Encoding did not match Huffman tree.");
    }

    return decoded;
}

/*
 * Name: write_output
 * Purpose: Writes a given decoded message to a given output file
 * Argument: A string containing the decoded message and a string with the name
 * of the output file
 * Return: None
 * Effects: none
 */
void HuffmanCoder::write_output(string text, string outputFile) {
    ofstream out;
    open_and_check(out, outputFile);
    int size = text.size();
    for (int i = 0; i < size; i++) {
        out << text[i];
    }
    close_file(out);
}

/*
 * Name: delete_tree
 * Purpose: Recursively deletes a given tree
 * Argument: A pointer to the root node of the tree being deleted
 * Return: None
 * Effects: none
 */
void HuffmanCoder::delete_tree(HuffmanTreeNode* node) {
    if (node != nullptr) {
        delete_tree(node->get_left());
        delete_tree(node->get_right());
        delete node;
    }
}

/*
 * Name: check_empty_file
 * Purpose: Checks if a given file is empty
 * Argument: A pointer to an array of integers that hold the frequencies of
 * every character in the file
 * Return: True if the file is empty, false otherwise
 * Effects: none
 */
bool HuffmanCoder::check_empty_file(int *counts) {
    for (int i = 0; i < 256; i++) {
        if (counts[i] != 0) {
            return false;
        }
    }
    return true;
}