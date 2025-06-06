#include "snake.h"
#include "./ui_snake.h"

#include <chrono>
#include <stdexcept>
#include <QDebug>

Snake::Snake(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Snake)
    , rng(new std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()))
{
    ui->setupUi(this);
    QFont font("Monospace");
    // font.setStyleHint(QFont::TypeWriter);
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(15);
    ui->GameBoard->setFont(font);
    resetBoard();
    snake.emplace_back(24, 6);
    snake.emplace_back(23, 6);
    snake.emplace_back(22, 6);
    placeSnake();
    placeFood(generateFood());
    setBoard();
    installEventFilter(this);
}

Snake::Coord::Coord()
    : x(0)
    , y(0)
{}

Snake::Coord::Coord(size_t _x, size_t _y)
    : x(_x)
    , y(_y)
{}

Snake::~Snake()
{
    delete ui;
}

bool Snake::Coord::operator==(const Coord& other)
{
    return x == other.x && y == other.y;
}

bool Snake::Coord::operator!=(const Coord& other)
{
    return x != other.x || y != other.y;
}

bool Snake::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        switch(keyEvent->key()){
        case Qt::Key_Up:
        case Qt::Key_W:
            if(alive) updateSnake(Direction::Up);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            if(alive) updateSnake(Direction::Down);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if(alive) updateSnake(Direction::Right);
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            if(alive) updateSnake(Direction::Left);
            break;
        default:
            break;
        }
    }
    return QObject::eventFilter(target, event);
}

QChar& Snake::findChar(const Coord& c)
{
    if(c.x > 49 || c.y > 12) throw std::out_of_range("The Given coordinate was outside of te board");

    return board[c.x + (c.y * 51)];
}

const QChar Snake::findChar(const Coord& c) const
{
    if(c.x > 49 || c.y > 12) throw std::out_of_range("The Given coordinate was outside of te board");

    return board[c.x + (c.y * 51)];
}

void Snake::placeSnake()
{
    for(const Coord& c : snake)
        findChar(c) = BODY;
    findChar(snake.front()) = HEAD;
}

const Snake::Coord Snake::generateFood() const
{
    std::deque<Coord> available;
    for(size_t y = 0; y < 13; ++y)
        for(size_t x = 0; x < 50; ++x)
            if(findChar(Coord(x, y)) == ' ')
                available.emplace_back(x, y);

    if(available.empty()) throw std::logic_error("No availble place to generate food");

    std::uniform_int_distribution<size_t> dist(0, available.size() - 1);

    return available[dist(*rng)];
}

void Snake::placeFood(const Coord& c)
{
    if(findChar(c) != ' ') throw std::logic_error("Cannot place food in a taken space");

    findChar(c) = FOOD;
    food = c;
}

void Snake::updateSnake(const Direction dir)
{
    Coord next(snake.front());
    switch (dir) {
    case Direction::Up:
        if (next.y == 0)
        {
            alive = false;
            break;
        }
        --next.y;
        break;
    case Direction::Down:
        if (next.y >= 12) alive = false;
        ++next.y;
        break;
    case Direction::Left:
        if (next.x == 0) {
            alive = false;
            break;
        }
        --next.x;
        break;
    case Direction::Right:
        ++next.x;
        if (next.x > 49) alive = false;
        break;
    }

    if (!alive) return;

    bool ate = findChar(next) == FOOD;

    if (ate) {
        snake.push_front(next);
        resetBoard();
        placeSnake();
        placeFood(generateFood());
        setBoard();
        return;
    }

    for(std::size_t i = 0; i < snake.size() - 1; ++i)
        if (snake[i] == next) {
            alive = false;
            return;
        }

    snake.pop_back();
    snake.push_front(next);
    resetBoard();
    placeSnake();
    placeFood(food);
    setBoard();
    return;
}

void Snake::resetBoard()
{
    board.clear();
    for(size_t i = 0; i < 12; ++i)
        board += "                                                  \n";
    board += "                                                  ";
}

void Snake::setBoard()
{
    ui->GameBoard->setText(board);
}
