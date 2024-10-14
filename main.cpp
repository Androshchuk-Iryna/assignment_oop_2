#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct ShapeDate{
    string name;
    string colour;
    string filling;
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
};

class fileMangnment{
public:
    static void save(const vector<vector<char>>& grid){ //implementation was taken from previous task(programming paradigms)
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

    static void load(vector<vector<char>>& grid){
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
    virtual void draw(Board& board, int x, int y, int size, string fill, char colour) const = 0;
};

class Triangle: public Shape{
public:
    void draw(Board& board, int x, int y, int height, string fill, char colour) const override {
        if (height <= 0) return;
        if (fill == "fill"){
            for (int i = 0; i < height; ++i) {
                int leftMost = x - i;
                int rightMost = x + i;
                int posY = y + i;

                if (posY < BOARD_HEIGHT) {
                    if (leftMost >= 0 && leftMost < BOARD_WIDTH)
                        board.grid[posY][leftMost] = colour;
                    if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
                        board.grid[posY][rightMost] = colour;
                }
            }
            for (int j = 0; j < 2 * height - 1; ++j) {
                int baseX = x - height + 1 + j;
                int baseY = y + height - 1;
                if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
                    board.grid[baseY][baseX] = colour;
            }
        }
        else if(fill == "frame"){
            for (int i = 0; i < height; ++i) {
                for (int j = x - i + 1; j < x + i; ++j) {
                    if (y + i < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH)
                        board.grid[y + i][j] = colour;
                }
            }
        }
    }
};

class Circle: public Shape{
public:
    void draw(Board& board, int x, int y, int radius, string fill, char colour) const override {
        if (radius <= 0) return;
        float dist;

        for (int i = 0; i <= 2 * radius; i++) {
            for (int j = 0; j <= 2 * radius; ++j) {
                dist = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));

                if (fill == "frame") {
                    if (dist <= radius + 0.5) {
                        int boardX = x - radius + j;
                        int boardY = y - radius + i;
                        if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
                            board.grid[boardY][boardX] = colour;
                        }
                    }
                }
                else if(fill == "fill"){
                    if (dist > radius - 0.5 && dist < radius + 0.5) {
                        int boardX = x - radius + j;
                        int boardY = y - radius + i;
                        if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
                            board.grid[boardY][boardX] = colour;
                        }
                    }
                }
            }
        }
    }
};

class Square: public Shape{
public:
    void draw(Board& board, int x, int y, int lenght, string fill, char colour) const override {
        if (lenght <= 0) return;

        if (fill == "fill"){
            for (int i = 0; i < lenght; ++i) {
                if (x + i < BOARD_WIDTH && y + i < BOARD_HEIGHT) {
                    board.grid[y][x + i] = colour;
                    board.grid[y + lenght - 1][x + i] = colour;
                }
                if (y + i < BOARD_HEIGHT && x + i < BOARD_WIDTH) {
                    board.grid[y + i][x] = colour;
                    board.grid[y + i][x + lenght - 1] = colour;
                }
            }
        }
        else if(fill == "frame"){
            for (int i = 1; i < lenght - 1; ++i) {
                for (int j = 1; j < lenght - 1; ++j) {
                    if (y + i < BOARD_HEIGHT && x + j < BOARD_WIDTH)
                        board.grid[y + i][x + j] = colour;
                }
            }
        }
    }

};
class Line: public Shape{
public:
    void draw(Board& board, int x, int y, int lenght, string fill, char colour) const override {
        if (lenght <= 0) return;
        for (int i = 0; i < lenght; ++i){
            if (x + i < BOARD_WIDTH && y < BOARD_HEIGHT) {
                board.grid[y+i][x + i] = colour;
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
    int ID = 0;

    int x,y, size;
    string shapeTipe;
    string colour;
    string command;
    string fill;

    while (true){
        cout << "Enter command (add, draw, clear, save, load, shapes, list, undo, exit): ";
        cin >> command;

        if (command == "add"){
            ID++;
            cin >> shapeTipe >> colour >> x >> y >> size >> fill;
            char cChar = colour[0];
            if (shapeTipe == "triangle"){
                triangle.draw(board, x, y, size, fill, cChar);
            }
            else if (shapeTipe == "circle"){
                circle.draw(board, x, y, size, fill, cChar);
            }
            else if (shapeTipe == "square"){
                square.draw(board, x, y, size, fill, cChar);
            }
            else if (shapeTipe == "line"){
                line.draw(board, x, y, size, fill, cChar);
            }
            shapesList.push_back({shapeTipe, colour, fill, x, y, size, ID});
        }
        else if (command == "draw"){
            board.print();
        }
        else if (command == "clear"){
            board.clear();
        }
        else if (command == "save") {
            fileMangnment::save(board.grid);
        } else if (command == "load") {
            fileMangnment::load(board.grid);
        }
        else if (command == "shapes"){
            cout << "All shapes:\n";
            cout << "Triangle: input x, y, height, fill/frame, colour\n";
            cout << "Circle: input x, y, radius, fill/frame, colour\n";
            cout << "Square: input x, y, lenght, fill/frame, colour\n";
            cout << "Line: input x, y, lenght, fill/frame, colour\n";
        }
        else if (command == "list"){
            cout << "List of all added shapes:\n";
            for (const auto& shape : shapesList) {
                cout << "ID: " << shape.id << ", Shape: " << shape.name
                     << ", X: " << shape.x << ", Y: " << shape.y << ", Size: " << shape.z
                     <<", Colour: "<< shape.colour << ", Fill/Frame: " << shape.filling<<"\n";
            }
        }
        else if(command == "undo"){
            if(!shapesList.empty()){
                shapesList.pop_back();
                board.clear();
                for (const auto& shape: shapesList){
                    if (shape.name == "triangle") {
                        triangle.draw(board, shape.x, shape.y, shape.z,  "", '*');
                    } else if (shape.name == "circle") {
                        circle.draw(board, shape.x, shape.y, shape.z,  "", '*');
                    } else if (shape.name == "square") {
                        square.draw(board, shape.x, shape.y, shape.z,  "", '*');
                    } else if (shape.name == "line") {
                        line.draw(board, shape.x, shape.y, shape.z,  "", '*');
                    }
                }
            } else{
                cout << "no shapes to delet";
            }
        }
        else if (command == "exit") {
            return 0;
        }
        else{
            cout << "Invalid" << endl;
        }

    }
}

//add triangle blue 10 5 5
//add circle 10 10 5
//add square 10 10 10
// add line 10 10 10
//add circle blue 10 10 5
//add triangle red 10 5 5 frame
//add circle red 10 10 5 frame
//add square blue 10 10 10 frame
