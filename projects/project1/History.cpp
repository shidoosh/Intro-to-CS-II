//
//  History.cpp
//  CS32 - PROJECT 1
//
//  Created by Stefanie Shidoosh on 4/6/17.
//  Copyright © 2017 Stefanie Shidoosh. All rights reserved.
//

#include "History.h"
#include "Arena.h"
#include <iostream>
using namespace std;


History::History(int nRows, int nCols) :  h_Rows(nRows), h_Cols(nCols)
{
    for(int i = 0; i < h_Rows; i++)
    {
        for(int j = 0; j < h_Cols; j++)
        {
            h_displayGrid[i][j] = '.';
        }
    }
}


bool History::record(int r, int c)
{
    bool result = true;
    if((r-1) < 0 || (c-1) < 0 || (r-1) > MAXROWS || (c-1) > MAXCOLS || r > h_Rows || c > h_Cols)
    {
        result = false; 
    }
    
    else
    {
        
        if(h_displayGrid[r-1][c-1] >='A' && h_displayGrid[r-1][c-1] <= 'Y')
        {
            h_displayGrid[r-1][c-1]++;
        }
        else if(h_displayGrid[r-1][c-1] >= 'Z')      //what to do about when it goes above Z?
        {
            h_displayGrid[r-1][c-1] = 'Z';
        }
        else
            h_displayGrid[r-1][c-1] = 'A';
    }
    return result;
}




void History::display() const
{
    for(int i = 0; i < h_Rows; i++)
    {
        for(int j = 0; j < h_Cols; j++)
        {
            cout << h_displayGrid[i][j];
        }
        cout << endl;
    }
    clearScreen();
}


