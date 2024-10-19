#include <ncurses.h>
#include<iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Node
{
public:
    int x, y;
    char data;
    Node* next;
    Node* prev;
    Node* up;
    Node* down;

    bool key;
    bool door;
    bool coin;
    bool bomb;
    bool Isplayer;

    Node() 
    {
        x = y = 0;
        key = door = coin = bomb = Isplayer = false;
        data = '.';
        next = prev = up = down = nullptr;
    }
};

class Stack
{
private:
    struct Node 
    {
        char move;
        Node* next;
    };

    Node* top;
   

public:
    Stack() : top(nullptr) {}

	 int row=0;
 	 int col=10;

    void push(char move) 
    {
        Node* newNode = new Node();
        newNode->move = move;
        newNode->next = top;
        top = newNode;

        //mvprintw(row++, col, "Pushed: %c", move);
        refresh();
    }

    char pop() 
    {
        if (top == nullptr) 
        {
            mvprintw(row++, col, "No moves to undo!");
            refresh();
            return '\0';
        }

        char lastMove = top->move;
        Node* temp = top;
        top = top->next;
        delete temp;

        //mvprintw(row++, col, "Popped: %c", lastMove);
        refresh();
        return lastMove;
    }

    bool isEmpty() 
    {
        return top == nullptr;
    }

    void clear() 
    {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
       // mvprintw(row++, col, "Stack cleared.");
        refresh();
    }

    void printStack() 
    {
        Node* temp = top;
        if (temp == nullptr) {
            mvprintw(row++, col, "Stack is empty.");
            refresh();
            return;
        }

        mvprintw(row++, col, "Moves in stack:");
        while (temp != nullptr) {
            mvprintw(row++, col, "%c", temp->move);
            temp = temp->next;
        }
        refresh();
    }

    ~Stack()
    {
        clear();
    }
};
class Maze
{
public:
    Node* head;
    Node* tail;
    int size;
    char** initialState;

    Maze()
    {
        head = tail = nullptr;
        size = 0;
        initialState = nullptr;
    }

    void createMaze(int n) 
    {
        size = n;
        Node* prevRow = nullptr;
        Node* currRow = nullptr;

        initialState = new char*[size];
        for (int i = 0; i < size; ++i) {
            initialState[i] = new char[size];
            for (int j = 0; j < size; ++j) {
                initialState[i][j] = '.';
            }
        }

        for (int i = 0; i < n; ++i) {
            Node* prevCol = nullptr;

            for (int j = 0; j < n; ++j) {
                Node* newNode = new Node();
                newNode->x = i;
                newNode->y = j;

                if (prevCol != nullptr) {
                    prevCol->next = newNode;
                    newNode->prev = prevCol;
                }

                if (prevRow != nullptr) {
                    Node* temp = prevRow;
                    for (int k = 0; k < j; ++k) temp = temp->next;
                    temp->down = newNode;
                    newNode->up = temp;
                }

                prevCol = newNode;

                if (i == 0 && j == 0) head = newNode;
                if (i == n - 1 && j == n - 1) tail = newNode;
                if (j == 0) currRow = newNode;
            }
            prevRow = currRow;
        }
    }

    Node* NodePosition(int pos) 
    {
        int x = (pos - 1) / size;
        int y = (pos - 1) % size;

        if (x < 0 || x >= size || y < 0 || y >= size) {
            return nullptr;
        }

        Node* temp = head;
        for (int i = 0; i < x; ++i) {
            if (temp->down != nullptr) {
                temp = temp->down;
            } else {
                return nullptr;
            }
        }
        for (int j = 0; j < y; ++j) {
            if (temp->next != nullptr) {
                temp = temp->next;
            } else {
                return nullptr;
            }
        }

        return temp;
    }

    void setKeyAtNode(int pos) 
    {
        Node* temp = NodePosition(pos);
        int row = 10;
        int col = 0;

        if (temp != nullptr) {
            temp->key = true;
            initialState[temp->x][temp->y] = 'K';
            mvaddch(temp->x, temp->y, 'K');
            //mvprintw(row, col, "Key successfully placed at position (%d, %d)\n", temp->x, temp->y);
            refresh();
        } else {
            mvprintw(row, col, "Failed to place key at position %d\n", pos);
            refresh();
        }
    }

    void setDoorAtNode(int pos) 
    {
        Node* temp = NodePosition(pos);
        int row = 12; // Adjusted row value to prevent overlap
        int col = 0;

        if (temp != nullptr) {
            temp->door = true;
            initialState[temp->x][temp->y] = 'D';
            mvaddch(temp->x, temp->y, 'D');
            //mvprintw(row, col, "Door successfully placed at position (%d, %d)\n", temp->x, temp->y);
            refresh();
        } else {
            mvprintw(row, col, "Failed to place door at position %d\n", pos);
            refresh();
        }
    }

    void setBombAtNode(int pos) 
    {
        Node* temp = NodePosition(pos);
        int row = 14; // Adjusted row value to prevent overlap
        int col = 0;

        if (temp != nullptr) {
            temp->bomb = true;
            initialState[temp->x][temp->y] = 'B';
            mvaddch(temp->x, temp->y, 'B');
            //mvprintw(row, col, "Bomb successfully placed at position (%d, %d)\n", temp->x, temp->y);
            refresh();
        } else {
            mvprintw(row, col, "Failed to place bomb at position %d\n", pos);
            refresh();
        }
    }

    void setCoinAtNode(int pos) 
    {
        Node* temp = NodePosition(pos);
        int row = 16; // Adjusted row value to prevent overlap
        int col = 0;

        if (temp != nullptr) {
            temp->coin = true;
            initialState[temp->x][temp->y] = 'C';
            mvaddch(temp->x, temp->y, 'C');
            //mvprintw(row, col, "Coin successfully placed at position (%d, %d)\n", temp->x, temp->y);
            refresh();
        } else {
            mvprintw(row, col, "Failed to place coin at position %d\n", pos);
            refresh();
        }
    }

    void printMaze() 
    {
        Node* rowPtr = head;
        int row = 5; // Adjusted to start printing maze at a different row
        int col = 50;

        for (int i = 0; i < size + 2; ++i) 
        {
            mvprintw(row, col + 2 * i, "O");
        }
        row++;

        while (rowPtr != nullptr) {
            int x = col;

            mvprintw(row, x, "O");
            x += 2;

            Node* colPtr = rowPtr;
            while (colPtr != nullptr) {
                if (colPtr->key)
                    mvprintw(row, x, ".");
                else if (colPtr->door)
                    mvprintw(row, x, ".");
                else if (colPtr->bomb)
                    mvprintw(row, x, "B");
                else if (colPtr->coin)
                    mvprintw(row, x, "C");
                else if (colPtr->Isplayer)
                    mvprintw(row, x, "P");
                else
                    mvprintw(row, x, "%c", colPtr->data);

                colPtr = colPtr->next;
                x += 2;
            }
            
            mvprintw(row, x, "O");
            row++;
            
            rowPtr = rowPtr->down;
        }

        for (int i = 0; i < size + 2; ++i) {
            mvprintw(row, col + 2 * i, "O");
        }

        refresh();
    }

    void printInitialState() 
    {
        int row = 25; // Adjusted to print initial state below other outputs
        int col = 0;

        mvprintw(row, col, "Initial Maze State:");
        row += 1;

        for (int i = 0; i < size; ++i) {
            int x = col;
            for (int j = 0; j < size; ++j) {
                mvprintw(row, x, "%c ", initialState[i][j]);
                x += 2;
            }
            row++;
        }
        refresh();
    }

    ~Maze()
    {
        Node* currRow = head;

        while (currRow != nullptr) {
            Node* currNode = currRow;
            while (currNode != nullptr) {
                Node* nextNode = currNode->next;
                delete currNode;
                currNode = nextNode;
            }
            Node* nextRow = currRow->down;
            delete currRow;
            currRow = nextRow;
        }

        for (int i = 0; i < size; ++i) {
            delete[] initialState[i];
        }
        delete[] initialState;
    }
};
class Player {
public:
    Node* current;
    Node* previousNode;
    int moves, undo;
    bool keyFound, coinFound, doorFound;
    char lastMove;
    Stack S;
    int bombCounter;
    int score;

    Player() {
        current = nullptr;
        previousNode = nullptr;
        moves = undo = 0;
        keyFound = coinFound = doorFound = false;
        lastMove = ' ';
        bombCounter = 0;
        score = 0;
    }

    void putPlayer(Node* n, Maze& m) {
        current = n;
        previousNode = nullptr;
        current->Isplayer = true;
        m.initialState[n->x][n->y] = 'P';
    }

    bool isReverseMove(char move) {
        if ((lastMove == 'w' && move == 's') || (lastMove == 's' && move == 'w') ||
            (lastMove == 'a' && move == 'd') || (lastMove == 'd' && move == 'a')) {
            return true;
        }
        return false;
    }

    void checkBomb(Maze& m) {
        if (current->bomb) {
            mvprintw(22, 0, "BOOM! You stepped on a bomb. Game Over!");
            refresh();
            mvprintw(23, 0, "\nInitial Maze State:\n");
            m.printInitialState();
            refresh();
            endwin();
            exit(0);
        }
    }

    void collectCoin() {
        if (current->coin) {
            mvprintw(21, 50, "Coin collected at position (%d, %d)!", current->x, current->y);
            refresh();
            undo++;
            current->coin = false;
        }
    }

    void moveUp(Maze& m) {
        if (current->up != nullptr) {
            S.push('w');
            current->Isplayer = false;
            previousNode = current;
            current = current->up;
            current->Isplayer = true;
            moves--;
            lastMove = 'w';
            checkBomb(m);
        } else {
            mvprintw(22, 50, "Reached Boundary");
            refresh();
        }
    }

    void moveDown(Maze& m) {
        if (current->down != nullptr) {
            S.push('s');
            current->Isplayer = false;
            previousNode = current;
            current = current->down;
            current->Isplayer = true;
            moves--;
            lastMove = 's';
            checkBomb(m);
        } else {
            mvprintw(22, 50, "Reached Boundary");
            refresh();
        }
    }

    void moveLeft(Maze& m) {
        if (current->prev != nullptr) {
            S.push('a');
            current->Isplayer = false;
            previousNode = current;
            current = current->prev;
            current->Isplayer = true;
            moves--;
            lastMove = 'a';
            checkBomb(m);
        } else {
            mvprintw(22, 50, "Reached Boundary");
            refresh();
        }
    }

    void moveRight(Maze& m) {
        if (current->next != nullptr) {
            S.push('d');
            current->Isplayer = false;
            previousNode = current;
            current = current->next;
            current->Isplayer = true;
            moves--;
            lastMove = 'd';
            checkBomb(m);
        } else {
            mvprintw(22, 50, "Reached Boundary");
            refresh();
        }
    }

    void undoMove(Maze& m) {
        if (undo > 0 && !S.isEmpty()) {
            char lastMove = S.pop();
            switch (lastMove) {
                case 'w': moveDown(m); break;
                case 's': moveUp(m); break;
                case 'a': moveRight(m); break;
                case 'd': moveLeft(m); break;
            }
            undo--;
            moves++;
        } else {
            mvprintw(22, 50, "No undos left or no moves to undo!");
            refresh();
        }
    }

    void collectKey() {
        if (current->key) {
            keyFound = true;
            current->key = false;
            mvprintw(21, 50, "Key collected at position (%d, %d)!", current->x, current->y);
            refresh();
        }
    }

    void checkDoor() {
        if (current->door) {
            if (keyFound) {
                doorFound = true;
                mvprintw(21, 50, "You have reached the door at position (%d, %d)!", current->x, current->y);
            } else {
                mvprintw(21, 50, "You found the door, but you haven't collected the key yet!");
            }
            refresh();
        }
    }

    int calculateDistance(Node* node1, Node* node2) {
        return abs(node1->x - node2->x) + abs(node1->y - node2->y);
    }

    void MovePlayer(Maze& maze, Node* keyNode, Node* doorNode) {
        static bool trackingKey = true;
        static int previousDistance = calculateDistance(current, keyNode);

        int command;
        mvprintw(0, 0, "Moves left: %d", moves);
        mvprintw(1, 0, "Undos Left: %d", undo);
        mvprintw(2, 0, "Use arrow keys to move or 'u' to undo, 'q' to quit.");
        refresh();

        command = getch();

        if ((command == KEY_UP && lastMove == KEY_DOWN) || 
            (command == KEY_DOWN && lastMove == KEY_UP) ||
            (command == KEY_LEFT && lastMove == KEY_RIGHT) || 
            (command == KEY_RIGHT && lastMove == KEY_LEFT)) {
            mvprintw(3, 0, "You can't go back to the node you just came from!");
            refresh();
            return;
        }

        switch (command) {
            case KEY_UP: moveUp(maze); break;
            case KEY_DOWN: moveDown(maze); break;
            case KEY_LEFT: moveLeft(maze); break;
            case KEY_RIGHT: moveRight(maze); break;
            case 'u': undoMove(maze); break;
            case 'q': 
                mvprintw(3, 0, "Exiting movement control.");
                refresh();
                endwin();
                exit(0);
            default: 
                mvprintw(3, 0, "Invalid command! Use arrow keys or 'q' to quit.");
        }

        bombCounter++;

        if (moves <= 0) {
            mvprintw(4, 0, "You've run out of moves! Game Over!");
            refresh();
            maze.printInitialState();
            endwin();
            exit(0);
        }

        int currentDistance = trackingKey ? calculateDistance(current, keyNode) : calculateDistance(current, doorNode);

        if (currentDistance < previousDistance) {
            mvprintw(5, 0, "You're getting closer!");
        } else if (currentDistance > previousDistance) {
            mvprintw(5, 0, "You're moving further away!");
        } else {
            mvprintw(5, 0, "You're maintaining the same distance.");
        }

        refresh();
        previousDistance = currentDistance;
        collectCoin();
        collectKey();
        checkDoor();

        if (keyFound && trackingKey) {
            trackingKey = false;
            previousDistance = calculateDistance(current, doorNode);
        }
    }

    void displayKeyStatus() {
        int x = 1;
        int y = 1;

        if (keyFound) {
            mvprintw(y, x, "Key Status: Collected");
        } else {
            mvprintw(y, x, "Key Status: Not Collected");
        }
        refresh();
    }

    void displayDoorStatus() {
        int x = 2;
        int y = 1;

        if (doorFound) {
            mvprintw(y, x, "Door Status: Reached to Door");
        } else {
            mvprintw(y, x, "Door Status: Find me ;)");
        }
        refresh();
    }
};
void GAMEOVER(Maze& m, Player& p) {
    if (p.current->bomb) {
        mvprintw(0, 0, "GAME OVER! You stepped on a bomb!");
        mvprintw(1, 0, "\nInitial Maze State:\n");
        m.printInitialState(); 
        refresh(); 
        getch();   
        exit(0); 
    }

    if (p.current->door) {
        if (p.keyFound) {
            p.score = p.moves; 
            mvprintw(3, 0, "YOU WON! You collected the key and reached the door!");
            mvprintw(4, 0, "Your score = %d", p.score);
            mvprintw(5, 0, "\nInitial Maze State:\n");
            m.printInitialState(); 
            refresh(); 
            getch();   
            exit(0); 
        } else {
            mvprintw(3, 0, "You found the door, but you haven't collected the key yet! Keep looking for the key.");
        }
    }
}
class Game {
public:
    Maze maze;
    Player player;
    Node* keyNode;
    Node* doorNode;
    int size;
    int keyPos, doorPos, bombPos, playerPos;

    Game() : size(10), keyNode(nullptr), doorNode(nullptr) {}

    void selectDifficulty() 
    {
    int choice;
    clear(); // Clears the screen before displaying the options

    mvprintw(1, 0, "Select difficulty level:");
    mvprintw(2, 0, "1. Easy (10x10 grid)");
    mvprintw(3, 0, "2. Moderate (15x15 grid)");
    mvprintw(4, 0, "3. Hard (20x20 grid)");
    mvprintw(5, 0, "Enter your choice (1-3): ");
    refresh(); // Refresh the screen to show the printed output
    getch(); // Wait for user input to proceed 
    scanw("%d", &choice); // Read user input

    switch (choice) {
        case 1:
            size = 10;
            break;
        case 2:
            size = 15;
            break;
        case 3:
            size = 20;
            break;
        default:
            mvprintw(6, 0, "Invalid choice. Defaulting to Easy mode (10x10 grid).");
            size = 10;
            break;
    }
    
   
    refresh(); // Refresh to show the default message if needed
    clear();
    refresh();
    
}

    void setupGame() {
        maze.createMaze(size);
        srand(time(0));
        keyPos = rand() % (size * size - 10 + 1) + 10;

        do {
            doorPos = rand() % (size * size - 10 + 1) + 10;
        } while (keyPos == doorPos);

        do {
            bombPos = rand() % (size * size - 10 + 1) + 10;
        } while (keyPos == bombPos || doorPos == bombPos);

        int coinPos1, coinPos2;
        do {
            coinPos1 = rand() % (size * size - 10 + 1) + 10;
        } while (coinPos1 == keyPos || coinPos1 == doorPos || coinPos1 == bombPos);

        do {
            coinPos2 = rand() % (size * size - 10 + 1) + 10;
        } while (coinPos2 == keyPos || coinPos2 == doorPos || coinPos2 == bombPos || coinPos2 == coinPos1);

        do {
            playerPos = rand() % (size * size - 10 + 1) + 10;
        } while (playerPos == keyPos || playerPos == doorPos || playerPos == bombPos || playerPos == coinPos1 || playerPos == coinPos2);

        maze.setKeyAtNode(keyPos);
        maze.setDoorAtNode(doorPos);
        maze.setBombAtNode(bombPos);
        maze.setCoinAtNode(coinPos1); 
        maze.setCoinAtNode(coinPos2); 

        Node* playerNode = maze.NodePosition(playerPos);
        player.putPlayer(playerNode, maze);

        keyNode = maze.NodePosition(keyPos);
        doorNode = maze.NodePosition(doorPos);

        int PlayerToKey = player.calculateDistance(player.current, keyNode);
        int KeyToDoor = player.calculateDistance(keyNode, doorNode);
        int totalDistance = PlayerToKey + KeyToDoor;

        if (size == 10) {
            player.moves = totalDistance + 6;
            player.undo = 6;
        } else if (size == 15) {
            player.moves = totalDistance + 2;
            player.undo = 4;
        } else if (size == 20) {
            player.moves = totalDistance;
            player.undo = 1;
        }

        mvprintw(6, 0, "Initial distance between player and key: %d", PlayerToKey);
        mvprintw(7, 0, "Distance between Key to Door: %d", KeyToDoor);
        mvprintw(8, 0, "Moves available: %d", player.moves);
        refresh();
        getch();
    }

    void displayStatus() {
        maze.printMaze();
        mvprintw(0, 0, "______________________\n");
        player.displayKeyStatus();
        player.displayDoorStatus();
        mvprintw(1, 0, "______________________\n");
        refresh();
    }

    void Movement() {
        player.MovePlayer(maze, keyNode, doorNode);    
    }

    void checkGameOver() {
        GAMEOVER(maze, player);
    }

    void startGame() {
        while (true) {
            displayStatus();
            Movement();
            checkGameOver();
        }
    }
};
int main() 
{
    initscr();            // Start ncurses mode
    cbreak();             // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    noecho();             // Do not echo input characters
    curs_set(0);          // Hide the cursor

    Game game;
    game.selectDifficulty();  // Ask the user to choose a difficulty level
    game.setupGame();         // Setup the game based on the selected difficulty
    game.startGame();         // Start the game loop

    endwin();             // End ncurses mode

    return 0;
}
