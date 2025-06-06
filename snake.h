#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include <random>
#include <memory>
#include <deque>

QT_BEGIN_NAMESPACE
namespace Ui {
class Snake;
}
QT_END_NAMESPACE

class Snake : public QMainWindow
{
    Q_OBJECT

public:
    Snake(QWidget *parent = nullptr);
    bool eventFilter(QObject *target, QEvent *event) ;
    ~Snake();

private:
    // Stores the x and y coordinates to know where something is on the board
    struct Coord
    {
        size_t x;
        size_t y;

        Coord();

        Coord(size_t _x, size_t _y);

        bool operator==(const Coord& other);

        bool operator!=(const Coord& other);
    };

    // Used for easily knowing which way the snake is planning on moving
    enum class Direction : uint_fast8_t
    {
        Up,
        Down,
        Left,
        Right
    };


    Ui::Snake *ui;
    QString board;
    std::unique_ptr<std::mt19937> rng;
    Coord food;
    std::deque<Coord> snake;
    bool alive = true;

    // Useful Constants
    static constexpr QChar HEAD = 'H';
    static constexpr QChar BODY = 'X';
    static constexpr QChar FOOD = 'O';

    // Returns a reference to the specified QChar
    QChar& findChar(const Coord& c);

    // Returns a const QChar of the specified value
    const QChar findChar(const Coord& c) const;

    // Places the snake on the board at is current positions
    void placeSnake();

    // Generate the location of the next piece of food
    const Coord generateFood() const;

    // Places food onto the board at the given location
    // (If there is food still on the board, replaces it)
    void placeFood(const Coord& c);

    // Moves the snake in the given direction
    void updateSnake(const Direction dir);

    // Resets the board to empty
    void resetBoard();

    // Sets the game board to the current board configuration
    void setBoard();
};
#endif // SNAKE_H
