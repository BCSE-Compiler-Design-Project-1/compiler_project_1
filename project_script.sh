#!/bin/sh
(
    #lexical analysis phase
    g++ -std=c++11 lexicalAnalyser.cpp -o lexicalAnalyser
    ./lexicalAnalyser < samplecode.txt
    
    #symbol table generation , output stored in symtab
    g++ -std=c++11 generate.cpp -o generate
    ./generate < samplecode.txt
    
    #LL1 parser parses the output from lexical analyser (stream of tokens) and stores the final output in final_output.txt
    g++ -std=c++11 LL1.cpp -o LL1
    ./LL1 < to_be_executed_filenames.txt
) > final_output.txt
