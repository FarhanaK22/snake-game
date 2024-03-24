#include <windows.h>
#include <process.h>
#include <cstdlib> 
#include <iostream> 
#include<stdio.h>
#include<random>
#include<conio.h>
#include<unistd.h>
using namespace std; 

#define Max_length 1000

//Directions 
const char dir_Up = 'w';
const char dir_Down = 's';
const char dir_Right = 'd';
const char dir_Left = 'a';

int consoleWidth, consoleHeight;

void initScreen()
{  //Using predefined class
    HANDLE hConsole =  GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole,&csbi);

    //Width & Height of console window
    consoleHeight = csbi.srWindow.Bottom- csbi.srWindow.Top+1;
    consoleWidth = csbi.srWindow.Right -csbi.srWindow.Left +1;
}

struct Point 
{
    int x_coord;
    int y_coord;

    Point()
    {

    }
    Point( int x, int y)
    {
        x_coord = x;
        y_coord = y;
    }
};

class Snake{
    int length;
    char direction;
    public:

    Point body[Max_length];
    Snake ( int x, int y)
    {   //initial length of snake
        length =1;
        // starting point of snakes body
        body [0] = Point (x,y); 
        //initial direction of snake
        direction = dir_Right;
    }
    ~Snake(){
        delete[] body;
    }
    int get_length(){
        return length;
    }
    void change_dir(char new_dir)
    {
        if(new_dir == dir_Up && direction != dir_Down)
        {
            direction = new_dir;
        }
        else if(new_dir == dir_Down && direction != dir_Up)
        {
            direction = new_dir;
        }
        else if(new_dir == dir_Left && direction != dir_Right)
        {
            direction = new_dir;
        }
        else if(new_dir == dir_Right && direction != dir_Left)
        {
            direction = new_dir;
        }
    }
    //function to move body of snake
    bool move(Point food){
        for(int i=length -1;i>0;i--)
        {
            body[i]= body [i-1];
        }
        switch(direction)
        { int val;
            case dir_Up :
            val = body[0].y_coord;
            body[0].y_coord = val-1;
            break;
            case dir_Down :
            val = body[0].y_coord;
            body[0].y_coord = val+1;
            break;
            case dir_Right :
            val = body[0].x_coord;
            body[0].x_coord = val+1;
            break;
            case dir_Left :
            val = body[0].x_coord;
            body[0].x_coord = val-1;
            break;
        }

        //to adjust body of snake when it reaches any corner
        Point head = body[0];

            // Check if the head is at the left edge
            if (head.x_coord == 0) {
                body[0].x_coord = consoleWidth;
            }
            // Check if the head is at the right edge
            else if (head.x_coord == consoleWidth) {
                body[0].x_coord = 0;
            }
            // Check if the head is at the top edge
            else if (head.y_coord == 0) {
                body[0].y_coord = consoleHeight;
            }
            else if (head.y_coord == consoleHeight) {
                body[0].y_coord = 0;
            }
        
        //when snake bites itself 
        for(int i =1;i<length;i++)
        {
            if(body[0].x_coord == body[i].x_coord && body[0].y_coord == body[i].y_coord )
            return false;
        }

        //Snake eats food
        if(food.x_coord == body[0].x_coord && food.y_coord == body[0].y_coord)
        {
            body[length]= Point (body[length-1].x_coord,body[length-1].y_coord);
            length ++;
        }
        return true;
    }
};

class Board{
    Snake *snake;
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = '*';
    int score;
    public:

    Board(){
        spawnFood();
        //initializing snake with coordinates
        snake = new Snake(10,10);
        score = 0;
    }
    ~Board(){
        delete snake;
    }
    int get_score()
    {
        return score;
    }
    void display_score()
    {   cout<<endl;
        gotoxy(consoleWidth/2,0);
        cout<<"CURRENT SCORE : "<<score;
    }
    void spawnFood(){
        int x = rand() % consoleWidth;
        int y = rand() % consoleHeight;
        food = Point (x,y);
    }
    // gotoxy uses console property to get the cursur to the coordinates
    void gotoxy(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
    }
    void draw()
    {
        system("cls");
        for(int i=0;i<snake->get_length();i++)
        {
            gotoxy(snake->body[i].x_coord,snake->body[i].y_coord);
            cout<<SNAKE_BODY;
        }
        gotoxy(food.x_coord,food.y_coord);
        cout<<FOOD;
        display_score();
    }
    bool update(){
       bool isAlive =  snake->move(food);
       if( isAlive == false)
       {
        return false;
       }
        if(food.x_coord == snake->body[0].x_coord && food.y_coord == snake->body[0].y_coord)
        {
            score ++;
            spawnFood();
        }
        return true;
    }
    void get_Input()
    {
        if(_kbhit())
        {
            int key = _getch();
            if(key == 'w' || key == 'W')
            {
                snake->change_dir(dir_Up);
            }
            else  if(key == 'a' || key == 'A')
            {
                snake->change_dir(dir_Left);
            }
            else  if(key == 's' || key == 'S')
            {
                snake->change_dir(dir_Down);
            }
            else  if(key == 'd' || key == 'D')
            {
                snake->change_dir(dir_Right);
            }
            //if any arrow key pressed 
            else{
                    switch (key) {
                        case 72: // Up arrow
                            snake->change_dir(dir_Up);
                            break;
                        case 80: // Down arrow
                            snake->change_dir(dir_Down);
                            break;
                        case 75: // Left arrow
                            snake->change_dir(dir_Left);
                            break;
                        case 77: // Right arrow
                            snake->change_dir(dir_Right);
                            break;
                        default:
                            // Handle other arrow keys if needed
                            break;
                    }
            }
        }
    }
};
void ShowGameWinMessage() {
    MessageBoxA(NULL, "CONGRATULATIONS!\nSCORE = 100\nLevel completed", "Snake Game", MB_ICONINFORMATION);
}
void ShowGameOverMessage(int x) {
    std::string scoreString = std::to_string(x);

    // Create the message
    std::string message = "GAME OVER\nYour Final score is: " + scoreString;

    // Display the message box
    MessageBoxA(NULL, message.c_str(), "Snake Game", MB_ICONINFORMATION);
}
int main()
{
    initScreen();
    Board *board = new Board();
    int x;
    int flag =0;
    while(true)
    {  if (!_kbhit()) {
            // No keyboard input, move the snake
            if (!board -> update()) {
            x= board->get_score();
            break;
            }
            board->draw();
            Sleep(100); // Delay for smoother animation
        }
        if(board->get_score()==100)
        { 
           flag=1;
            break;
        }
        board->get_Input();
    }
    if (flag==0)
    ShowGameOverMessage( x);
    else
    ShowGameWinMessage();

    return 0;
}
