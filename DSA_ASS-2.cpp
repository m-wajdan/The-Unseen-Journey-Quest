#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip> // For better formatting using iomanip

using namespace std; 
class Node//-----------------------------------// NODE CLASS
{
public:
    int x, y;
    char data;
    Node* next;
    Node* prev;
    Node* up;
    Node* down;

    bool key;
    bool door; // used
    bool coin; // used
    bool bomb; // used
    bool Isplayer; // used

    Node() {
        x = y = 0;
        key = door = coin = bomb = Isplayer = false;
        data = '.';
        next = prev = up = down = nullptr;
    }
};


class Stack//----------------------------------// STACK CLASS
{

    struct Node 
    {
        char move;
        Node* next;
    };

    Node * top;
public:
Stack() : top(nullptr) {}

void push(char move) 
    {
        Node* newNode = new Node();
        newNode->move = move;
        newNode->next = top;
        top = newNode;
    }
char pop() 
    {
        if (top == nullptr) {
            cout << "No moves to undo!" << endl;
            return '\0';  // Return null character if stack is empty
        }
        char lastMove = top->move;
        Node* temp = top;
        top = top->next;
        delete temp;
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
    }

~Stack()
    {
        clear();  // Ensure all nodes are deleted when stack is destroyed
    }

};


class Maze//-----------------------------------//MAZE CLASS
{
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

    Node* NodePosition(int pos) {
        int x = (pos - 1) / size;  // row number
        int y = (pos - 1) % size;  // column number

        // Validate position
        if (x < 0 || x >= size || y < 0 || y >= size) {
            return nullptr;
        }

        Node* temp = head;
        // Traverse rows
        for (int i = 0; i < x; ++i) {
            if (temp->down != nullptr) {
                temp = temp->down;
            } else {
                return nullptr;
            }
        }
        // Traverse columns
        for (int j = 0; j < y; ++j) {
            if (temp->next != nullptr) {
                temp = temp->next;
            } else {
                return nullptr;
            }
        }

        return temp;
    }

    void setKeyAtNode(int pos) {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) {
            temp->key = true;
            initialState[temp->x][temp->y] = 'K'; // Store key in initial state.
            // cout << "Key successfully placed at position (" << temp->x << ", " << temp->y << ")\n"; //---------------- > uncomment to show the coordinates for demo :)
        } else {
            cout << "Failed to place key at position " << pos << endl;
        }
    }

    void setDoorAtNode(int pos) {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) {
            temp->door = true;
            initialState[temp->x][temp->y] = 'D'; // Store door in initial state
            // cout << "Door successfully placed at position (" << temp->x << ", " << temp->y << ")\n"; //----------------> uncomment to show the cooordinates
        } else {
            cout << "Failed to place door at position " << pos << endl;
        }
    }

    void setBombAtNode(int pos) {
        Node* temp = NodePosition(pos);
        if (temp != nullptr) {
            temp->bomb = true;
            initialState[temp->x][temp->y] = 'B'; // Store bomb in initial state
            // cout << "Bomb successfully placed at position (" << temp->x << ", " << temp->y << ")\n";  //-----------------> uncomment to show the coordinates
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

    ~Maze() {
        for (int i = 0; i < size; ++i) {
            delete[] initialState[i];
        }
        delete[] initialState;
    }
};

class Player//---------------------------------//PLAYER CLASS
{
public:
    Node* current;
    Node* previousNode; // To store the previous node
    int moves, undo;
    bool keyFound, coinFound, doorFound;
    char lastMove;
    Stack S;

    Player() 
    {
        current = nullptr;
        previousNode = nullptr;
        moves = undo = 0;
        keyFound = coinFound = doorFound = false;
        lastMove = ' ';  // Initialize with an empty char
    }

    void putPlayer(Node* n, Maze& m) 
    {
        current = n;
        previousNode = nullptr;  // No previous node initially
        current->Isplayer = true;
        m.initialState[n->x][n->y] = 'P';  // Update initial state
    }
    bool isReverseMove(char move) 
    {
        if ((lastMove == 'w' && move == 's') || (lastMove == 's' && move == 'w') ||
            (lastMove == 'a' && move == 'd') || (lastMove == 'd' && move == 'a')) {
            return true;
        }
        return false;
    }

    void checkBomb(Maze& m) 
    {
        if (current->bomb) {
            cout << "BOOM! You stepped on a bomb. Game Over!" << endl;
            cout << "\nInitial Maze State:\n";
            m.printInitialState(); // Print the initial state of the maze
            exit(0); // End the game
        }
    }

    void moveUp(Maze& m) {
        if (current->up != nullptr) 
        {
            S.push('w');
            current->Isplayer = false;
            previousNode = current;
            current = current->up;
            current->Isplayer = true;
            moves--;
            lastMove = 'w'; // Set the last move direction
            checkBomb(m);
        } else {
            cout << "Reached Boundary\n";
        }
    }

    void moveDown(Maze& m) {
        if (current->down != nullptr) 
        {
            S.push('s');
            current->Isplayer = false;
            previousNode = current;
            current = current->down;
            current->Isplayer = true;
            moves--;
            lastMove = 's'; // Set the last move direction
            checkBomb(m);
        } else {
            cout << "Reached Boundary\n";
        }
    }

    void moveLeft(Maze& m) 
    {
        S.push('a');
        if (current->prev != nullptr) {
            current->Isplayer = false;
            previousNode = current;
            current = current->prev;
            current->Isplayer = true;
            moves--;
            lastMove = 'a'; // Set the last move direction
            checkBomb(m);
        } else {
            cout << "Reached Boundary\n";
        }
    }

    void moveRight(Maze& m) {
        if (current->next != nullptr) 
        {
            S.push('d');
            current->Isplayer = false;
            previousNode = current;
            current = current->next;
            current->Isplayer = true;
            moves--;
            lastMove = 'd'; // Set the last move direction
            checkBomb(m);
        } else {
            cout << "Reached Boundary\n";
        }
    }


    void undoMove(Maze& m) {
        if (undo > 0 && ! S.isEmpty()) {
            char lastMove = S.pop();
            switch (lastMove) {
                case 'w': moveDown(m); break;  // Reverse the previous move
                case 's': moveUp(m); break;
                case 'a': moveRight(m); break;
                case 'd': moveLeft(m); break;
            }
            undo--;  // Decrement available undos
            moves++;
        }
         else
        {
            cout << "No undos left or no moves to undo!\n";
        }
    }


    void collectKey() {
        if (current->key) {
            keyFound = true;
            current->key = false;  // Remove the key from the current node
            cout << "Key collected at position (" << current->x << ", " << current->y << ")!" << endl;
        }
    }

    void checkDoor() {
        if (current->door) {
            if (keyFound) {
                doorFound = true;
                cout << "You have reached the door at position (" << current->x << ", " << current->y << ")!" << endl;
            }
            else {
                cout << "You found the door, but you haven't collected the key yet! Keep looking for the key." << endl;
            }
        }
    }

    // Calculate Manhattan distance between two nodes
    int calculateDistance(Node* node1, Node* node2) {
        return abs(node1->x - node2->x) + abs(node1->y - node2->y);
    }

    void MovePlayer(Maze& maze, Node* keyNode, Node* doorNode) 
    {
        static bool trackingKey = true; // Track the key first
        static int previousDistance = calculateDistance(current, keyNode); // Initial distance to key

        char command;
        cout << "Moves left: " << moves << endl;
        cout<< "Undos Left: "<<undo<<endl;
        cout << "Enter command (w=up, s=down, a=left, d=right, u=undo ,q=quit): ";
        cin >> command;


         if (isReverseMove(command)) 
         {
            cout << "You can't go back to the node you just came from!" << endl;
            return;  // Prevent the move
        }

        

        switch (command) {
            case 'w': moveUp(maze); break;
            case 's': moveDown(maze); break;
            case 'a': moveLeft(maze); break;
            case 'd': moveRight(maze); break;
            case 'u': undoMove(maze);  // Handle the undo operation here
            break;
            case 'q': 
                cout << "Exiting movement control." << endl; 
                exit(0); // Exit the game loop
            default: 
                cout << "Invalid command! Use 'w', 'a', 's', 'd' to move or 'q' to quit." << endl;
        }

             if (moves <= 0) 
            {
            cout << "You've run out of moves! Game Over!" << endl;
            exit(0); // End the game
            }
        // After moving, check the new distance based on what we're tracking (key or door)
        int currentDistance;
        if (trackingKey) {
            currentDistance = calculateDistance(current, keyNode);
        } else {
            currentDistance = calculateDistance(current, doorNode);
        }

        // Compare distances and provide feedback
        if (currentDistance < previousDistance) {
            cout << "You're getting closer!" << endl;
        }
        else if (currentDistance > previousDistance) {
            cout << "You're moving further away!" << endl;
        }
        else {
            cout << "You're maintaining the same distance." << endl;
        }

        // Update the previous distance for the next move
        previousDistance = currentDistance;

        collectKey();
        checkDoor();

        // Once the key is collected, switch to tracking the door
        if (keyFound && trackingKey) {
            trackingKey = false; // Now track the door
            previousDistance = calculateDistance(current, doorNode); // Start measuring distance to door
        }
    }

    void displayKeyStatus() {
        if (keyFound) {
            cout << "Key Status: Collected\n";
        } else {
            cout << "Key Status: Not Collected\n";
        }
    }

    void displayDoorStatus() {
        if (doorFound) {
            cout << "Door Status: Reached to Door\n";
        } else {
            cout << "Door Status: Find me ;) \n";
        }
    }
};

// Separate GAMEOVER function outside the Player class
void GAMEOVER(Maze& m, Player& p) {
    if (p.current->bomb) {
        cout << "GAME OVER! You stepped on a bomb!" << endl;
        cout << "\nInitial Maze State:\n";
        m.printInitialState();
        exit(0); // End the game
    }

    // If the player reaches the door
    if (p.current->door) {
        // Check if the key was found first
        if (p.keyFound) {
            cout << "YOU WON! You collected the key and reached the door!" << endl;
            cout << "\nInitial Maze State:\n";
            m.printInitialState();
            exit(0); // End the game
        }
        else {
            cout << "You found the door, but you haven't collected the key yet! Keep looking for the key." << endl;
        }
    }
}

// Corrected clearScreen function with proper message
void clearScreen(Player& player) {
    if (player.keyFound) {
        cout << "You have collected the key!\n";
    }
    else {
        cout << "You have not collected the key yet.\n";
    }

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

class Game {                                                      // GAME CLASS
public:
    Maze maze;
    Player player;
    Node* keyNode;
    Node* doorNode;
    int size;
    int keyPos, doorPos, bombPos;

    Game() : size(10), keyNode(nullptr), doorNode(nullptr) {}

    void selectDifficulty() {
        int choice;
        cout << "Select difficulty level:\n";
        cout << "1. Easy (10x10 grid)\n";
        cout << "2. Moderate (15x15 grid)\n";
        cout << "3. Hard (20x20 grid)\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;

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
                cout << "Invalid choice. Defaulting to Easy mode (10x10 grid).\n";
                size = 10;
                break;
        }
    }

    void setupGame() 
    {
    maze.createMaze(size);

    // Set positions for key, door, bomb, and player
    srand(time(0));
    keyPos = rand() % (size * size - 10 + 1) + 10;

    do {
        doorPos = rand() % (size * size - 10 + 1) + 10;
    } while (keyPos == doorPos);

    do {
        bombPos = rand() % (size * size - 10 + 1) + 10;
    } while (keyPos == bombPos || doorPos == bombPos);

    // Generate a random position for the player
    int playerPos;
    do {
        playerPos = rand() % (size * size - 10 + 1) + 10;
    } while (playerPos == keyPos || playerPos == doorPos || playerPos == bombPos); // Ensure player's position is not on the key, door, or bomb

    maze.setKeyAtNode(keyPos);
    maze.setDoorAtNode(doorPos);
    maze.setBombAtNode(bombPos);

    // Place player at a random position
    Node* playerNode = maze.NodePosition(playerPos);
    player.putPlayer(playerNode, maze);  // Place the player on the randomly chosen node

    // Store references to key and door nodes
    keyNode = maze.NodePosition(keyPos);
    doorNode = maze.NodePosition(doorPos);

  // ** Calculate the initial distance between player and key **
    int initialDistance = player.calculateDistance(player.current, keyNode);

    // ** Adjust 'moves' based on difficulty level **
    if (size == 10) {  // Easy level
        player.moves = initialDistance + 6;
        player.undo = 6;
    } else if (size == 15) {  // Moderate level
        player.moves = initialDistance + 2;
        player.undo=4;
    } else if (size == 20) {  // Hard level
        player.moves = initialDistance;
        player.undo=1;
    }

    cout << "Initial distance between player and key: " << initialDistance << endl;
    cout << "Moves available: " << player.moves << endl;

}

    void displayStatus() {
        maze.printMaze();
        player.displayKeyStatus();
        player.displayDoorStatus();
        cout << "**********************\n";
    }

    void processInput() {
        player.MovePlayer(maze, keyNode, doorNode);
    }

    void checkGameOver() {
        // This checks if the player hit a bomb or reached the door
        GAMEOVER(maze, player);
    }

    void startGame() {
        while (true) {
            displayStatus();
            processInput();
            checkGameOver();
        }
    }

};

int main() 
{
    Game game;
    game.selectDifficulty();  // Ask the user to choose a difficulty level
    game.setupGame();         // Setup the game based on the selected difficulty
    game.startGame();         // Start the game loop

    return 0;
}