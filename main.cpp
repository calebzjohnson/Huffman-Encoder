/*
 * main.cpp
 * Author: Caleb Johnson (cjohns34)
 * Date: 11/15/23
 * Purpose: This file contains the main function for the zap program. This
 * function drives the program, calling either the encoder or decoder function
 * depending on the command line arguments given by the user. It also ensures
 * that the user properly inputs the command line argument and prints a usage
 * error message if the user types an improper command.
 */

#include "HuffmanCoder.h"
using namespace std;

#include <iostream>

int main(int argc, char *argv[]) {
    // check usage
    if (argc != 4) {
        cerr << "Usage: ./zap [zap | unzap] inputFile outputFile\n";
        exit(EXIT_FAILURE);
    }
    string argTwo = argv[1];
    if (argTwo != "zap" and argTwo != "unzap") {
        cerr << "Usage: ./zap [zap | unzap] inputFile outputFile\n";
        exit(EXIT_FAILURE);
    }

    // create HuffmanCoder object
    HuffmanCoder huffmanCoder;

    if (argTwo == "zap") {
        huffmanCoder.encoder(argv[2], argv[3]);
    } else {
        huffmanCoder.decoder(argv[2], argv[3]);
    }
    return 0;
}