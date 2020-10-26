#include <vector>
#include <fstream>
#include <sstream>

#define DEFAULT_FILE "/home/francisco/intro_plan/master-ipr/map1/map1.csv"
#define DEFAULT_START_X 2
#define DEFAULT_START_Y 2
#define DEFAULT_END_X 7
#define DEFAULT_END_Y 2

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
    /*Calculate score of a given node *
      Score of a node = Manhattan distance to goal node*/
    int getScore(int x, int y)
    {
    return  (x - DEFAULT_END_X) + abs(y - DEFAULT_END_Y));
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

        intMap[DEFAULT_START_X][DEFAULT_START_Y] = 3;
        intMap[DEFAULT_END_X][DEFAULT_END_Y] = 4;
        this->dump();

        Node* init = new Node(DEFAULT_START_X,DEFAULT_START_Y,0,-2, getScore(DEFAULT_START_X, DEFAULT_START_Y));
        nodes.push_back(init);

        bool done = false;

        int goalParentId;

        while( ! done )
        {
            int keepNodeSize = nodes.size();
            printf("-------------------------nodes: %d\n",keepNodeSize);

            for(int nodeIdx=0;nodeIdx<keepNodeSize;nodeIdx++)
            {
                nodes[nodeIdx]->dump();

                int tmpX, tmpY;

                //printf("up\n");
                tmpX = nodes[nodeIdx]->_x - 1;
                tmpY = nodes[nodeIdx]->_y;
                if( intMap[tmpX][tmpY] == 4)
                {
                    printf("GOOOOL!!!\n");
                    goalParentId = nodes[nodeIdx]->_id;
                    done = true;
                    break;
                }
                else if( intMap[tmpX][tmpY] == 0 )
                {
                    //printf("Create node\n");
                    Node* node = new Node(tmpX,tmpY,nodes.size(),nodes[nodeIdx]->_id, getScore(tmpX,tmpY));
                    intMap[tmpX][tmpY] = 2;
                    nodes.push_back(node);
                }

                //printf("down\n");
                tmpX = nodes[nodeIdx]->_x + 1;
                tmpY = nodes[nodeIdx]->_y;
                if( intMap[tmpX][tmpY] == 4)
                {
                    printf("GOOOOL!!!\n");
                    goalParentId = nodes[nodeIdx]->_id;
                    done = true;
                    break;

                }
                else if( intMap[tmpX][tmpY] == 0 )
                {
                    //printf("Create node\n");
                    Node* node = new Node(tmpX,tmpY,nodes.size(),nodes[nodeIdx]->_id,getScore(tmpX,tmpY));
                    intMap[tmpX][tmpY] = 2;
                    nodes.push_back(node);
                }

                //printf("right\n");
                tmpX = nodes[nodeIdx]->_x;
                tmpY = nodes[nodeIdx]->_y + 1;
                if( intMap[tmpX][tmpY] == 4)
                {
                    printf("GOOOOL!!!\n");
                    goalParentId = nodes[nodeIdx]->_id;
                    done = true;
                    break;
                }
                else if( intMap[tmpX][tmpY] == 0 )
                {
                    //printf("Create node\n");
                    Node* node = new Node(tmpX,tmpY,nodes.size(),nodes[nodeIdx]->_id, getScore(tmpX,tmpY));
                    intMap[tmpX][tmpY] = 2;
                    nodes.push_back(node);
                }

                //printf("left\n");
                tmpX = nodes[nodeIdx]->_x;
                tmpY = nodes[nodeIdx]->_y - 1;
                if( intMap[tmpX][tmpY] == 4)
                {
                    printf("GOOOOL!!!\n");
                    goalParentId = nodes[nodeIdx]->_id;
                    done = true;
                    break;
                }
                else if( intMap[tmpX][tmpY] == 0 )
                {
                    //printf("Create node\n");
                    Node* node = new Node(tmpX,tmpY,nodes.size(),nodes[nodeIdx]->_id, getScore(tmpX,tmpY));
                    intMap[tmpX][tmpY] = 2;
                    nodes.push_back(node);
                }

            }
            this->dump();
        }

        printf("%%%%%%%%%%%%%%%%%%%%\n");
        bool ok = false;
        while( ! ok )
        {
            for(int nodeIdx=0;nodeIdx<nodes.size();nodeIdx++)
            {
                if( nodes[nodeIdx]->_id == goalParentId )
                {
                    nodes[nodeIdx]->dump();
                    goalParentId = nodes[nodeIdx]->_parentId;
                    if( goalParentId == 0)
                    {

                        ok = true;
                        printf("%%%%%%%%%%%%%%%%%%%%2\n");
                    }
                }
            }
        }

        return true;
    } //--main

private:
    std::vector<std::vector<int> > intMap;  //0empty,1wall,2visited,3start,4end
    std::vector<Node*> nodes;
    std::ifstream file;
};

int main()
{
    Program program;
    if( ! program.run() )
        return 1;
    return 0;
}
