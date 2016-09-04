//
//  genData.cpp
//
//  Created by Brian Taylor on 06/12/15.
//  52311859
//  z2b0b
//  Lab section: L0D
//  Copyright (c) 2015 Brian Taylor. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>  // rand
#include <time.h>    // time

using namespace std;

int main(int argc, const char * argv[]) {
    
    int n;
    
    
    if (argc == 3) {
        n = atoi(argv[1]);
        
        fstream outfile(argv[2], fstream::out);
        string key;
                
        srand( time(NULL));  // initialize random seed
        
        for (int j = 1; j <= n; j++) {                 // create n keys
            key = "";
            for (int i = 0; i < 8; i++) {              // create the 8-letter key
                key = key + (char) (rand() % 26 + 65);
            }
            
            cout << key << "   " << j << endl;    // for debugging purposes
            
            outfile << key << "   " << j << endl;  // write the key and 'data' to the file
        }
        outfile.close();
        
    } else {
        cerr << "Usage: " << argv[0] << " n filename" << endl;
        exit(-1);
    }
    
    return 0;
}