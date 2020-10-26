#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#define DEFAULT_FILE "/home/francisco/intro_plan/master-ipr/map1/map1.csv"
#define DEFAULT_START_X 1
#define DEFAULT_START_Y 1
#define DEFAULT_END_X 7
#define DEFAULT_END_Y 2
#define OBSTACLE -777
#define INI_SCORE 777

class Node
{
public:
    Node(int x,int y,int id,int parentId, int score) {
      _x = x; _y = y;
      _id = id; _parentId = parentId;
      _score = score;
    }
    void dump()
    {
        printf("---------- x %d, y %d, id %d, pid %d\n",_x,_y,_id,_parentId, _score);
    }
    int _x,_y,_id,_parentId, _score;
};


class Program
{
public:
    bool parseFileLine(std::vector<int>& intsOnFileLine)
    {
        intsOnFileLine.clear();

        if( file.eof() )
            return false;

        std::string csv;
        getline(file, csv);
        std::istringstream buffer(csv);
        std::string token;
        int d;

        while( std::getline( buffer, token, ',' ) )
        {
            std::istringstream ss(token);
            ss >> d;
            intsOnFileLine.push_back(d);
        }


        return true;
    }

    void dump()
    {
        for(int i=0;i<intMap.size();i++)
        {
            for(int j=0;j<intMap[0].size();j++)
            {
                printf("%d ",intMap[i][j]);
            }
            printf("\n");
        }

    }

    void dumpTable(int **table, int rows, int cols)
    {
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                printf("%d ",table[i][j]);
            }
            printf("\n");
        }
    }

    void backTracking(int **dpTable){

        //If node is in explored list we pop the stack and call BT again
        if(){
            nodeStack.pop_back();
            backTracking(dpTable);
        }
        //Explore top of the stack, order results according to their score

        return backTracking(dpTable);
    }

    /* Solve a tie between two options with same score when performing backtracking
     * Checks top and right cells and the one with higher scoring adjacent cells gets chosen 
     * Default option is above
     * 
     * Input: table, and position which caused the tie**
     * Ouput: row, col of the cell considered to be better**/
    int solveTie(int **table, int row, int col, int sizeRows, int sizeCols)
    {
        int scoreAbove = 0;
        int scoreLeft = 0;

        //Score for cell above original
        if(col != sizeCols)
            scoreAbove +=  table[row-1][col+1];
        if(row != 1)
            scoreAbove += table[row-2][col];

        //Score for cell to the left of original
        if(row != sizeRows)
            scoreAbove +=  table[row+1][col-1];
        if(col != 1)
            scoreAbove += table[row][col-2];

        //In case of another tie we select above
        if(scoreLeft > scoreAbove)
            return 1;
        return 0;
    }


    bool run()
    {
        std::string fileName = DEFAULT_FILE;
        file.open(fileName.c_str());
        if( ! file.is_open() )
        {
              printf("Not able to open file: %s\n",fileName.c_str());
              return false;
        }
        printf("Opened file: %s\n",fileName.c_str());

        std::vector<int> intsOnFileLine;

        while( this->parseFileLine(intsOnFileLine) )
        {
            if ( intsOnFileLine.size() == 0 ) continue;

            intMap.push_back( intsOnFileLine );
        }

        file.close();

        //Declare DP Matrix and aux variables
        int i, j, max;
        int rows = intMap.size() - 2;
        int cols = intMap[0].size() - 2;

        int **dpTable;
        for(i = 0; i < rows; i ++)
            dpTable[i] = new int[cols];
      
        //std::cout << rows << " Numero de filas \n";
        //std::cout << cols << " Numero de columnas \n";

        //Initialize DP Matrix first row and column
        //In case we find an obstacle subsequent cells get obstacle score
        for(int obs_flag = 0, i = 0; i < rows; i ++) {
            if(intMap[i+1][1] == 1 || obs_flag) {
                dpTable[i][0] = OBSTACLE;
                obs_flag = 1;
            }
            else {
                dpTable[i][0] = INI_SCORE - i;
            }
        }
        for(int obs_flag = 0, j = 0; j < cols; j ++) {
            if(intMap[1][j+1] == 1 || obs_flag) {
                dpTable[j][0] = OBSTACLE;
                obs_flag = 1;
            }
            else {
                dpTable[0][j] = INI_SCORE - j;
            }
        }
        //Update costs of each cell with the cost of getting there
        for(i = 1; i < rows; i ++)
        {
            for(j = 1; j < cols; j ++)
            {   
                //In case current cell is an obstacle
                if(intMap[i+1][j+1] == 1)
                {
                    max = OBSTACLE;
                } else {
                    //Update cell with optimal score
                    max = dpTable[i][j-1];
                    max = (max >= dpTable[i-1][j]) ? max : dpTable[i-1][j];
                    max = (max > 0) ? max - 1 : max + 1;
                }
                
                dpTable[i][j] = max;
            }
        }
        printf("This are the scores for each of the cells of the DP table");
        this->dumpTable(dpTable, rows, cols);

        //Perform backtracking to get the optimal path according to our DP Table

        //Push goal node into the stack for backtracking
        Node* goal = new Node(DEFAULT_END_X, DEFAULT_END_Y, 1, -1, dpTable[DEFAULT_END_X][DEFAULT_END_Y]);
        nodeStack.push_back(goal);
       


        return true;
    } //--main

private:
    std::vector<std::vector<int> > intMap;
    std::ifstream file;
    std::vector<Node*> nodeStack;
    std::vector<Node*> nodeExplored;
};

int main()
{
    Program program;
    if( ! program.run() )
        return 1;
    return 0;
}
