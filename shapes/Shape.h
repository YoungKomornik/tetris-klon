#include <SFML/Graphics.hpp>
#include <time.h>
#include <math.h>
using namespace sf;
class Shapes
{

public:
    Sprite blok[4];
    Texture bloktexture;
    void setPosition(Vector2f newposition)
    {
        for (int i = 0; i <= 3; i++)
        {
            Vector2f nowapozycja = blok[i].getPosition();
            nowapozycja.x += newposition.x;
            nowapozycja.y += newposition.y;
            blok[i].setPosition(nowapozycja);
        }
    }
    void move(Vector2f nowapozycja, int gameGrid[10][20])
    {
        for (int i = 0; i < 4; i++)
        {
            int x = blok[i].getPosition().x;
            int y = blok[i].getPosition().y;
            std::pair<int, int> gridPos = getGridPosition(x, y);
            if (gameGrid[gridPos.first + 1][gridPos.second] == 1 && nowapozycja.x>0)
            {
                return;
            }
            if (gameGrid[gridPos.first - 1][gridPos.second] == 1 && nowapozycja.x<0)
            {
                return;
            }
        }
        for (int i = 0; i <= 3; i++)
        {
            blok[i].move(nowapozycja);
        }
        for (int i = 0; i < 4; i++)
        {
            if (!obszarGry.getGlobalBounds().contains(blok[i].getPosition()))
            {
                for (int j = 0; j < 4; j++)
                {
                    blok[j].move(-nowapozycja);
                }
            }
        }
    }
    void grawitacja()
    {
        for (int i = 0; i <= 3; i++)
        {
            if (blok[i].getPosition().y > 528)
            {
                return;
            }
        }
        Vector2f grawitacja;
        grawitacja.x = 0;
        grawitacja.y = obszarGry.getJednaKratka();
        setPosition(grawitacja);
    }
    void draw(RenderWindow *aplikacja)
    {
        for (int i = 0; i <= 3; i++)
        {
            aplikacja->draw(blok[i]);
        }
    }
    virtual void rotate(int gameGrid[10][20]) {}
    virtual void reverseRotate(int gameGrid[10][20]){}
    void fixOutOfBounds()
    {
        for (int i = 0; i < 4; i++)
        {
            while (!obszarGry.getGlobalBounds().contains(blok[i].getPosition()))
            {
                bool upcheck = false;
                if (blok[i].getPosition().y < obszarGry.getPozycja().y)
                {
                    upcheck = true;
                }
                for (int j = 0; j <= 3; j++)
                {
                    if (blok[j].getPosition().x < 400)
                    {
                        blok[j].move(obszarGry.getJednaKratka(), 0);
                    }
                    else
                    {
                        blok[j].move(-obszarGry.getJednaKratka(), 0);
                    }
                    if (upcheck)
                    {
                        blok[j].move(0, obszarGry.getJednaKratka());
                    }
                }
            }
        }
    }
    bool checkBlocksCollision(int gameGrid[10][20]){
        for(int i=0;i<4;i++){
            int x = blok[i].getPosition().x;
            int y = blok[i].getPosition().y;
            std::pair<int, int> gridPos = getGridPosition(x, y);
            if(gameGrid[gridPos.first][gridPos.second] == 1){
                return true;
            }
        }
        return false;
    }
};
class Square : public Shapes
{
public:
    bool ReachedBottom;
    Square()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color::Red);
        }
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[1].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[2].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());
        blok[3].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());
    }
    void rotate(int gamegrid[10][20]) override
    {
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
    }
};
class Line : public Shapes
{
public:
    bool ReachedBottom;
    enum class Orientation
    {
        Up,
        Right,
        Down,
        Left
    };
    Orientation orientation;
    Line()
    {
        bloktexture.loadFromFile("blok.bmp");
        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color::Green);
        }
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[1].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[2].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + (2 * obszarGry.getJednaKratka()), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[3].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + (3 * obszarGry.getJednaKratka()), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        orientation = Orientation::Up;
    }
    void rotate(int gamegrid[10][20]) override
    {
        if (orientation == Orientation::Up)
        {

            blok[0].setPosition(blok[0].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-obszarGry.getJednaKratka(), 2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
        }
        else if (orientation == Orientation::Right)
        {
            blok[0].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 2 * obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-2 * obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
        }
        else if (orientation == Orientation::Down)
        {
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-2 * obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
        }
        else if (orientation == Orientation::Left)
        {
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            reverseRotate(gamegrid);
        }
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
        if (orientation == Orientation::Up)
        {
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
        }
        else if (orientation == Orientation::Right)
        {
            blok[0].setPosition(blok[0].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-obszarGry.getJednaKratka(), 2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
        }
        else if (orientation == Orientation::Down)
        {
            blok[0].setPosition(blok[0].getPosition() - Vector2f(obszarGry.getJednaKratka(), 2 * obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-2 * obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
        }
        else if (orientation == Orientation::Left)
        {
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-2 * obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            rotate(gamegrid);
        }
    }
};
class TShape : public Shapes
{
public:
    enum class Orientation
    {
        Up,
        Right,
        Down,
        Left
    };
    Orientation orientation;
    TShape()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color::Blue);
        }
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());
        blok[1].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[2].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
        blok[3].setPosition(blok[0].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
        orientation = Orientation::Up;
    }
    void rotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
            break;
        case Orientation::Right:
            blok[2].setPosition(blok[2].getPosition() + Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        case Orientation::Down:
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;
        case Orientation::Left:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            reverseRotate(gamegrid);
        }
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;
        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
            break;
        case Orientation::Down:
            blok[2].setPosition(blok[2].getPosition() - Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
            break;
        case Orientation::Left:
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            rotate(gamegrid);
        }
    }
};
class ZShape : public Shapes
{
public:
    enum class Orientation
    {
        Up,
        Right,
        Down,
        Left
    };
    Orientation orientation;
    ZShape()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color::Magenta);
        }

        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[1].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[2].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
        blok[3].setPosition(blok[0].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
        orientation = Orientation::Up;
    }
    void rotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[1].setPosition(blok[1].getPosition() + Vector2f(0, 2 * obszarGry.getJednaKratka()));
            blok[0].setPosition(blok[0].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Right;
            break;
        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(0, 2 * obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Down;
            break;
        case Orientation::Down:
            blok[1].setPosition(blok[1].getPosition() + Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;
        case Orientation::Left:
            blok[1].setPosition(blok[1].getPosition() + Vector2f(obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Up;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            reverseRotate(gamegrid);
        }
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[1].setPosition(blok[1].getPosition() - Vector2f(obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Left;
            break;
        case Orientation::Right:
            blok[1].setPosition(blok[1].getPosition() - Vector2f(0, 2 * obszarGry.getJednaKratka()));
            blok[0].setPosition(blok[0].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Up;
            break;
        case Orientation::Down:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(0, 2 * obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Right;
            break;
        case Orientation::Left:
            blok[1].setPosition(blok[1].getPosition() - Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            rotate(gamegrid);
        }
    }
};
class SShape : public Shapes
{
public:
    enum class Orientation
    {
        Up,
        Right,
        Down,
        Left
    };
    Orientation orientation;
    SShape()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color::Cyan);
        }
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());
        blok[1].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[2].setPosition(blok[1].getPosition() + Vector2f(0, -obszarGry.getJednaKratka()));
        blok[3].setPosition(blok[2].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        orientation = Orientation::Up;
    }
    void rotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(0, 2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
            break;
        case Orientation::Right:
            blok[2].setPosition(blok[2].getPosition() + Vector2f(0, 2 * obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Down;
            break;
        case Orientation::Down:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-2 * obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(0, -2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;
        case Orientation::Left:
            blok[2].setPosition(blok[2].getPosition() + Vector2f(0, -2 * obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Up;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            reverseRotate(gamegrid);
        }
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[2].setPosition(blok[2].getPosition() - Vector2f(0, -2 * obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Left;
            break;
        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(0, 2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
            break;
        case Orientation::Down:
            blok[2].setPosition(blok[2].getPosition() - Vector2f(0, 2 * obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Right;
            break;
        case Orientation::Left:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-2 * obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(0, -2 * obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            rotate(gamegrid);
        }
    }
};
class LShape : public Shapes
{
public:
    enum class Orientation
    {
        Up,
        Right,
        Down,
        Left
    };
    Orientation orientation;
    LShape()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color::Yellow);
        }
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());
        blok[1].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[2].setPosition(blok[1].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[3].setPosition(blok[2].getPosition() + Vector2f(0, -obszarGry.getJednaKratka()));
        orientation = Orientation::Up;
    }
    void rotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(0, obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Right;
            break;
        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(0, -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        case Orientation::Down:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(0, -obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;
        case Orientation::Left:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Up;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            reverseRotate(gamegrid);
        }
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(2 * obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Left;
            break;
        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(0, obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-obszarGry.getJednaKratka(), 0));
            orientation = Orientation::Up;
            break;
        case Orientation::Down:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(0, -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
            break;
        case Orientation::Left:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(obszarGry.getJednaKratka(), 0));
            blok[2].setPosition(blok[2].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(0, -obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            rotate(gamegrid);
        }
    }
};
class ReverseLShape : public Shapes
{
public:
    enum class Orientation
    {
        Up,
        Right,
        Down,
        Left
    };
    Orientation orientation;
    ReverseLShape()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i <= 3; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setColor(Color(252, 152, 3));
        }
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());
        blok[1].setPosition(blok[0].getPosition() + Vector2f(0, obszarGry.getJednaKratka()));
        blok[2].setPosition(blok[1].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[3].setPosition(blok[2].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        orientation = Orientation::Up;
    }
    void rotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
            blok[1].setPosition(blok[1].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(0, -obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
            break;

        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(0, obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        case Orientation::Down:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(0, obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() + Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;
        case Orientation::Left:
            blok[0].setPosition(blok[0].getPosition() + Vector2f(0, -2 * obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() + Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() + Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            reverseRotate(gamegrid);
        }
    }
    void reverseRotate(int gamegrid[10][20]) override
    {
        switch (orientation)
        {
        case Orientation::Up:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(0, -2 * obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            orientation = Orientation::Left;
            break;

        case Orientation::Right:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(obszarGry.getJednaKratka(), 0));
            blok[1].setPosition(blok[1].getPosition() - Vector2f(obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(0, -obszarGry.getJednaKratka()));
            orientation = Orientation::Up;
            break;
        case Orientation::Down:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(-obszarGry.getJednaKratka(), obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() - Vector2f(obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(0, obszarGry.getJednaKratka()));
            orientation = Orientation::Right;
            break;
        case Orientation::Left:
            blok[0].setPosition(blok[0].getPosition() - Vector2f(0, obszarGry.getJednaKratka()));
            blok[1].setPosition(blok[1].getPosition() - Vector2f(-obszarGry.getJednaKratka(), 0));
            blok[3].setPosition(blok[3].getPosition() - Vector2f(-obszarGry.getJednaKratka(), -obszarGry.getJednaKratka()));
            orientation = Orientation::Down;
            break;
        }
        fixOutOfBounds();
        if(checkBlocksCollision(gamegrid)){
            rotate(gamegrid);
        }
    }
};