#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct ShapeDate{
    string name;
    int x, y, z;
    int id;

};

struct Board{
    vector<vector<char>> grid;
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

    void print() {
        for (auto& row : grid) {
            for (char c : row) {
                cout << c;
            }
            cout << "\n";
        }
    }
    void clear(){
        for (auto& row : grid) {
            for (char& c : row) {
                c = ' ';
            }
        }
    }
    void shapes(){
        cout << "All shapes:\n";
        cout << "Triangle: input x, y, height\n";
        cout << "Circle: input x, y, radius\n";
        cout << "Square: input x, y, lenght\n";
        cout << "Line: input x, y, lenght\n";
    }
    void save(){ //implementation was taken from previous task(programming paradigms)
        cout << "Enter file name to save: ";
        string filename;
        cin >> filename;

        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file\n";
            return;

        }
        for (auto& row : grid) {
            for (char c : row) {
                file << c;
            }
            file << '\n';
        }
        file.close();
    }
    void load(){
        cout << "Enter file name to save: ";
        string filename;
        cin >> filename;

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file\n";
            return;
        }
        string line;
        int row = 0;
        while (getline(file, line) && row < BOARD_HEIGHT) {
            for (int col = 0; col < line.size() && col < BOARD_WIDTH; ++col) {
                grid[row][col] = line[col];
            }
            row++;
        }

        file.close();
    }
};


class Shape{
public:
    virtual void draw(Board& board, int x, int y, int size) const = 0;
};
class Triangle: public Shape{
public:
    void draw(Board& board, int x, int y, int height) const override {

        if (height <= 0) return; // Ensure the triangle height is positive and sensible
        for (int i = 0; i < height; ++i) {
            int leftMost = x - i; // Calculate the starting position
            int rightMost = x + i; // Calculate the ending position
            int posY = y + i; // Calculate the vertical position
// Draw only the edges/border of the triangle
            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH) // Check bounds for left most position
                    board.grid[posY][leftMost] = '*';
                if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
// Check bounds for right most position
                    board.grid[posY][rightMost] = '*';
            }
        }
// Draw the base of the triangle separately
        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) // Check bounds for each position on the base
                board.grid[baseY][baseX] = '*';
        }
    }
};
class Circle: public Shape{
public:
    void draw(Board& board, int x, int y, int radius) const override {
        if (radius <= 0) return;
        float dist;

        for (int i = 0; i <= 2 * radius; i++) {
            for (int j = 0; j <= 2 * radius; ++j) {
                dist = sqrt((i - radius) * (i - radius) +
                            (j - radius) * (j - radius));

                if (dist > radius - 0.5 && dist < radius + 0.5) {
                    int boardX = x - radius + j;
                    int boardY = y - radius + i;
                    if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
                        board.grid[boardY][boardX] = '*';
                    }
                }
            }
        }
    }
};
class Square: public Shape{
public:
    void draw(Board& board, int x, int y, int lenght) const override {
        if (lenght <= 0) return;
        for (int i = 0; i < lenght; ++i) {
            if (x + i < BOARD_WIDTH && y + i < BOARD_HEIGHT) {
                board.grid[y][x + i] = '*';
                board.grid[y + lenght - 1][x + i] = '*';
            }
            if (y + i < BOARD_HEIGHT && x + i < BOARD_WIDTH) {
                board.grid[y + i][x] = '*';
                board.grid[y + i][x + lenght - 1] = '*';
            }
        }
    }

};
class Line: public Shape{
public:
    void draw(Board& board, int x, int y, int lenght) const override {
        if (lenght <= 0) return;
        for (int i = 0; i < lenght; ++i){
            if (x + i < BOARD_WIDTH && y < BOARD_HEIGHT) {
                board.grid[y+i][x + i] = '*';
            }
        }
    }
};



int main() {
    Board board;
    Triangle triangle;
    Circle circle;
    Square square;
    Line line;

    vector<ShapeDate> shapesList;
    int shapeID = 0;

    int x,y, size;
    string shapeTipe;
    string command;

    while (true){
        cout << "Enter command (add, draw, clear, save, load, shapes, list, undo, exit): ";
        cin >> command;

        if (command == "add"){
            shapeID++;
            cin >> shapeTipe >> x >> y >> size;
            if (shapeTipe == "triangle"){
                triangle.draw(board, x, y, size);
            }
            else if (shapeTipe == "circle"){
                circle.draw(board, x, y, size);
            }
            else if (shapeTipe == "square"){
                square.draw(board, x, y, size);
            }
            else if (shapeTipe == "line"){
                line.draw(board, x, y, size);
            }
            shapesList.push_back({shapeTipe, x, y, size, shapeID});

        }
        else if (command == "draw"){
            board.print();
        }
        else if (command == "clear"){
            board.clear();
        }
        else if (command == "save"){
            board.save();
        }
        else if (command == "load"){
            board.load();
        }
        else if (command == "shapes"){
            board.shapes();
        }
        else if (command == "list"){
            cout << "List of all added shapes:\n";
            for (const auto& shape : shapesList) {
                cout << "ID: " << shape.id << ", Shape: " << shape.name
                     << ", X: " << shape.x << ", Y: " << shape.y << ", Size: " << shape.z << "\n";
            }
        }
        else{
            cout << "Invalid" << endl;
        }

    }
}

//add triangle 10 5 5
//add circle 10 10 5
//add square 10 10 10
// add line 10 10 10
