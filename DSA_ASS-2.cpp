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

class Maze
{
public:
    Node* head;
    Node* tail;
    int size;

    Maze()
    {
        head = tail = nullptr;
        size = 0;
    }

    void createMaze(int n) 
    {
        size = n;
        Node* prevRow = nullptr;
        Node* currRow = nullptr;

        for (int i = 0; i < n; ++i) 
        {
            Node* prevCol = nullptr;

            for (int j = 0; j < n; ++j) 
            {
                Node* newNode = new Node();
                newNode->x = i;
                newNode->y = j;

                if (prevCol != nullptr) 
                {
                    prevCol->next = newNode;
                    newNode->prev = prevCol;
                }

                if (prevRow != nullptr) 
                {
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
        if (pos < 1 || pos > size * size) 
            return nullptr;

        Node* temp = head;
        int count = 1;

        while (temp != nullptr && count < pos) 
        {
            if (temp->next != nullptr) {
                temp = temp->next;
            } else {
                temp = temp->down;
                if (temp != nullptr) {
                    count++;
                    while (temp->prev != nullptr) {
                        temp = temp->prev;
                    }
                }
            }
            count++;
        }
        return temp;
    }

    void setKeyAtNode(int pos)
    {
        Node* temp = NodePosition(pos);
        if (temp != nullptr)
        {
            temp->key = true;
            cout << "Key successfully placed at position (" << temp->x << ", " << temp->y << ")\n";
        }
        else
        {
            cout << "Failed to place key at position " << pos << endl;
        }
    }

    void setDoorAtNode(int pos)
    {
        Node* temp = NodePosition(pos);
        if (temp != nullptr)
        {
            temp->door = true;
            cout << "Door successfully placed at position (" << temp->x << ", " << temp->y << ")\n";
        }
        else
        {
            cout << "Failed to place door at position " << pos << endl;
        }
    }

    void setBombAtNode(int pos) 
    {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) 
        {
            temp->bomb = true;
            cout << "Bomb successfully placed at position (" << temp->x << ", " << temp->y << ")\n";
        }
        else
        {
            cout << "Failed to place bomb at position " << pos << endl;
        }
    }

    void printMaze() 
    {
        Node* rowPtr = head;
        while (rowPtr != nullptr) 
        {
            Node* colPtr = rowPtr;
            while (colPtr != nullptr) 
            {
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
};

class Player
{
public:
    Node* current;
    int moves, undo;
    bool keyFound, coinFound;

    Player()
    {
        current = nullptr;
        moves = undo = 0;
        keyFound = coinFound = false;
    }

    void putPlayer(Node* n)
    {
        current = n;
        current->Isplayer = true;
    }

    void checkBomb() 
    {
        if (current->bomb) 
        {
            cout << "BOOM! You stepped on a bomb. Game Over!" << endl;
            exit(0); // End the game
        }
    }

    void moveUp() 
    {
        if (current->up != nullptr) 
        {
            current->Isplayer = false;
            current = current->up;
            current->Isplayer = true;
            moves++;
            checkBomb();
        }
        else
            cout << "Reached Boundary\n";
    }

    void moveDown() 
    {
        if (current->down != nullptr) 
        {
            current->Isplayer = false;
            current = current->down;
            current->Isplayer = true;
            moves++;
            checkBomb();
        }
        else
            cout << "Reached Boundary\n";
    }

    void moveLeft() 
    {
        if (current->prev != nullptr)
        {
            current->Isplayer = false;
            current = current->prev;
            current->Isplayer = true;
            moves++;
            checkBomb();
        }
        else
            cout << "Reached Boundary\n";
    }

    void moveRight() 
    {
        if (current->next != nullptr)
        {
            current->Isplayer = false;
            current = current->next;
            current->Isplayer = true;
            moves++;
            checkBomb();
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
    void MovePlayer(Maze& maze) 
    {
        char command;
        cout << "Enter command (w=up, s=down, a=left, d=right, q=quit): ";
        cin >> command;

        switch (command) 
        {
            case 'w': moveUp(); break;
            case 's': moveDown(); break;
            case 'a': moveLeft(); break;
            case 'd': moveRight(); break;
            case 'q': cout << "Exiting movement control." << endl; return;
            default: cout << "Invalid command! Use 'w', 'a', 's', 'd' to move or 'q' to quit." << endl;
        }


        collectKey();
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
    int size = 15;
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
    p.putPlayer(maze.head);

    char turn = ' ';
    while (turn != 'q') 
    {
        clearScreen(p);  // Pass player object to clearScreen
        cout << "Key is placed at node number: " << keyPos << endl;
        cout << "Door is placed at node number: " << doorPos << endl;
        cout << "Bomb is placed at node number: " << bombPos << endl;
        maze.printMaze();
        p.MovePlayer(maze);
    }

    return 0;
}
