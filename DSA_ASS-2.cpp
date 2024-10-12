#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>

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
    bool door;//used
    bool coin;//used
    bool bomb;//used
    bool Isplayer;//used

    Node()
    {
        x = y = 0;
        key = door = coin = bomb = Isplayer = false;
        data = '.';
        next = prev = up = down = nullptr;
    }
};

class Maze {
public:
    Node* head;
    Node* tail;
    int size;
    char** initialState; // 2D array to store initial game state

    Maze() {
        head = tail = nullptr;
        size = 0;
        initialState = nullptr;
    }

    void createMaze(int n) {
        size = n;
        Node* prevRow = nullptr;
        Node* currRow = nullptr;

        // Allocate memory for the initial state array
        initialState = new char*[size];
        for (int i = 0; i < size; ++i) {
            initialState[i] = new char[size];
            for (int j = 0; j < size; ++j) {
                initialState[i][j] = '.'; // Initialize with '.'
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
        int x = (pos - 1) / size;  // row number
        int y = (pos - 1) % size;  // column number

        Node* temp = head;
        // Traverse rows
        for (int i = 0; i < x; ++i) {
            temp = temp->down;
        }
        // Traverse columns
        for (int j = 0; j < y; ++j) {
            temp = temp->next;
        }

        return temp;
}

    void setKeyAtNode(int pos) {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) {
            temp->key = true;
            initialState[temp->x][temp->y] = 'K'; // Store key in initial state
            cout << "Key successfully placed at position (" << temp->x << ", " << temp->y << ")\n";
        } else {
            cout << "Failed to place key at position " << pos << endl;
        }
    }

    void setDoorAtNode(int pos) {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) {
            temp->door = true;
            initialState[temp->x][temp->y] = 'D'; // Store door in initial state
            cout << "Door successfully placed at position (" << temp->x << ", " << temp->y << ")\n";
        } else {
            cout << "Failed to place door at position " << pos << endl;
        }
    }

    void setBombAtNode(int pos) {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) {
            temp->bomb = true;
            initialState[temp->x][temp->y] = 'B'; // Store bomb in initial state
            cout << "Bomb successfully placed at position (" << temp->x << ", " << temp->y << ")\n";
        } else {
            cout << "Failed to place bomb at position " << pos << endl;
        }
    }

    void printMaze() {
        Node* rowPtr = head;
        while (rowPtr != nullptr) {
            Node* colPtr = rowPtr;
            while (colPtr != nullptr) {
                if (colPtr->key)
                    cout << "K ";
                else if (colPtr->door)
                    cout << "D ";
                else if (colPtr->bomb) 
                    cout << "B "; 
                else if (colPtr->Isplayer)
                    cout << "P ";
                else
                    cout << colPtr->data << " ";

                colPtr = colPtr->next;
            }
            cout << endl;
            rowPtr = rowPtr->down;
        }
    }

    void printInitialState() {
        cout << "Initial Maze State:\n";
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << initialState[i][j] << " ";
            }
            cout << endl;
        }
    }

    ~Maze() 
    {
        for (int i = 0; i < size; ++i) {
            delete[] initialState[i];
        }
        delete[] initialState;
    }
};

class Player
{
public:
    Node* current;
    int moves, undo;
    bool keyFound, coinFound,doorFound;

    Player()
    {
        current = nullptr;
        moves = undo = 0;
        keyFound = coinFound =doorFound= false;
    }

   void putPlayer(Node* n, Maze& m)
    {
        current = n;
        current->Isplayer = true;
        // Set the initial position of the player in the initialState array
        m.initialState[n->x][n->y] = 'P';
    }

    void checkBomb(Maze& m) 
    {
        if (current->bomb) 
        {
            cout << "BOOM! You stepped on a bomb. Game Over!" << endl;
        cout << "\nInitial Maze State:\n";
        m.printInitialState(); // Print the initial state of the maze
        exit(0); // End the game
        }
    }

    void moveUp(Maze& m) 
    {
        if (current->up != nullptr) 
        {
            current->Isplayer = false;
            current = current->up;
            current->Isplayer = true;
            moves++;
            checkBomb(m);
        }
        else
            cout << "Reached Boundary\n";
    }

    void moveDown(Maze &m) 
    {
        if (current->down != nullptr) 
        {
            current->Isplayer = false;
            current = current->down;
            current->Isplayer = true;
            moves++;
            checkBomb(m);
        }
        else
            cout << "Reached Boundary\n";
    }

    void moveLeft(Maze& m) 
    {
        if (current->prev != nullptr)
        {
            current->Isplayer = false;
            current = current->prev;
            current->Isplayer = true;
            moves++;
            checkBomb(m);
        }
        else
            cout << "Reached Boundary\n";
    }

    void moveRight(Maze &m) 
    {
        if (current->next != nullptr)
        {
            current->Isplayer = false;
            current = current->next;
            current->Isplayer = true;
            moves++;
            checkBomb(m);
        }
        else
            cout << "Reached Boundary\n";
    }
    void collectKey() 
    {
        if (current->key) 
        {
            keyFound = true;
            current->key = false;  // Remove the key from the current node
            cout << "Key collected at position (" << current->x << ", " << current->y << ")!" << endl;
        }
    }
     void checkDoor() 
     {
        if (current->door) 
        {
            doorFound = true;
            current->door = false;  // Remove the door from the current node
            cout << "You have reached the door at position (" << current->x << ", " << current->y << ")!" << endl;
        }
    }
    void MovePlayer(Maze& maze) 
    {
        char command;
        cout << "Enter command (w=up, s=down, a=left, d=right, q=quit): ";
        cin >> command;

        switch (command) 
        {
            case 'w': moveUp(maze); break;
            case 's': moveDown(maze); break;
            case 'a': moveLeft(maze); break;
            case 'd': moveRight(maze); break;
            case 'q': cout << "Exiting movement control." << endl; return;
            default: cout << "Invalid command! Use 'w', 'a', 's', 'd' to move or 'q' to quit." << endl;
        }


        collectKey();
        checkDoor();;
    }
    void displayKeyStatus() 
    {
        if (keyFound) {
            cout << "Key Status: Collected\n";
        } else {
            cout << "Key Status: Not Collected\n";
        }
    }
    void displayDoorStatus()
    {
        if (doorFound) 
        {
            cout << "Door Status: Reached to Door\n";
        } else {
            cout << "Door Status: Find me ;) \n";
        }
    }

void GAMEOVER(Maze& m) 
{
    if (keyFound && doorFound) 
    {
        cout << "YOU WON! You collected the key and reached the door!" << endl;
        cout << "\nInitial Maze State:\n";
        m.printInitialState(); // Print the initial state of the maze
        exit(0); // End the game
    } 
    else if (current->bomb) 
    {
        cout << "GAME OVER! You stepped on a bomb!" << endl;
        cout << "\nInitial Maze State:\n";
        m.printInitialState(); // Print the initial state of the maze
        exit(0); // End the game
    }
}

};


void clearScreen(Player& player) 
{
    if (!player.keyFound) 
    {
        cout << "You have collected the key!\n";
        // player.keyFound = false;  // Reset the keyFound status after displaying the message
    }

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


int main()
{
    Maze maze;
    int size = 10;
    maze.createMaze(size);

    srand(time(0));
    int keyPos = rand() % ((size * size) - 10 + 1) + 10;
    int doorPos;
    int bombPos;
    do {
        doorPos = rand() % ((size * size) - 10 + 1) + 10;
    } while (keyPos == doorPos);

    do {
        bombPos = rand() % ((size * size) - 10 + 1) + 10;
    } while (keyPos == bombPos || doorPos == bombPos); // Ensure bomb is not placed on key or door

    maze.setKeyAtNode(keyPos);
    maze.setDoorAtNode(doorPos);
    maze.setBombAtNode(bombPos); // Place the bomb in the maze

    Player p;
    p.putPlayer(maze.head,maze);


    Node* keyNode = maze.NodePosition(keyPos);
    Node* doorNode = maze.NodePosition(doorPos);
    Node* bombNode = maze.NodePosition(bombPos);
    Node* playerNode = p.current;



    char turn = ' ';
    while (turn != 'q') 
    {
        clearScreen(p);  // Pass player object to clearScreen
                    // cout << "Key is placed at node number: " << keyPos << endl;
                    // cout << "Door is placed at node number: " << doorPos << endl;
                    // cout << "Bomb is placed at node number: " << bombPos << endl;

                    // // Print their coordinates
                    // cout << "Key is placed at coordinates: (" << keyNode->x << ", " << keyNode->y << ")" << endl;
                    // cout << "Door is placed at coordinates: (" << doorNode->x << ", " << doorNode->y << ")" << endl;
                    // cout << "Bomb is placed at coordinates: (" << bombNode->x << ", " << bombNode->y << ")" << endl;
                    // cout << "Player is at coordinates: (" << playerNode->x << ", " << playerNode->y << ")" << endl;
        maze.printMaze();
        p.displayKeyStatus();
        p.displayDoorStatus();
        p.MovePlayer(maze);
        p.GAMEOVER(maze);
    }

    return 0;
}
