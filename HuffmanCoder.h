/*
 * HuffmanCoder.h
 * Author: Caleb Johnson (cjohns34)
 * Date: 11/15/23
 * Purpose: This file contains the class definition of the HuffmanCoder class.
 * This class allows users to compress a given text file into a "zapped" file
 * and decompress a given "zapped" file back into a text file. This class
 * utilizes the Huffman Encoding Algorithm to encode characters in a given
 * text into binary encodings of various sizes. Characters that appear more
 * frequently in text are given shorter encodings such that the "zapped" file
 * optimizes the number of bits that it uses. Every zapped file also contains
 * a serialized version of the tree used to get encodings so that the program
 * can use this tree to decode the zapped file.
 */

#ifndef _HUFFMAN_CODER
#define _HUFFMAN_CODER

#include "HuffmanTreeNode.h"
#include "BinaryIO.h"
#include "ZapUtil.h"

#include <string>
#include <fstream>

class HuffmanCoder {
    public:
        HuffmanCoder();
        ~HuffmanCoder();
        void encoder(const std::string &inputFile, 
        const std::string &outputFile);
        void decoder(const std::string &inputFile, 
        const std::string &outputFile);

    private:
        // member variables
        std::string *encodings;

        // file helpers
        template<typename streamtype>
        void open_and_check(streamtype &stream, std::string file_name);
        template<typename streamtype>
        void close_file(streamtype &stream);

        // encoder helpers
        int *count_freqs(std::istream &text);
        HuffmanTreeNode *make_tree(int *counts);
        std::string serialize_tree(HuffmanTreeNode *root);
        std::string get_bitstring(std::ifstream &infile, 
        const std::string &file);
        bool check_empty_file(int *counts);

        // decoder helpers
        HuffmanTreeNode *deserialize_tree(std::istream &in);
        std::string decode_bits(std::istream &in);
        void write_output(std::string text, std::string outputFile);

        // general helpers
        void get_encodings(HuffmanTreeNode *root, std::string e);
        void delete_tree(HuffmanTreeNode* node);
};

#endif