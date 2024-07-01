#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;

class Line
{
public:
    Sprite blok[4];
    Texture bloktexture;
    enum class Orientation
    {
        Horizontal,
        Vertical
    };
    Orientation orientation;
    Line()
    {
        bloktexture.loadFromFile("blok.bmp");

        for (int i = 0; i < 4; i++)
        {
            blok[i].setTexture(bloktexture);
            blok[i].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + (i * obszarGry.getJednaKratka()));
        }
    }
    void rotate()
{
    if (orientation == Orientation::Horizontal)
    {
       
        blok[1].setPosition(blok[0].getPosition() + Vector2f(0, obszarGry.getJednaKratka()));
        blok[2].setPosition(blok[0].getPosition() + Vector2f(0, 2 * obszarGry.getJednaKratka()));
        blok[3].setPosition(blok[0].getPosition() + Vector2f(0, 3 * obszarGry.getJednaKratka()));
        orientation = Orientation::Vertical;
    }
    else
    {
        //łopbracamy na pionowe
        blok[1].setPosition(blok[0].getPosition() + Vector2f(obszarGry.getJednaKratka(), 0));
        blok[2].setPosition(blok[0].getPosition() + Vector2f(2 * obszarGry.getJednaKratka(), 0));
        blok[3].setPosition(blok[0].getPosition() + Vector2f(3 * obszarGry.getJednaKratka(), 0));
        orientation = Orientation::Horizontal;
    }
}
    void setPosition(Vector2f newposition)
    {
        for (int i = 0; i < 4; i++)
        {
            Vector2f nowapozycja = blok[i].getPosition();
            nowapozycja.x += newposition.x;
            nowapozycja.y += newposition.y;
            blok[i].setPosition(nowapozycja);
        }
    }
    void move(Vector2f nowapozycja)
    {
        for (int i = 0; i < 4; i++)
        {
            blok[i].move(nowapozycja);
        }
    }
    void grawitacja()
    {
        if (blok[2].getPosition().y && blok[3].getPosition().y > 528)
        {
            return;
        }
        Vector2f grawitacja;
        grawitacja.x = 0;
        grawitacja.y = obszarGry.getJednaKratka();
        setPosition(grawitacja);
    }
    void draw(RenderWindow *aplikacja)
    {
        for (int i = 0; i < 4; i++)
        {
            aplikacja->draw(blok[i]);
        }
    }
};