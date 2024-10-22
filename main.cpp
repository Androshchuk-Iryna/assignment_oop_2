#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct Board;

class Shape {
public:
    virtual void draw(Board& board, int x, int y, int size, string fill, char colour) const = 0;
};

class Triangle : public Shape {
public:
    void draw(Board& board, int x, int y, int height, string fill, char colour) const override;
};

class Circle : public Shape {
public:
    void draw(Board& board, int x, int y, int radius, string fill, char colour) const override;
};

class Square : public Shape {
public:
    void draw(Board& board, int x, int y, int length, string fill, char colour) const override;
};

class Line : public Shape {
public:
    void draw(Board& board, int x, int y, int length, string fill, char colour) const override;
};

struct ShapeDate{
    string name;
    string colour;
    string filling;
    int x, y, z;
    int id;

};

struct Board{
    Triangle triangle;
    Circle circle;
    Square square;
    Line line;
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
    void draw(const string& shapeType, int x, int y, int size, const string& fill, const string& colour) {
        char cChar = colour[0];

        if (shapeType == "triangle") {
            triangle.draw(*this, x, y, size, fill, cChar);
        } else if (shapeType == "circle") {
            circle.draw(*this, x, y, size, fill, cChar);
        } else if (shapeType == "square") {
            square.draw(*this, x, y, size, fill, cChar);
        } else if (shapeType == "line") {
            line.draw(*this, x, y, size, fill, cChar);
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


void Triangle::draw(Board& board, int x, int y, int height, string fill, char colour) const {
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

void Circle::draw(Board& board, int x, int y, int radius, string fill, char colour) const {
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

void Square::draw(Board& board, int x, int y, int lenght, string fill, char colour) const {
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

void Line::draw(Board& board, int x, int y, int lenght, string fill, char colour) const {
    if (lenght <= 0) return;
    for (int i = 0; i < lenght; ++i){
        if (x + i < BOARD_WIDTH && y < BOARD_HEIGHT) {
            board.grid[y+i][x + i] = colour;
        }
    }}
class ShapeFactory {
    Board& board;
    int Id = -1;

public:
    ShapeFactory(Board& b) : board(b) {}

    void selectShape(const vector<ShapeDate>& shapesList, int id) {
        for (const auto& shape : shapesList) {
            if (shape.id == id) {
                Id = id;
                cout << "Shape ID: " << shape.id << ", Name: " << shape.name
                     << ", Colour: " << shape.colour << ", X: " << shape.x
                     << ", Y: " << shape.y << ", Size: " << shape.z
                     << ", Filling: " << shape.filling << "\n";
            }
            else{
                cout << "Shape not found.\n";
                Id = -1;
            }
        }
    }

    void editShape(vector<ShapeDate>& shapesList, int newSize) {
        if (Id == -1) {
            cout << "No shape selected for editing.\n";
            return;
        }

        for (auto& shape : shapesList) {
            if (shape.id == Id) {
                shape.z = newSize;
                cout << "Shape updated successfully.\n";
                Id = -1;
                board.clear();
                for (const auto& s : shapesList) {
                    board.draw(s.name, s.x, s.y, s.z, s.filling, s.colour);
                }
                return;
            }
        }
        cout << "Shape not found.\n";
    }

    void removeShape(vector<ShapeDate>& shapesList) {
        if (Id == -1) {
            cout << "No shape selected for removal.\n";
            return;
        }

        if (!shapesList.empty()) {
            for (int i = 0; i < shapesList.size(); ++i) {
                if (shapesList[i].id == Id) {
                    shapesList.erase(shapesList.begin() + i);
                    cout << "Shape removed successfully.\n";
                    Id = -1;
                    board.clear();
                    for (const auto& shape : shapesList) {
                        board.draw(shape.name, shape.x, shape.y, shape.z, shape.filling, shape.colour);
                    }
                    return;
                }
            }
            cout << "Shape not found.\n";
        } else {
            cout << "Shape list is empty.\n";
        }
    }
    void paint(vector<ShapeDate>& shapesList, char newColour) {
        if (Id == -1) {
            cout << "No shape selected for editing.\n";
            return;
        }
        for (auto& shape : shapesList) {
            if (shape.id == Id) {
                shape.colour = newColour;
                cout << "Shape painted successfully.\n";
                Id = -1;
                board.clear();
                for (const auto& s : shapesList) {
                    board.draw(s.name, s.x, s.y, s.z, s.filling, s.colour);
                }
                return;
            }
        }
        cout << "Shape not found.\n";
    }
    void move(vector<ShapeDate>& shapesList, int newX, int newY) {
        if (Id == -1) {
            cout << "No shape selected for editing.\n";
            return;
        }
        for (auto& shape : shapesList) {
            if (shape.id == Id) {
                shape.x = newX;
                shape.y = newY;
                cout << "Shape moved successfully.\n";
                Id = -1;
                board.clear();
                for (const auto& s : shapesList) {
                    board.draw(s.name, s.x, s.y, s.z, s.filling, s.colour);
                }
            }
        }


    }
};



int main() {
    Board board;
    ShapeFactory sf(board);

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
            board.draw(shapeTipe, x, y, size, fill, colour);
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
            if (!shapesList.empty()) {
                shapesList.pop_back();
                board.clear();
                for (const auto& shape : shapesList) {
                    board.draw(shape.name, shape.x, shape.y, shape.z, shape.filling, shape.colour);
                }
            } else{
                cout << "no shapes to delete";
            }
        } else if (command == "select") {
            int id;
            cout << "Enter the ID of the shape to select: ";
            cin >> id;
            sf.selectShape(shapesList, id);
        } else if (command == "remove") {
            sf.removeShape(shapesList);
        } else if (command == "edit") {
            int newSize;
            cout << "Enter the new size for the selected shape: ";
            cin >> newSize;
            sf.editShape(shapesList, newSize);
        }
        else if(command == "paint"){
            char newColour;
            cout << "Enter the new colour for the selected shape: ";
            cin >> newColour;
            sf.paint(shapesList, newColour);

        }
        else if(command == "move"){
            int newX, newY;
            cout << "Enter the new x and y coordinates for the selected shape: ";
            cin >> newX >> newY;
            sf.move(shapesList, newX, newY);
        }
        else if (command == "exit") {
            return 0;
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
//add circle blue 10 10 5
//add triangle red 10 5 5 frame
//add circle red 10 10 5 frame
//add square blue 10 10 10 frame
