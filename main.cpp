#include<iostream>
#include<vector>
#include <cstdlib>
#include <ctime>
#include "Graph.cpp" 
#include "BST.cpp"
#include<conio.h>
#include <list>
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

void buildGraph(Graph &sudokuGraph)
{
   
    for(int row =0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            int linearIndex = (row*9)+col; //assigns an index to cell
            sudokuGraph.adjList[linearIndex] = getConnectedNodes(row, col);

        }
    }
}


//adding functions to integrate BST with sudoku
//bst for each cell will store its possible values
//used for user input validation

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
    int getScore() const 
    { 
        return score; 
    }
    int getTries() const 
    { 
        return tries; 
    }
    string getName() const 
    { 
        return name_tag; 
    }


};
class Sudoku
{

    vector<vector<int>> sudokuGrid;
    public:
    vector<vector<int>> displayGrid;
    vector<vector<BST>>possibleValues;
    Graph sudokuGraph;
    public:
    Sudoku()
    {
        sudokuGrid.assign(9, vector<int>(9, 0));//empty grid 
        displayGrid.assign(9, vector<int>(9, 0));
        possibleValues.assign(9, vector<BST>(9));

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
            buildGraph(sudokuGraph);
            initializePossibleValues();
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

    void displayMenu(Player &player)
    {
        while(true)
        {
        system("cls");
        cout<<"======== Sudoku Game Menu ========"<<endl;
        cout<<"1. Start a new game"<<endl;
        cout<<"2. View leaderboard"<<endl;
        cout<<"3. Exit" <<endl;
        cout<<"=================================="<<endl;
        cout<<"Enter your choice: ";
        char choice = _getch();  
        if(choice == '1')
        {
            cout<<"\nStarting a new game...\n";
            playSudoku(player);
            break;

        }
        else if(choice == '2')
        {
            cout <<"\nDisplaying the leaderboard...\n";
            //displayLeaderboard();
            break;

        }
        else if(choice == '3')
        {
            cout <<"\nExiting the game. Goodbye!\n";
            //savePlayerInfo();
            break;
        }
        else
        {
            cout<<"\nInvalid input. Please press a key between 1 and 3.\n";
            system("pause");
        }
        }


    }
    void inputValue(int row, int col, int value)
    {
         cout << "Trying to input value " << value << " at (" << row << ", " << col << ")\n";
        if(isSafe(row, col, value))
        {
            displayGrid[row][col] = value;
            updatePossibleValues(row, col, value, sudokuGraph);
            cout << "Value " << value << " inserted at (" << row << ", " << col << ")\n";

        }
        else
        {
            //player.tries--
            cout<<"Attempts Left: "<<"tries"<<endl;
        }
    }
    bool checkUserInput(int row, int col, int value)
    {
        if((displayGrid[row][col]!=0) || (value>9 || value<=0) || (row < 0 || row >= 9 || col < 0 || col >= 9))
        {
            return false;
        }
        //BST valisdation
        if (!isValidInput(row, col, value)) 
        {
            cout << "Error: Value not valid for this cell.\n";
            return false;
        }
        //use the graph to implement constraints on the value
        int userIndex = row*9 + col;
        for(int i: sudokuGraph.adjList[userIndex])
        {
           int nRow = i/9;
           int nCol = i%9;
           if(displayGrid[nRow][nCol] == value)
           {
            cout << "Error: Value violates Sudoku constraints.\n";
            return false;
           }
        }
        return true;
    }

    void playSudoku(Player &player)
    {
        string difficulty;
        cout<<"\nEnter Difficulty Level(Easy, Medium, Hard): \n";
        cin>>difficulty;
        generateSudoku(difficulty);
        printGridDisplay();
        auto startTime = time(0);
        while (true) 
        {
        //system("cls");
        printGridDisplay();
        cout << "\nPlayer: " << player.getName() << "\nScore: " << player.getScore() << "\nTries left: " << player.getTries() << endl;

        int row, col, value;
        printCellOptions();
        cout << "\nEnter Row (0-8): ";
        cin >> row;
        cout << "Enter Column (0-8): ";
        cin >> col;
        cout << "Enter Value (1-9): ";
        cin >> value;

        if (row == -1 || col == -1) 
        {
            cout << "Exiting the game...\n";
            break;
        }

        inputValue(row, col, value);

        if (player.getTries() <= 0) 
        {
            cout << "\nGame Over! You've exhausted all attempts.\n";
            break;
        }
        }

        
        //function validates user input
        //time functionality --- attribute of player class
        //each time a value is added -- check if grid solved else prompt for 
        //next input
        //if solved --- check against solved grid
        //if incorrect --- subtract score 
        //give the option to show leaderboard
        //save the player info if they choose to exit
        //rematch is possible




    }
    bool isPossible(int val, int row, int col)
    {
        for(int i=0; i<9; i++)
        {
            if(displayGrid[row][i] == val)
            {
                return false;
            }
            if(displayGrid[i][col]==val)
            {
                return false;
            }
            if(displayGrid[3*(row/3)+i/3][3*(col/3)+i%3]==val)
            {
                return false;
            }
        }
        return true;
    }

    void initializePossibleValues()
    {
        
        for (int row = 0; row < 9; row++) 
        {
            for (int col = 0; col < 9; col++) 
            {
                if(displayGrid[row][col]==0)
                {
                    
                    for(int val=1; val<=9; val++)
                    {
                        if(isPossible(val, row, col))
                        {
                            possibleValues[row][col].insert(val);
                        }
                    
                    }
                }

            }
        }
    }

    void updatePossibleValues(int row, int col, int value, Graph &graph)
    {
        possibleValues[row][col].remove(value);

        int index = row*9 + col;
        //index for graph

        for(int i: graph.adjList[index])
        {
            int r = i/9;
            int c = i%9;
            possibleValues[r][c].remove(value);
        }
        //removing the value if its connected to the node in the graph

    }

    bool isValidInput(int row, int col, int val)
    {
        return possibleValues[row][col].search(val);
        //if the value is present in the bst, it returns true;

    }
    void printCellOptions() 
    {
    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++) 
        {
            cout << "Cell (" << row << ", " << col << "): ";
            for (int val = 1; val <= 9; val++) 
            {
                if (possibleValues[row][col].search(val)) 
                {
                    cout << val << " ";
                }

            }
            cout<<endl;
        }
    }
    }


};



int main() 
{
    srand(time(0));
    Sudoku sudoku;
    Player player("PlayerName", 'A');
    sudoku.displayMenu(player);

    
}



