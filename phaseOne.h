/*
 * TODO: File Header
 */

#include <istream>
#include <string>

#include "HuffmanTreeNode.h"

void count_freqs(std::istream &text);
std::string serialize_tree(HuffmanTreeNode *root);
HuffmanTreeNode *deserialize_tree(const std::string &s);
HuffmanTreeNode *deserialize_help(std::istream &in);