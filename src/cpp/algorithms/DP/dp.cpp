#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <stdlib.h> 
#include <time.h>
#include <chrono> 
using namespace std::chrono; 

#define DEFAULT_FILE "/home/francisco/intro_plan/master-ipr/map1/map1.csv"
#define DEFAULT_START_X 1
#define DEFAULT_START_Y 1
#define DEFAULT_END_X 5 //Si el mapa tiene N 1's en la primera fila, maximo N-3
#define DEFAULT_END_Y 7 //Si el mapa tiene M 1's en la primera columna, maximo N-3
#define OBSTACLE 0
#define INI_SCORE 777
#define WORST_SCORE -777

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
        printf("---------- x %d, y %d, id %d, pid %d\n",_x,_y,_id,_parentId);
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

    //Checks if a node with same coordinates exists in explored list
    bool isNodeExplored(Node *node){
        if(nodeExplored.empty())
            return false;
        for(int i = 0; i < nodeExplored.size(); i++){
            if((nodeExplored[i]->_x == node->_x) && (nodeExplored[i]->_y == node->_y) ){
                return true;
            }
        }
        return false;
    }

    void backTracking(int **dpTable, int rows, int cols){

        //Stop if nodeStack is empty
        if(nodeStack.empty())
            return;
        //Pop from stack and recover last node
        Node *node = nodeStack.back();
        nodeStack.pop_back();

        //If this node is already in the list, call again backtrack
        if(isNodeExplored(node)){
            backTracking(dpTable, rows, cols);
        }
        //Add node to explored list
        nodeExplored.push_back(node);

        std::cout << "\n x:" << nodeExplored.back()->_x << " y:"<< nodeExplored.back()->_y;//remove

        //Stop in case goal is reached
        if(node->_score == INI_SCORE) {
            nodeStack.clear();
            printf("\nSALIDA ECONTRADA\n");
            return;
        }

        //Get best scoring adjacent node
        int max = WORST_SCORE;
        int top_score, left_score, right_score, bot_score = 0;
        Node * max_node, *node_top, *node_left, *node_right, *node_bot = NULL;

        //If top element exists
        if(node->_x > 0){
            //perror("Ejecuto top");
            top_score  = dpTable[node->_x-1][node->_y];
            node_top = new Node(node->_x-1, node->_y, rand(), node->_id, top_score);

            if(top_score > max && top_score!=0){
                max_node = node_top;
                max = top_score;
            }
        }
        
        //If left element exists
        if(node->_y > 0){
            //perror("Ejecuto left");
            left_score  = dpTable[node->_x][node->_y-1];
            node_left = new Node(node->_x, node->_y-1, rand(), node->_id, left_score);

            if(left_score > max && left_score!=0){
                max_node = node_left;
                max = left_score;
            }
        }
        
        //If right element exists
        if((node->_y + 1) <= cols){
            //perror("Ejecuto right");
            right_score  = dpTable[node->_x][node->_y+1];
            node_right = new Node(node->_x, node->_y+1, rand(), node->_id, right_score);

            if(right_score > max && right_score != 0){
                max_node = node_right;
                max = right_score;
            }
        }
        
        //If bot element exists
        if((node->_x + 1) <= rows){
            //perror("Ejecuto bot");
            bot_score  = dpTable[node->_x+1][node->_y];
            node_bot = new Node(node->_x+1, node->_y, rand(), node->_id, bot_score);

            if(bot_score > max && bot_score!=0){
                max_node = node_bot;
                max = bot_score;
            }
        }
        if(max_node != node_bot && node_bot != NULL && bot_score != 0)
            nodeStack.push_back(node_bot);

        if(max_node != node_right && node_right != NULL && right_score != 0)
            nodeStack.push_back(node_right);

        if(max_node != node_left && node_left != NULL && left_score != 0)
            nodeStack.push_back(node_left);

        if(max_node != node_top && node_top != NULL && top_score != 0)
            nodeStack.push_back(node_top);

        //Push selected node into the stack
        if(max_node != NULL)
            nodeStack.push_back(max_node);
        //perror("Fin BT call");
        return backTracking(dpTable, rows, cols);
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
            scoreLeft +=  table[row+1][col-1];
        if(col != 1)
            scoreLeft += table[row][col-2];

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
        
        // initialize random seed for IDs
        srand (time(NULL));

        //Declare DP Matrix and aux variables
        int i, j, max, up, left;
        int rows = intMap.size()-2;
        int cols = intMap[0].size()-2;

        int **dpTable = new int*[rows];

        for(i = 0; i < rows; i ++)
            dpTable[i] = new int[cols];     
        std::cout << rows << " filas \n";
        std::cout << cols << " columnas \n";


        //Initialize DP Matrix first row and column
        //In case we find an obstacle subsequent cells get obstacle score
        for(int obs_flag = 0, i = 0; i < rows; i ++) {
             if(intMap[i+1][1] == 1) {                   
                dpTable[i][0] = OBSTACLE;
                obs_flag = -1;
            } else if(obs_flag !=0) {
                dpTable[i][0] = OBSTACLE + obs_flag;
                obs_flag --;
            }else {
                dpTable[i][0] = INI_SCORE - i;
            }
        }
        for(int obs_flag = 0, j = 0; j < cols; j ++) {
            if(intMap[1][j+1] == 1) {                   
                dpTable[0][j] = OBSTACLE;
                obs_flag = -1;
            } else if(obs_flag !=0){
                dpTable[0][j] = OBSTACLE + obs_flag;
                obs_flag --;
            } else {
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
                    up = dpTable[i][j-1];
                    left = dpTable[i-1][j];

                    //Ignore obstacles in case of existing path
                    if ((up == 0 && left != 0))
                        max = left;
                    if ((left == 0 && up != 0))
                        max = up;
                    //Any other case, get high scoring option
                    else {
                        max = (left > up) ? left : up;
                    }
                    max --;
                }   
                dpTable[i][j] = max;
            }
        }
        printf("\nPuntuacion para cada celda de la tabla DP\n\n");
        this->dumpTable(dpTable, rows, cols);

        //Push goal node into the stack for backtracking
        Node* goal = new Node(DEFAULT_END_X, DEFAULT_END_Y, 1, -1, dpTable[DEFAULT_END_X][DEFAULT_END_Y]);
        nodeStack.push_back(goal);
        printf("\n\nLista con la secuencia de nodos explorada que lleva a la meta:\n");
        auto start = high_resolution_clock::now();
       //Perform backtracking to get the optimal path according to our DP Table
        this->backTracking(dpTable, rows, cols);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        
        std::cout << duration.count() << ": Tiempo de ejecucion\n"; 
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
