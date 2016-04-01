#include "globals.h"
#include "History.h"
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////
//  History implementations
///////////////////////////////////////////////////////////////////////////

History::History(int nRows, int nCols)
{
	//set number of rows and columns
	m_rows = nRows;
	m_cols = nCols;

	//populate grid with dots (empty)
	for (int i = 0; i < m_cols; i++) {
		for (int j = 0; j < m_cols; j++) {
			grid[i][j] = 0;
		}
	}
}

bool History::record(int r, int c)
{
	//returns false if specified coordinate is not in the grid
	if (r < 0 || r > m_rows || c < 0 || c > m_cols) return false;
	//increment count of kills at rc
	else
		grid[r - 1][c - 1]++;
	return true;
}

void History::display() const
{
	clearScreen();
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
		{
			if (grid[i][j] == 0)
				cout << '.';//prints dot if no kills at coordinate
			else if (grid[i][j] < 26)
				cout << (char)('A' - 1 + grid[i][j]);//prints corresponding letter for 1-25 kills
			else
				cout << 'Z'; //prints Z for 26+ kills
		}
		cout << endl;
	}
}