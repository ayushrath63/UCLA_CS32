#include <queue>
#include <string>

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


// Return true if there is a path from (sr,sc) to (er,ec) through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = '$'; //'$' will denote a discovered coordinate

	while (coordQueue.size() != 0)
	{
		//Find current position and pop from Queue
		Coord current = coordQueue.front();
		coordQueue.pop();
		int r = current.r();
		int c = current.c();

		//return true if at end coordinate
		if (r == er && c == ec)
			return true;

		//Check each direction if not at end
		else
		{
			//North
			if (maze[r - 1][c] != 'X' && maze[r - 1][c] != '$')
			{
				coordQueue.push(Coord(r - 1, c));
				maze[r - 1][c] = '$';
			}
			//East
			if (maze[r][c + 1] != 'X' && maze[r][c + 1] != '$')
			{
				coordQueue.push(Coord(r, c + 1));
				maze[r][c + 1] = '$';
			}
			//South
			if (maze[r + 1][c] != 'X' && maze[r + 1][c] != '$')
			{
				coordQueue.push(Coord(r + 1, c));
				maze[r + 1][c] = '$';
			}
			//West
			if (maze[r][c - 1] != 'X' && maze[r][c - 1] != '$')
			{
				coordQueue.push(Coord(r, c - 1));
				maze[r][c - 1] = '$';
			}
		}
	}

	//return false if path not found;
	return false;
}

//TESTS BELOW

#include <iostream>

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}