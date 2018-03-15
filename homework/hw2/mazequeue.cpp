//
//  mazequeue.cpp
//  CS32 - HOMEWORK 2
//
//  Created by Stefanie Shidoosh on 5/1/17.
//  Copyright © 2017 Stefanie Shidoosh. All rights reserved.
//

#include <iostream>
#include <queue>
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
    std::queue<Coord> coordQueue;
    
    coordQueue.push(Coord(sr, sc)); //Push the starting coordinate (sr,sc) onto the coordinate stack
    if(maze[sr][sc] =='.')   //will we be passed only good values?
    maze[sr][sc] = '@';             // update maze[sr][sc] to indicate that the algorithm has encountered
    //it (i.e., set maze[sr][sc] to have a value other than '.').
    
    Coord endPoints = Coord(er, ec);
    
    while(!coordQueue.empty())
    {
        Coord front = coordQueue.front();
        coordQueue.pop();               // pop top coordinate
        
        if(front.r() == endPoints.r() && front.c() == endPoints.c())//if the current (r,c) coordinate is equal to the ending coord,return true
            return true;

        for(int i = 0; i < 4; i++)
        {
            if(maze[front.r()-1][front.c()]=='.')
            {
                maze[front.r()-1][front.c()] = '@';  //[r-1, c] NORTH
                coordQueue.push(Coord(front.r()-1, front.c()));
            }
        
            else if(maze[front.r()][front.c()+1]=='.')
            {
                maze[front.r()][front.c()+1] = '@'; //[r, c+1] EAST
                coordQueue.push(Coord(front.r(), front.c()+1));
            }
        
            else if(maze[front.r()+1][front.c()]=='.')
            {
                maze[front.r()+1][front.c()] = '@'; //[r+1, c] SOUTH
                coordQueue.push(Coord(front.r()+1, front.c()));
            }
        
            else if(maze[front.r()][front.c()-1]=='.')
            {
                maze[front.r()][front.c()-1] = '@'; //[r, c-1] WEST
                coordQueue.push(Coord(front.r(), front.c()-1));
            }
        }
    }
    return result;
}
