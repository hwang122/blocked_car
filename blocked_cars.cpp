#include <iostream>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

//car on the chessboard
class car {
private:
    int length;
    int x_pos;
    int y_pos;
    int direct;
public:
    car();
    car(int s, int x, int y, int d);
    car(const car &c);
    bool isValid(int height, int width);
    bool isTargetValid(int exit_x, int exit_y);

    void moveUp() {x_pos--;};
    void moveDown() {x_pos++;};
    void moveLeft() {y_pos--;};
    void moveRight() {y_pos++;};

    int getLength() const{return length;};
    int getXpos() const{return x_pos;};
    int getYpos() const{return y_pos;};
    int getDirect() const{return direct;};

    void setXpos(int x){x_pos = x;};
    void setYpos(int y){y_pos = y;};
    ~car(){};
};
//default constructor
car::car()
{
    length = 0;
    x_pos = -1;
    y_pos = -1;
    direct = -1;
}
//constructor
car::car(int s, int x, int y, int d)
{
    length = s;
    x_pos = x;
    y_pos = y;
    direct = d;
}
//copy constructor
car::car(const car &c)
{
    length = c.getLength();
    x_pos = c.getXpos();
    y_pos = c.getYpos();
    direct = c.getDirect();
}
//check input car is valid
bool car::isValid(int height, int width)
{
    //check every car's parameter
    if(length < 2 || length > 3)
        return false;
    if(direct == 0 &&
        (x_pos < 0 || x_pos > height-1 ||
        y_pos < length-1 || y_pos > width-1))
        return false;
    if(direct == 1 &&
        (x_pos < 0 || x_pos > height-length ||
        y_pos < 0 || y_pos > width-1))
        return false;
    return true;
}
//check target car is valid
bool car::isTargetValid(int exit_x, int exit_y)
{
    if(direct == 0 && x_pos != exit_x)
        return false;
    if(direct == 1 && y_pos != exit_y)
        return false;
    return true;
}


//chessboard of the game
class chessboard {
private:
    int height;
    int width;
    int exit_x;
    int exit_y;
    int **board;
    vector<car> cars;
    vector<vector<int> > moves;
public:
    chessboard(int h, int w, int x, int y);
    chessboard(const chessboard &cboard);
    bool placeCar(vector<car> c);
    void placeOneCar(char c, int index);
    bool isSolved();
    void moveCar(vector<chessboard> &states);

    int getHeight() const{return height;};
    int getWidth() const{return width;};
    int getExitX() const{return exit_x;};
    int getExitY() const{return exit_y;};
    int **getBoard() const{return board;};
    vector<car> getCars() const{return cars;};
    vector<vector<int> > getMoves() const{return moves;};

    //size_t hashValue() const;
    bool operator==(const chessboard &c) const;
    ~chessboard();
};
//constructor
chessboard::chessboard(int h, int w, int x, int y)
{
    height = h;
    width = w;
    exit_x = x;
    exit_y = y;
    //initialize board
    board = new int*[height];
    for(int i = 0; i < height; i++)
    {
        board[i] = new int[width];
    }
    //initialize value of board
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            board[i][j] = 0;
}
//copy constructor
chessboard::chessboard(const chessboard &cboard)
{
    height = cboard.height;
    width = cboard.width;
    exit_x = cboard.exit_x;
    exit_y = cboard.exit_y;

    cars = cboard.cars;
    moves = cboard.moves;

    board = new int*[height];
    for(int i = 0; i < height; i++)
    {
        board[i] = new int[width];
    }
    //initialize value of board
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            board[i][j] = cboard.board[i][j];
}
//initialize chessboard
bool chessboard::placeCar(vector<car> c)
{
    cars = c;
    //place cars onto the chessboard and check the validation
    for(int i = 0; i < (signed)cars.size(); i++)
    {
        if(!cars[i].isValid(height, width))
            return false;

        //check target car's position, straight towards exit
        if(i == 0 && !cars[i].isTargetValid(exit_x, exit_y))
            return false;

        int x = cars[i].getXpos();
        int y = cars[i].getYpos();
        for(int j = 0; j < cars[i].getLength(); j++)
        {
            if(cars[i].getDirect() == 0)
            {
                //set position on the board to be index of car
                if(y - j >= 0 && board[x][y - j] == 0)
                    board[x][y-j] = i+1;
                else
                    //this position is invalid, return false to end game
                    return false;
            }
            else
            {
                if(x + j < height && board[x+j][y] == 0)
                    board[x+j][y] = i+1;
                else
                    return false;
            }
        }
    }

    return true;
}
//move one car
void chessboard::placeOneCar(char c, int index)
{
    int x = cars[index].getXpos();
    int y = cars[index].getYpos();
    int length = cars[index].getLength();

    switch(c)
    {
        //move up
        case 'u':
        {
            board[x-1][y] = index+1;
            board[x+length-1][y] = 0;
            cars[index].moveUp();
            break;
        }
        //move down
        case 'd':
        {
            board[x][y] = 0;
            board[x+length][y] = index+1;
            cars[index].moveDown();
            break;
        }
        //move left
        case 'l':
        {
            board[x][y-length] = index+1;
            board[x][y] = 0;
            cars[index].moveLeft();
            break;
        }
        //move right
        case 'r':
        {
            board[x][y-length+1] = 0;
            board[x][y+1] = index+1;
            cars[index].moveRight();
            break;
        }
        default:
            break;
    }
}
//check whether target car reaches exit
bool chessboard::isSolved()
{
    car target = cars[0];
    if(target.getDirect() == 0)
    {
        if(target.getYpos() == exit_y ||
           target.getYpos() == exit_y + target.getLength() - 1)
            return true;
    }
    else
    {
        if(target.getXpos() == exit_x ||
           target.getXpos() == exit_x + target.getLength() - 1)
            return true;
    }
    return false;
}
//move cars and save the current chessboard states
void chessboard::moveCar(vector<chessboard> &states)
{
    for(int i = 0; i < (signed)cars.size(); i++)
    {
        car c = cars[i];
        int length = c.getLength();

        //move up
        if(c.getDirect() == 1 && c.getXpos()-1 >= 0 &&
           board[c.getXpos()-1][c.getYpos()] == 0)
        {
            chessboard temp = *this;
            temp.placeOneCar('u', i);
            int parameter[] = {i+1, 0, 1};
            temp.moves.push_back(vector<int>(parameter, parameter+3));
            states.push_back(temp);
        }

        //move down
        if(c.getDirect() == 1 && c.getXpos()+length < height &&
           board[c.getXpos()+length][c.getYpos()] == 0)
        {
            chessboard temp = *this;
            temp.placeOneCar('d', i);
            int parameter[] = {i+1, 1, 1};
            temp.moves.push_back(vector<int>(parameter, parameter+3));
            states.push_back(temp);
        }

        //move left
        if(c.getDirect() == 0 && c.getYpos()-length >= 0 &&
           board[c.getXpos()][c.getYpos()-length] == 0)
        {
            chessboard temp = *this;
            temp.placeOneCar('l', i);
            int parameter[] = {i+1, 1, 1};
            temp.moves.push_back(vector<int>(parameter, parameter+3));
            states.push_back(temp);
        }

        //move right
        if(c.getDirect() == 0 && c.getYpos()+1 < width &&
           board[c.getXpos()][c.getYpos()+1] == 0)
        {
            chessboard temp = *this;
            temp.placeOneCar('r', i);
            int parameter[] = {i+1, 0, 1};
            temp.moves.push_back(vector<int>(parameter, parameter+3));
            states.push_back(temp);
        }
    }
}

bool chessboard::operator==(const chessboard &c) const
{
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            if(this->board[i][j] != c.board[i][j])
                return false;
    //return memcmp(board, c.board, sizeof(board)) == 0;
    return true;
}

chessboard::~chessboard()
{
    for(int i = 0; i < height; i++)
    {
        delete board[i];
    }
    delete board;
}



class solution {
public:
    //use BFS to find fastest solution
    void fastSolution(chessboard &board);
    void allSolution(chessboard &board);
};

void solution::fastSolution(chessboard &board)
{
	//store patterns of chessboard to remove unnecessary moves
    vector<chessboard> pattern;
	//queue to store chessboard states
    queue<chessboard> stateQ;
	//store chessborad after next moving generated by moving cars on chessboard
    vector<chessboard> nextMove;

    stateQ.push(board);
    pattern.push_back(board);
	
	//there is possible movings
    while(!stateQ.empty())
    {
        chessboard curr = stateQ.front();
        stateQ.pop();
		
		//if problem is solved
        if(curr.isSolved())
        {
            cout<<endl<<"Solution: "<<endl;
            int cumulate = 1, i = 0;
            for(; i < (signed)curr.getMoves().size()-1; i++)
            {
                if(curr.getMoves()[i][0] == curr.getMoves()[i+1][0] &&
                   curr.getMoves()[i][1] == curr.getMoves()[i+1][1])
                {
                    cumulate++;
                }
                else
                {
                    cout<<curr.getMoves()[i][0]<<" ";
                    cout<<curr.getMoves()[i][1]<<" ";
                    cout<<cumulate<<endl;
                    cumulate = 1;
                }
            }
            cout<<curr.getMoves()[i][0]<<" ";
            cout<<curr.getMoves()[i][1]<<" ";
            cout<<cumulate<<endl;
            //cout<<"can be solved"<<endl;
            return;
        }

        curr.moveCar(nextMove);
        for(chessboard &next : nextMove)
        {
            bool appear = false;
            //check whether chessboard happened before
            for(int i = 0; i < (signed)pattern.size(); i++)
            {
                if(next == pattern[i])
                {
                    appear = true;
                    break;
                }
            }
            //pattern is not exist before
            if(!appear)
            {
                //cout<<"pattern not found"<<endl;
                pattern.push_back(next);
                stateQ.push(next);
            }
        }
    }
    cout<<"There is no solution, can't solve"<<endl;
}

void solution::allSolution(chessboard &board)
{
    vector<chessboard> pattern;
    queue<chessboard> stateQ;
    vector<chessboard> nextMove;

    stateQ.push(board);
    pattern.push_back(board);

    int num_solution = 1;
    while(!stateQ.empty())
    {
        chessboard curr = stateQ.front();
        stateQ.pop();

        if(curr.isSolved())
        {
            cout<<endl<<"Solution #"<<num_solution<<endl;
            int cumulate = 1, i = 0;
            for(; i < (signed)curr.getMoves().size()-1; i++)
            {
                if(curr.getMoves()[i][0] == curr.getMoves()[i+1][0] &&
                   curr.getMoves()[i][1] == curr.getMoves()[i+1][1])
                {
                    cumulate++;
                }
                else
                {
                    cout<<curr.getMoves()[i][0]<<" ";
                    cout<<curr.getMoves()[i][1]<<" ";
                    cout<<cumulate<<endl;
                    cumulate = 1;
                }
            }
            cout<<curr.getMoves()[i][0]<<" ";
            cout<<curr.getMoves()[i][1]<<" ";
            cout<<cumulate<<endl;
            num_solution++;
            continue;
        }

        curr.moveCar(nextMove);
        for(chessboard &next : nextMove)
        {
            bool appear = false;
            //check whether chessboard happened before
            for(int i = 0; i < (signed)pattern.size(); i++)
            {
                if(next == pattern[i])
                {
                    appear = true;
                    break;
                }
            }
            //pattern is not exist before
            if(!appear)
            {
                //cout<<"pattern not found"<<endl;
                pattern.push_back(next);
                stateQ.push(next);
            }
        }
    }
}

int main(){
    //height and width of board
    int m, n;
    cout<<"Input height and width of chessborad: ";
    cin>>m>>n;
    //exit position
    int x, y;
    cout<<"Input exit position: ";
    cin>>x>>y;
    chessboard cboard(m, n, x, y);

    //number of cars
    int k;
    cout<<"Input number of cars: ";
    cin>>k;
    //list of cars
    vector<car> cars(k, car());
    int s, car_x, car_y, d;
    cout<<"Input parameters of car: "<<endl;
    for(int i = 0; i < k; i++)
    {
        cin>>s>>car_x>>car_y>>d;
        cars[i] = car(s, car_x, car_y, d);
    }

    //place cars onto board
    if(!cboard.placeCar(cars))
    {
        cout<<"Invalid set of cars"<<endl;
        return -1;
    }

    int choose = -1;
    while(choose != 0)
    {
        cout<<endl<<"1. Output one solution"<<endl;
        cout<<"2. Output solution with the fewest moves"<<endl;
        cout<<"3. Output all solutions"<<endl;
        cout<<"0. Exit"<<endl;
        cin>>choose;

        solution *result = new solution();
        switch(choose)
        {
        case 1:
            result->fastSolution(cboard);
            break;
        case 2:
            result->fastSolution(cboard);
            break;
        case 3:
            result->allSolution(cboard);
            break;
        default:
            break;
        }
        delete result;
    }

    return 0;
}
