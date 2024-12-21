#include<iostream>
#include <fstream>
#include<vector>
#include <cstdlib>
#include <ctime>
#include "Graph.cpp" 
#include "BST.cpp"
#include<conio.h>
#include <list>
#include<string>
#include<thread>
#include<chrono>
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
    public:
    string name_tag;
    char character;
    int score;
    int tries;
    
    Player(string name_tag, char character)
    {
        this->name_tag = name_tag;
        this->character = character;
        this->score = 100; //initial points
        this->tries = 3; 
    }
    
    void savePlayerInfo()
    {
        ofstream outFile(name_tag + ".txt");
        if(outFile.is_open())
        {
            outFile<<"NameTag:"<<name_tag<<endl;
            outFile<<"Character:"<<character<<endl;
            outFile<<"Score:"<<score<<endl;
            outFile.close();
            cout<<"Player information saved successfully!\n";

        }
        else
        {
            cout<<"Error: Unable to save player information."<<endl;
        }
        

    }
    void loadPlayerInfo()
    {
        ifstream inFile(name_tag + ".txt");
        if(inFile.is_open())
        {
            string line;
            while(getline(inFile, line))
            {
                if(line.find("NameTag:")!= string::npos)
                {
                    name_tag = line.substr((line.find(":"))+1);
                }
                else if(line.find("Character:")!= string::npos)
                {
                    character = line.substr(line.find(":") + 1)[0];
                }
                else if(line.find("Score:")!=string::npos)
                {
                    score = stoi(line.substr(line.find(":") + 1));
                }

            }
            inFile.close();

        }
        else
        {
            cout<<"\nError: Can't find saved player information.\n";
        }


    }


};
class Sudoku
{

    vector<vector<int>> sudokuGrid;
    vector<vector<int>> displayGrid;
    vector<vector<BST>>possibleValues;
    Graph sudokuGraph;
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
        void initializeSudokuGrid()
    {
        int toFill = 20;
        int retries = 100;
        while(toFill>0 && retries>0)
        {
            int row = rand() % 9;
            int col = rand() % 9;
            int randomNumber =(rand() % 9)+1;//generate random number to add to grid
            if(sudokuGrid[row][col]==0 && isSafe(randomNumber, row, col))
            {
                sudokuGrid[row][col]=randomNumber;
                toFill--;
            }
            else
            {
                retries--;
            }

        }
        if(!fillGrid())
        {
            sudokuGrid.assign(9, vector<int>(9, 0));
            initializeSudokuGrid();
        }
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
    
    void printGridDisplay()
    {
    cout << "Sudoku Grid:" << endl;
    for (int i = 0; i < 9; i++) 
    {
        if (i % 3 == 0 && i != 0) 
        {
            cout << "------+-------+------" << endl;
        }
        for (int j = 0; j < 9; j++) 
        {
            if (j % 3 == 0 && j != 0) 
            {
                cout << "| ";
            }
            cout << (displayGrid[i][j] == 0 ? "." : to_string(displayGrid[i][j])) <<" ";
        }
        cout<<endl;
    }
    }
        void inputValue(int row, int col, int value, Player &player)
    {
        if(checkUserInput(row, col, value))
        {
            displayGrid[row][col] = value;
            updatePossibleValues(row, col, value, sudokuGraph);
            cout << "Value " << value << " inserted at (" << row << ", " << col << ")\n";

        }
        else
        {
            player.tries--;
            cout<<"Attempts Left: "<<player.tries<<endl;
        }
    }

    
    bool checkUserInput(int row, int col, int value)
    {
        if((!possibleValues[row][col].search(value))|| (value>9 || value<=0) || (row < 0 || row >= 9 || col < 0 || col >= 9))
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
    
    bool isGridSolved()
    {
        for(int row=0; row<9; row++)
        {
            for(int col= 0; col<9; col++)
            {
                if(displayGrid[row][col]==0)
                {
                    return false;
                }
                if(!isSafe(displayGrid[row][col], row, col))
                {
                    return false;
                }
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
        system("cls");
        printGridDisplay();
        cout<<"\nPlayer: " <<player.name_tag<<"\nScore: "<<player.score<<"\nTries left: "<<player.tries<<endl;

        int row, col, value;
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

        inputValue(row, col, value, player);
        if(isGridSolved())
        {
            cout<<"Congratulations! You solved the grid, your score has been updated!\n";
            if(difficulty == "easy"||difficulty=="Easy")
            {
                player.score = player.score + 100;
            }
            if(difficulty == "medium"||difficulty=="Medium")
            {
                player.score = player.score + 150;
            }
            if(difficulty == "hard"||difficulty=="Hard")
            {
                player.score = player.score + 250;
            }
            break;
            
        }

        if (player.tries <= 0) 
        {
            cout << "\nGame Over! You've exhausted all attempts.\n";
            player.score-=50;
            break;
        }
        }
        auto endTime = time(0);
        int duration = difftime(endTime, startTime);//seconds
        int minutes = duration/60;
        int seconds = duration%60;
        cout<<"\nTime taken to solve the Sudoku grid: "<<minutes<<"minutes "<<seconds<<" seconds.\n";
        this_thread::sleep_for(chrono::milliseconds(10000));
        displayMenu(player);

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

    public:
    Sudoku()
    {
        sudokuGrid.assign(9, vector<int>(9, 0));//empty grid 
        initializeSudokuGrid();
        displayGrid.assign(9, vector<int>(9, 0));
        possibleValues.assign(9, vector<BST>(9));

    }

    void displayMenu(Player &player)
    {
        while(true)
        {
        system("cls");
        cout<<"======== Sudoku Game Menu ========"<<endl;
        cout<<"1. Start a new game"<<endl;
        cout<<"2. View Player Stats"<<endl;
        cout<<"3. Exit" <<endl;
        cout<<"=================================="<<endl;
        cout<<"Enter your choice: ";
        char choice = _getch();  
        if(choice == '1')
        {
            player.loadPlayerInfo();
            cout<<"\nStarting a new game...\n";
            playSudoku(player);
            break;


        }
        else if(choice == '2')
        {
            cout <<"\nDisplaying the Player Stats...\n";
            cout<<"Player Name_Tag: "<<player.name_tag<<endl;
            cout<<"Player Character: "<<((player.character == 'X') ? "The Cipher" : "The Riddler")<<endl;
            cout<<"Player Score: "<<player.score<<endl;
            cout<<"Press any key to return to menu..."<<endl;
            _getch();
            continue;


        }
        else if(choice == '3')
        {
            cout <<"\nExiting the game. Goodbye "<<player.name_tag<<"!\n";
            player.savePlayerInfo();
            break;

        }
        else
        {
            cout<<"\nInvalid input. Please press a key between 1 and 3.\n";
            system("pause");
        }
        }
    }




};



int main() 
{
    srand(time(0));
    Sudoku sudoku;
    string n;
    char c;
    cout<<"===================================================="<<endl;
    cout<<"Welcome to the Sudoku Puzzle Interactive Game\n";
    cout<<"===================================================="<<endl;
    cout<<"Enter your unique nametag: ";
    getline(cin, n);
    cout<<"Choose a wild character to play as: \n 'X'--- The Cipher or 'K' --- The Riddler\n";
    while(true)
    {
        c = _getch();
        if(c == 'X' || c == 'K')
        {
            break;
        }
        else
        {
            cout<<"Choose a wild character to play as: \n 'X'--- The Cipher or 'K' --- The Riddler\n";

        }

    }
    char st;
    cout<<"Character chosen successfully..."<<endl;
    cout<<"Press '0' to start playing"<<endl;
    st = _getch();
    if(st == '0')
    {
        Player player(n, c);
        sudoku.displayMenu(player);

    }
    else
    {
        cout<<"Bye Bye "<<n<<"..."<<endl;
    }

    

    
}



