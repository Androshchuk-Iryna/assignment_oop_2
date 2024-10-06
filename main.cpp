#include <iostream>
#include <vector>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;
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

    }
};

class Draw{

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

};
class Square: public Shape{

};
class Line: public Shape{

};




// Struct to define the board


int main() {
    Board board;
    Triangle triangle;
    triangle.draw(board,10, 1, 10);
    board.print();
    return 0;
}