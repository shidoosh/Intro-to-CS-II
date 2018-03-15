//
//  History.h
//  CS32 - PROJECT 1
//
//  Created by Stefanie Shidoosh on 4/6/17.
//  Copyright © 2017 Stefanie Shidoosh. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"

class Arena;
class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    char h_displayGrid[MAXROWS][MAXCOLS];

private:
    int h_Rows;
    int h_Cols;
};


#endif /* History_h */
