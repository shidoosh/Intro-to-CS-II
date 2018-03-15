//
//  main.cpp
//  CS32 - HOMEWORK 2
//
//  Created by Stefanie Shidoosh on 4/28/17.
//  Copyright © 2017 Stefanie Shidoosh. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;


class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
/*
 Push the starting coordinate (sr,sc) onto the coordinate stack and
 update maze[sr][sc] to indicate that the algorithm has encountered
 it (i.e., set maze[sr][sc] to have a value other than '.').
 While the stack is not empty,
 {   Pop the top coordinate off the stack. This gives you the current
 (r,c) location that your algorithm is exploring.
 If the current (r,c) coordinate is equal to the ending coordinate,
 then we've solved the maze so return true!
 Check each place you can move from the current cell as follows:
 If you can move NORTH and haven't encountered that cell yet,
 then push the coordinate (r-1,c) onto the stack and update
 maze[r-1][c] to indicate the algorithm has encountered it.
 If you can move EAST and haven't encountered that cell yet,
 then push the coordinate (r,c+1) onto the stack and update
 maze[r][c+1] to indicate the algorithm has encountered it.
 If you can move SOUTH and haven't encountered that cell yet,
 then push the coordinate (r+1,c) onto the stack and update
 maze[r+1][c] to indicate the algorithm has encountered it.
 If you can move WEST and haven't encountered that cell yet,
 then push the coordinate (r,c-1) onto the stack and update
 maze[r][c-1] to indicate the algorithm has encountered it.
 }
 There was no solution, so return false
 */
    bool result = false;
    std::stack<Coord> coordStack;
    
    coordStack.push(Coord(sr, sc)); //Push the starting coordinate (sr,sc) onto the coordinate stack
    maze[sr][sc] = '@';             // update maze[sr][sc] to indicate that the algorithm has encountered
                                    //it (i.e., set maze[sr][sc] to have a value other than '.').
    
    Coord endPoints = Coord(er, ec);
    
    while(!coordStack.empty())
    {
        Coord top = coordStack.top();
        coordStack.pop();               // pop top coordinate
        maze[top.r()][top.c()] = '@';
        
        if(top.r() == endPoints.r() && top.c() == endPoints.c())//if the current (r,c) coordinate is equal to the ending coord,return true
        {
            return true;
        }

            if(maze[top.r()-1][top.c()]=='.')
            {
                maze[top.r()-1][top.c()] = '@';  //[r-1, c] NORTH
                coordStack.push(Coord(top.r()-1, top.c()));
            }
            if(maze[top.r()][top.c()+1]=='.')
            {
                maze[top.r()][top.c()+1] = '@'; //[r, c+1] EAST
                coordStack.push(Coord(top.r(), top.c()+1));
            }
            if(maze[top.r()+1][top.c()]=='.')
            {
                maze[top.r()+1][top.c()] = '@'; //[r+1, c] SOUTH
                coordStack.push(Coord(top.r()+1, top.c()));
            }
            if(maze[top.r()][top.c()-1]=='.')
            {
                maze[top.r()][top.c()-1] = '@'; //[r, c-1] WEST
                coordStack.push(Coord(top.r(), top.c()-1));
            }
    }
    return result;
}
