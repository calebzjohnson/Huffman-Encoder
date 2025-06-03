/*
 * unit_tests.h
 * Author: Caleb Johnson (cjohns34)
 * Date: 11/14/23
 *  Purpose: This file uses the unit_test framework to test my implementation of
 *  the phase one functions count_freqs, serialize_tree and deserialize_tree.
 */

#include "phaseOne.h"
#include "HuffmanTreeNode.h"

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

// COUNT FREQ TESTS
// MAKE THESE SO THEY USE OSTRINGSTREAM TOMORROW
// tests if count freq works when passed a one line string stream
void CF_one_line() {
    std::istringstream test("Hello World!");
    count_freqs(test);
}

// tests if count freq works when passed a multi line string stream
void CF_multi_line() {
    std::istringstream test("Hello World\nMy name is Caleb!\nWbu?");
    count_freqs(test);
}

// tests if count freq works on an empty istringstream
void CF_empty() {
    std::istringstream test("");
    count_freqs(test);
}

// SERIALIZE TESTS
// tests if serialize works on a tree with one node
void S_one_node() {
    HuffmanTreeNode root('a', 5);
    std::string serialized = serialize_tree(&root);
    assert(serialized == "La");
}

// tests if serialize works on a simple tree with three nodes
void S_three_nodes() {
    HuffmanTreeNode left('a', 4);
    HuffmanTreeNode right('b', 2);
    HuffmanTreeNode root('\0', 2, &left, &right);
    std::string serialized = serialize_tree(&root);
    assert(serialized == "ILaLb");
}

// tests if serialize works on an empty tree
void S_empty() {
    std::string serialized = serialize_tree(nullptr);
    assert(serialized == "");
}

// tests if serialize works on a larger huffman tree
void S_large() {
    HuffmanTreeNode n1(' ', 1);
    HuffmanTreeNode n2('b', 1);
    HuffmanTreeNode n3('\0', 2, &n1, &n2);

    HuffmanTreeNode n4('c', 2);
    HuffmanTreeNode n5('\0', 4, &n4, &n3);

    HuffmanTreeNode n6('d', 4);
    HuffmanTreeNode n7('\0', 8, &n6, &n5);

    HuffmanTreeNode n8('f', 5);
    HuffmanTreeNode n9('\n', 5);
    HuffmanTreeNode n10('\0', 10, &n8, &n9);

    HuffmanTreeNode root('\0', 18, &n10, &n7);

    std::string serialized = serialize_tree(&root);
    assert(serialized == "IILfL\nILdILcIL Lb");
}

// DESERIALIZE TESTS
// tests to make sure deserialize works on an empty tree
void deserialize_empty() {
    HuffmanTreeNode *node = deserialize_tree("");
    assert(node == nullptr);
}

// tests to make sure deserialize works on a tree with one node
void deserialize_one() {
    HuffmanTreeNode *node = deserialize_tree("Lb");
    assert(node->get_val() == 'b');
    delete node;
}

// tests to make sure deserialize works on a small tree
void deserialize_small() {
    HuffmanTreeNode *root = deserialize_tree("ILaLb");
    assert(root->get_val() == '\0');
    assert(root->get_left()->get_val() == 'a');
    assert(root->get_right()->get_val() == 'b');
    assert(serialize_tree(root) == "ILaLb");
    delete root->get_left();
    delete root->get_right();
    delete root;
}

// tests to make sure deserialize works on a large tree
void deserialize_large() {
    HuffmanTreeNode *root = deserialize_tree("IILfL\nILdILcIL Lb");
    assert(serialize_tree(root) == "IILfL\nILdILcIL Lb");
    delete root->get_left()->get_left();
    delete root->get_left()->get_right();
    delete root->get_left();
    delete root->get_right()->get_right()->get_right()->get_right();
    delete root->get_right()->get_right()->get_right()->get_left();
    delete root->get_right()->get_right()->get_right();
    delete root->get_right()->get_right()->get_left();
    delete root->get_right()->get_right();
    delete root->get_right()->get_left();
    delete root->get_right();
    delete root;
}