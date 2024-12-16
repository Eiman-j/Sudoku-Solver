#include<iostream>
#include<vector>
using namespace std;

class Sudoku
{

    vector<vector<int>> sudokuGrid;
    public:
    Sudoku()
    {
        sudokuGrid.assign(9, vector<int>(9, 0));//empty grid 

    }

    bool isSafe(int val, int row, int col)
    {
        for(int i=0; i<9; i++)
        {
            if(sudokuGrid[row][i] == val)
            {
                return false;
            }
            if(sudokuGrid[i][col]==val)
            {
                return false;
            }
            if(sudokuGrid[3*(row/3)+i/3][3*(col/3)+i%3]==val)
            {
                return false;
            }
        }
        return true;
    }
    bool fillGrid()
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                if(sudokuGrid[i][j]==0)
                {
                    for(int val=1; val<=9; val++)
                    {
                        if(isSafe(val, i, j))
                        {
                            sudokuGrid[i][j]=val;
                            bool isPossible = fillGrid();
                            if(isPossible)
                            {
                                return true;
                            }
                            else
                            {
                                //backtrack
                                sudokuGrid[i][j]=0;
                            }

                        }

                    }
                    return false;
                }
            }
        }
        return true;
    }
    

    void printGrid()
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cout << sudokuGrid[i][j] << " ";
            }
            cout << endl;
        }
    }

};

int main()
{
    Sudoku sudoku;
    sudoku.fillGrid();
    sudoku.printGrid();
}


