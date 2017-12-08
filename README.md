## Ternary Directed Acyclic Word Graph Dictionary/Spellchecker ##

A spellchecker implemented through a ternary directed acyclic word graph dictionary. 

## Installation ##
To run the program at the command line:
g++ TernaryDagDictionaryMain.cpp TernaryDagDictionary.cpp -lcurl
./a.out

-lcurl flag is required because of the <curl/curl.h> header file used to download the dictionary.

TernaryDagDictionary.h, TernaryDagDictionaryMain.cpp, TernaryDagDictionary.cpp and TernaryDagNode.h must be in the 
same directory.

## Usage ##

At the prompt, enter a word with up to two spelling errors. If the corrected verion of the entered word is in the dictionary, it will
be displayed. Otherwise, the user will be notified that the word is not in the dictionary.

## Credits ##

Terrence Gausi
Christian Lopez
Hangbo Gu

## License ##

MIT License

Copyright (c) 2017 SJSU CMPE130 Fall 2017 Group 3

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
