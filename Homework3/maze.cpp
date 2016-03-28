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

// Return true if there is a path from (sr,sc) to (er,ec) through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	maze[sr][sc] = '$'; //'$' will denote a discovered coordinate

	int r = sr;
	int c = sc;

	//return true if at end coordinate
	if (sr == er && sc == ec)
		return true;

	//Check each direction if not at end
	else
	{
		//North
		if (maze[r - 1][c] != 'X' && maze[r - 1][c] != '$')
		{
			if (pathExists(maze, nRows, nCols, r - 1, c, er, ec))
				return true;
		}
		//East
		if (maze[r][c + 1] != 'X' && maze[r][c + 1] != '$')
		{
			if (pathExists(maze, nRows, nCols, r, c + 1, er, ec))
				return true;
		}
		//South
		if (maze[r + 1][c] != 'X' && maze[r + 1][c] != '$')
		{
			if (pathExists(maze, nRows, nCols, r + 1, c, er, ec))
				return true;
		}
		//West
		if (maze[r][c - 1] != 'X' && maze[r][c - 1] != '$')
		{
			if (pathExists(maze, nRows, nCols, r, c - 1, er, ec))
				return true;
		}
	}

	//return false if path not found;
	return false;
}
