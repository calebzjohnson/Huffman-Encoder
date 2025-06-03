/*
 * TODO: File Header
 */

#include "phaseOne.h"
using namespace std;
#include <vector>
#include <iostream>
#include <sstream>


void count_freqs(istream &text) {
    int counts[256];
    for (int i = 0; i < 256; i++) {
        counts[i] = 0;
    }

    char currChar;
    while (text.get(currChar)) {
        counts[int(currChar)]++;
    }

    for (int i = 0; i < 256; i ++) {
        if (counts[i] > 0) {
            char toPrint = i;
            cout << toPrint << ": " << counts[i] << '\n';
        }
    }
}

string serialize_tree(HuffmanTreeNode *root) {
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

HuffmanTreeNode *deserialize_tree(const std::string &s) {
    istringstream serial(s);
    return deserialize_help(serial);
}

HuffmanTreeNode *deserialize_help(istream &in) {
    char currChar;
    if (in.get(currChar)) {
        if (currChar == 'L') {
            in.get(currChar);
            HuffmanTreeNode *newNode = new HuffmanTreeNode(currChar, 0);
            return newNode;
        } else {
            HuffmanTreeNode *newNode = new HuffmanTreeNode('\0', 0);
            newNode->set_left(deserialize_help(in));
            newNode->set_right(deserialize_help(in));
            return newNode;
        }
    } else {
        return nullptr;
    }
}