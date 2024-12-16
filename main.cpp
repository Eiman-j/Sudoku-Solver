#include<iostream>
#include<vector>
#include <cstdlib>
#include <ctime>
#include "Graph.cpp" 
//using graph for constraint monitoring while backtracking/inputting values
using namespace std;


//build a graph that represents
//our grid with constraints
//vertices are each cell
//vertices connected through an edge 
//are either in same row, col or 3x3 subgrid
//validating user input


list<int> getConnectedNodes(int row, int col)
{
    list<int> tempNeighbours;
    int subGridRow = (row/3)*3;
    int subGridCol = (col/3)*3;
    //for the row
    for(int i=0; i<9; i++)
    {
        if(i!=col)
        {
            tempNeighbours.push_back((row*9) + i);
        }

    }
    //for the column
    for(int i=0; i<9; i++)
    {
        if(i!= row)
        {
            tempNeighbours.push_back((i*9)+col);
        }

    }
    //for the 3x3 subgrid
    for(int i=subGridRow; i<subGridRow+3; i++)
    {
        for(int j=subGridCol; j<subGridCol+3; j++)
        {
            if(i!=row || j!=col)
            {
                tempNeighbours.push_back((i*9)+j);
            }
        }
    }
    return tempNeighbours;

}

void buildGraph()
{
    Graph sudokuGraph;
    for(int row =0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            int linearIndex = (row*9)+col; //assigns an index to cell
            sudokuGraph.adjList[linearIndex] = getConnectedNodes(row, col);

        }
    }
}
class Sudoku
{

    vector<vector<int>> sudokuGrid;
    public:
    vector<vector<int>> displayGrid;
    public:
    Sudoku()
    {
        sudokuGrid.assign(9, vector<int>(9, 0));//empty grid 
        displayGrid.assign(9, vector<int>(9, 0));

    }
    void createDisplayGrid()
    {
        //cpoying values from the original grid to the user grid/unsolved
        for(int i= 0; i<9; i++)
        {
            for(int j = 0; j<9; j++)
            {
                displayGrid[i][j]= sudokuGrid[i][j];
            }
        }
    }
    void removeCells(int cellsToRemove) 
    {
        //randomly removes values from cells depending on the difficulty level
        while (cellsToRemove > 0) 
        {
        int row = rand() % 9;
        int col = rand() % 9;
        if (displayGrid[row][col] != 0) 
        {
            displayGrid[row][col] = 0;
            --cellsToRemove;
        }
        }
    }
    void generateSudoku(string& difficultyLevel)
    {
        bool isFilled = fillGrid();
        createDisplayGrid();
        int cellsToRemove = 0;
        if(isFilled)
        {
            if(difficultyLevel == "easy" || difficultyLevel =="Easy")
            {
                cellsToRemove = 30;   
            }
            if(difficultyLevel == "medium" || difficultyLevel =="Medium")
            {
                cellsToRemove = 40;   
            }
            if(difficultyLevel == "hard" || difficultyLevel =="Hard")
            {
                cellsToRemove = 50;   
            }

            removeCells(cellsToRemove);
            buildGraph();
        }

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

    void printGridDisplay()
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cout << displayGrid[i][j] << " ";
            }
            cout << endl;
        }
    }

};

class Player
{
    string name_tag;
    char character;
    int score;
    int tries;
    public:
    Player(string name_tag, char character)
    {
        this->name_tag = name_tag;
        this->character = character;
        this->score = 100; //initial points
        this->tries = 3; 
    }
    void inputValue(Sudoku &sudoku, Graph &graph, int row, int col, int value)
    {
        if(checkUserInput(sudoku, graph, row, col, value))
        {
            sudoku.displayGrid[row][col] = value;

        }
        else
        {
            --tries;
            cout<<"Attempts Left: "<<tries<<endl;
        }
    }
    bool checkUserInput(Sudoku &sudoku, Graph &graph, int row, int col, int value)
    {
        if((sudoku.displayGrid[row][col]!=0) || (value>9 || value<=0) || (row < 0 || row >= 9 || col < 0 || col >= 9))
        {
            return false;
        }
        //use the graph to implement constraints on the value
        int userIndex = row*9 + col;
        for(int i: graph.adjList[userIndex])
        {
           int nRow = i/9;
           int nCol = i%9;
           if(sudoku.displayGrid[nRow][nCol] == value)
           {
            cout << "Error: Value violates Sudoku constraints.\n";
            return false;
           }
        }
        return true;
    }


};

int main()
{
    srand(time(0));
    Sudoku sudoku;
    string difficulty = "easy";
    sudoku.generateSudoku(difficulty);
    sudoku.printGrid();
    cout<<endl<<endl;
    sudoku.printGridDisplay();


}


