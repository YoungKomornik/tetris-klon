#include <SFML/Graphics.hpp>

using namespace sf;

class Square
{
public:
    Sprite blok[4];
    Texture bloktexture;
    bool ReachedBottom;
    Square()
    {
        bloktexture.loadFromFile("blok.bmp");

        blok[0].setTexture(bloktexture);
        blok[0].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());

        blok[1].setTexture(bloktexture);
        blok[1].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi());

        blok[2].setTexture(bloktexture);
        blok[2].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());

        blok[3].setTexture(bloktexture);
        blok[3].setPosition(obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka(), obszarGry.getPozycja().y + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka());
    }
    void setPosition(Vector2f nowapozycja)
    {
        for (int i = 0; i < 4; i++)
        {
            blok[i].setPosition(nowapozycja);
        }
    }
    Vector2f getPosition()
    {
        return blok[0].getPosition();
    }
    void move(Vector2f nowapozycja)
    {
        if (blok[3].getPosition().x + nowapozycja.x > obszarGry.getPozycja().x + obszarGry.getSzerokosc() - obszarGry.getSzerokoscKrawedzi())
        {
            return;
        }
        if (blok[1].getPosition().x + nowapozycja.x < obszarGry.getPozycja().x + obszarGry.getSzerokoscKrawedzi() + obszarGry.getJednaKratka())
        {
            return;
        }
        if (blok[4].getPosition().y + obszarGry.getJednaKratka() - 3 + nowapozycja.y > obszarGry.getPozycja().y + (obszarGry.getSzerokoscKrawedzi() * 2) + obszarGry.getWysokosc())
        {
            return;
        }
        
        for (int i = 0; i < 4; i++)
        {
            blok[i].move(nowapozycja);
        }
    }
    void grawitacja()
    {
        if (blok[2].getPosition().y && blok[3].getPosition().y > obszarGry.getWysokosc() - obszarGry.getJednaKratka())
        {
            return;
        }
        Vector2f grawitacja;
        grawitacja.x = 0;
        grawitacja.y = obszarGry.getJednaKratka();
        move(grawitacja);
    }
    void draw(RenderWindow *aplikacja)
    {
        for (int i = 0; i < 4; i++)
        {
            aplikacja->draw(blok[i]);
        }
    }
};