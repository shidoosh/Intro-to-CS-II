
bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    if(sr == er && sc == ec)
        return true;
    maze[sr][sc] = '@';
    if(maze[sr-1][sc]=='.')   //NORTH
    {
        if(pathExists(maze, sr-1, sc, er, ec))
            return true;
    }
    if(maze[sr][sc+1]=='.')  //EAST
    {
        if(pathExists(maze, sr, sc+1, er, ec))
           return true;
    }
    if(maze[sr+1][sc]=='.')   //SOUTH
    {
        if(pathExists(maze, sr+1, sc, er, ec))
            return true;
    }
    if(maze[sr][sc-1]=='.')  //WEST
    {
        if(pathExists(maze, sr, sc-1, er, ec))
            return true;
    }
    
    return false;
}
