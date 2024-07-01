#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <random>
using namespace sf;

class Plansza
{
public:
    const int jednaKratka = 28;
    const int szerokoscKrawedzi = 5;
    int szerokosc;
    int wysokosc;
    Vector2f pozycja;
    Texture texture;
    Sprite plansza;
    Texture borderTexture;
    Sprite border;
    Plansza()
    {
        pozycja.x = 256;
        pozycja.y = 16;
        texture.loadFromFile("board.bmp");
        borderTexture.loadFromFile("border.bmp");
        szerokosc = texture.getSize().x;
        wysokosc = texture.getSize().y;
        plansza.setTexture(texture);
        plansza.setPosition(pozycja.x + 5, pozycja.y + 5);
        border.setTexture(borderTexture);
        border.setPosition(pozycja);
    }
    int getSzerokoscKrawedzi()
    {
        return szerokoscKrawedzi;
    }
    int getJednaKratka()
    {
        return jednaKratka;
    }
    Vector2f getPozycja()
    {
        return pozycja;
    }
    int getSzerokosc()
    {
        return szerokosc;
    }
    int getWysokosc()
    {
        return wysokosc;
    }
    void draw(RenderWindow *aplikacja)
    {
        aplikacja->draw(plansza);
        aplikacja->draw(border);
    }
    FloatRect getGlobalBounds()
    {
        return plansza.getGlobalBounds();
    }
};
Plansza obszarGry;
const std::vector<int> X_COORDS = {261, 289, 317, 345, 373, 401, 429, 457, 485, 513};
const std::vector<int> Y_COORDS = {21, 49, 77, 105, 133, 161, 189, 217, 245, 273, 301, 329, 357, 385, 413, 441, 469, 497, 525, 553};
std::pair<int, int> getGridPosition(int x, int y)
{

    int gridX = -1, gridY = -1;
    for (int i = 0; i < X_COORDS.size(); ++i)
    {
        if (x == X_COORDS[i])
        {
            gridX = i;
            break;
        }
    }
    for (int i = 0; i < Y_COORDS.size(); ++i)
    {
        if (y == Y_COORDS[i])
        {
            gridY = i;
            break;
        }
    }
    return {gridX, gridY};
}
#include "shapes/Shape.h"

// void drawPlaced(RenderWindow *aplikacja, Sprite BlockToDraw)
// {
//     aplikacja->draw(BlockToDraw);
// };

int main()
{

    RenderWindow aplikacja(VideoMode(800, 600, 1), "Tetris");
    Image ikona;
    ikona.loadFromFile("ikona.bmp");
    aplikacja.setIcon(25, 25, ikona.getPixelsPtr());

    Clock grawitacja;
    Time minionyCzas;
    Time predkosc;
    predkosc = seconds(1);

    bool start;
    bool hardDrop = false;
    bool touchedBottom = false;
    bool IsThereAShape = false;

    int linesCompletedForScore = 0; // For score calculation
    int LinesCompletedForLevel = 0; // For level calucation
    int Score = 0;
    int CurrentLevel = 1;

    int gameGrid[10][20] = {0};
    Color colorGrid[10][20];
    Sprite BlocksToDraw[10][20];

    Texture PlacedBlockTexture;
    PlacedBlockTexture.loadFromFile("blok.bmp");

    std::vector<std::unique_ptr<Shapes>> ShapesTemplates;
    ShapesTemplates.push_back(std::make_unique<Square>());
    ShapesTemplates.push_back(std::make_unique<Line>());
    ShapesTemplates.push_back(std::make_unique<TShape>());
    ShapesTemplates.push_back(std::make_unique<ZShape>());
    ShapesTemplates.push_back(std::make_unique<SShape>());
    ShapesTemplates.push_back(std::make_unique<LShape>());
    ShapesTemplates.push_back(std::make_unique<ReverseLShape>());

    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % ShapesTemplates.size();
    std::unique_ptr<Shapes> aktywnyKsztalt = std::move(ShapesTemplates[randomIndex]);
    ShapesTemplates.erase(ShapesTemplates.begin() + randomIndex);
    IsThereAShape = true;

    bool audioplaying = false;
    Music audio;

    if (!audio.openFromFile("kim_jest_tukan.ogg"))
    {
        return -1;
    }

    audio.setVolume(25);
    audio.setLoop(true);
    Font font;
    if (!font.loadFromFile("arial-unicode-ms.ttf"))
    {
        return -1;
    }
    Text ThisTextAboveScore;
    ThisTextAboveScore.setFont(font);
    ThisTextAboveScore.setCharacterSize(24);
    ThisTextAboveScore.setString(L"Twój wynik:");
    ThisTextAboveScore.setFillColor(sf::Color::White);
    ThisTextAboveScore.setPosition(0, 0);

    Text ScoreText;
    ScoreText.setFont(font);
    ScoreText.setCharacterSize(24);
    ScoreText.setFillColor(sf::Color::White);
    ScoreText.setPosition(0, 30);

    Text ThisTextAboveLevel;
    ThisTextAboveLevel.setFont(font);
    ThisTextAboveLevel.setCharacterSize(24);
    ThisTextAboveLevel.setString("Aktualny Poziom:");
    ThisTextAboveLevel.setFillColor(sf::Color::White);
    ThisTextAboveLevel.setPosition(0, 55);

    Text LevelText;
    LevelText.setFont(font);
    LevelText.setCharacterSize(24);
    LevelText.setFillColor(sf::Color::White);
    LevelText.setPosition(0, 79);

    while (aplikacja.isOpen())
    {
        Event zdarzenie;
        while (aplikacja.pollEvent(zdarzenie))
        {
            if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Space)
            {
                start = true;
                grawitacja.restart();
            }
        }
        while (start)
        {
            if (!audioplaying)
            {
                audio.play();
                audioplaying = true;
            }
            ScoreText.setString(std::__cxx11::to_string(Score));
            LevelText.setString(std::__cxx11::to_string(CurrentLevel));
            while (aplikacja.pollEvent(zdarzenie))
            {
                /* Checking if the window is closed or if the escape key is pressed. If it is, it sets
                the start variable to false and closes the window. */

                if (zdarzenie.type == Event::Closed || (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Escape))
                {
                    start = false;
                    aplikacja.close();
                }
                /* Checking if the down key is pressed and if it is, it moves the shape to the bottom
                of the screen. */
                if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Down)
                {

                    while (!hardDrop)
                    {
                        aktywnyKsztalt->Shapes::grawitacja();
                        // Check for collision with the ground and placed blocks
                        for (int i = 0; i < 4; i++)
                        {
                            int x = aktywnyKsztalt->Shapes::blok[i].getPosition().x;
                            int y = aktywnyKsztalt->Shapes::blok[i].getPosition().y;
                            std::pair<int, int> gridPos = getGridPosition(x, y);

                            // Check if the grid position is occupied by a placed block
                            if (gridPos.first >= 0 && gridPos.first < 10 && gridPos.second >= 0 && gridPos.second < 20)
                            {
                                if (gameGrid[gridPos.first][gridPos.second] == 1)
                                {
                                    hardDrop = true;
                                    break;
                                }

                                // Check if the shape is on top of a placed block
                                if (gridPos.second == 19 || gameGrid[gridPos.first][gridPos.second + 1] == 1)
                                {
                                    hardDrop = true;
                                    break;
                                }
                            }

                            // Check if the shape has reached the bottom of the screen
                            if (aktywnyKsztalt->Shapes::blok[i].getPosition().y == 553)
                            {
                                hardDrop = true;
                                break;
                            }
                        }
                    }
                }

                if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Right)
                {
                    Vector2f prawo(obszarGry.getJednaKratka(), 0);
                    aktywnyKsztalt->Shapes::move(prawo, gameGrid);
                }
                if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Left)
                {
                    Vector2f lewo(-obszarGry.getJednaKratka(), 0);
                    aktywnyKsztalt->Shapes::move(lewo, gameGrid);
                }

                if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::LAlt)
                {
                    aktywnyKsztalt->reverseRotate(gameGrid);
                }
                if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::RAlt)
                {
                    aktywnyKsztalt->rotate(gameGrid);
                }
                if (zdarzenie.type == Event::KeyPressed && zdarzenie.key.code == Keyboard::Up)
                {
                    predkosc = milliseconds(50 - ((CurrentLevel * 40)));
                }
                else
                {
                    predkosc = milliseconds(1000 - ((CurrentLevel * 40)));
                }
            }

            minionyCzas = grawitacja.getElapsedTime();

            if (!hardDrop || !touchedBottom)
            {
                if (minionyCzas >= predkosc)
                {
                    aktywnyKsztalt->Shapes::grawitacja();
                    grawitacja.restart();
                }
            }

            aplikacja.clear();
            aplikacja.draw(ThisTextAboveLevel);
            aplikacja.draw(ThisTextAboveScore);
            aplikacja.draw(ScoreText);
            aplikacja.draw(LevelText);
            obszarGry.draw(&aplikacja);

            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    if (gameGrid[i][j] == 1)
                    {
                        BlocksToDraw[i][j].setTexture(PlacedBlockTexture);
                        BlocksToDraw[i][j].setPosition(X_COORDS[i], Y_COORDS[j]);
                        BlocksToDraw[i][j].setColor(colorGrid[i][j]);
                        // drawPlaced(&aplikacja, BlockToDraw);
                        aplikacja.draw(BlocksToDraw[i][j]);
                    }
                }
            }

            aktywnyKsztalt->Shapes::draw(&aplikacja);
            aplikacja.display();

            if (!hardDrop)
            {
                for (int i = 0; i < 4; i++)
                {
                    int x = aktywnyKsztalt->Shapes::blok[i].getPosition().x;
                    int y = aktywnyKsztalt->Shapes::blok[i].getPosition().y;
                    std::pair<int, int> gridPos = getGridPosition(x, y);

                    // Check if the grid position is occupied by a placed block
                    if (gridPos.first >= 0 && gridPos.first < 10 && gridPos.second >= 0 && gridPos.second < 20)
                    {
                        if (gameGrid[gridPos.first][gridPos.second] == 1)
                        {
                            touchedBottom = true;
                            break;
                        }

                        // Check if the shape is on top of a placed block
                        if (gridPos.second == 19 || gameGrid[gridPos.first][gridPos.second + 1] == 1)
                        {
                            touchedBottom = true;
                            break;
                        }
                        else
                        {
                            touchedBottom = false;
                        }
                    }

                    // Check if the shape has reached the bottom of the screen
                    if (aktywnyKsztalt->Shapes::blok[i].getPosition().y == 553)
                    {
                        touchedBottom = true;
                        grawitacja.restart();
                        break;
                    }
                }
            }
            minionyCzas = grawitacja.getElapsedTime();
            if (hardDrop || (touchedBottom && minionyCzas >= predkosc))
            {
                for (int i = 0; i < 4; i++)
                {
                    int x = aktywnyKsztalt->Shapes::blok[i].getPosition().x;
                    int y = aktywnyKsztalt->Shapes::blok[i].getPosition().y;
                    Color color = aktywnyKsztalt->Shapes::blok[i].getColor();
                    std::pair<int, int> gridPos = getGridPosition(x, y);
                    gameGrid[gridPos.first][gridPos.second] = 1;
                    colorGrid[gridPos.first][gridPos.second] = color;
                }
                aktywnyKsztalt.release();
                IsThereAShape = false;
                hardDrop = false;
                touchedBottom = false;
                grawitacja.restart();
            }

            for (int j = 0; j < 20; j++)
            {
                bool lineFilled = true;
                for (int i = 0; i < 10; i++)
                {
                    if (gameGrid[i][j] == 0)
                    {
                        lineFilled = false;
                        break;
                    }
                }
                if (lineFilled)
                {
                    linesCompletedForScore++;
                    LinesCompletedForLevel++;
                    // Delete all sprites from the row
                    for (int i = 0; i < 10; i++)
                    {
                        BlocksToDraw[i][j].setTextureRect(sf::IntRect(0, 0, 0, 0));
                        gameGrid[i][j] = 0;
                        colorGrid[i][j] = Color::Transparent;
                    }
                    // Move all blocks above the row down by one
                    for (int k = j - 1; k >= 0; k--)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            BlocksToDraw[i][k + 1] = BlocksToDraw[i][k];
                            gameGrid[i][k + 1] = gameGrid[i][k];
                            colorGrid[i][k + 1] = colorGrid[i][k];
                        }
                    }
                }
            }

            switch (linesCompletedForScore)
            {
            case 0:
                break;
            case 1:
                Score += (100 * CurrentLevel);
                linesCompletedForScore = 0;
                break;
            case 2:
                Score += (300 * CurrentLevel);
                linesCompletedForScore = 0;
                break;
            case 3:
                Score += (500 * CurrentLevel);
                linesCompletedForScore = 0;
                break;
            case 4:
                Score += (800 * CurrentLevel);
                linesCompletedForScore = 0;
                break;
            }
            if ((CurrentLevel * 5) <= LinesCompletedForLevel)
            {
                CurrentLevel++;
                LinesCompletedForLevel = 0;
            }

            if (IsThereAShape)
            {
                continue;
            }

            if (ShapesTemplates.size() < 1)
            {
                ShapesTemplates.clear();
                ShapesTemplates.push_back(std::make_unique<Square>());
                ShapesTemplates.push_back(std::make_unique<Line>());
                ShapesTemplates.push_back(std::make_unique<TShape>());
                ShapesTemplates.push_back(std::make_unique<ZShape>());
                ShapesTemplates.push_back(std::make_unique<SShape>());
                ShapesTemplates.push_back(std::make_unique<LShape>());
                ShapesTemplates.push_back(std::make_unique<ReverseLShape>());
            }
            int randomIndex = std::rand() % (ShapesTemplates.size());
            aktywnyKsztalt = std::move(ShapesTemplates[randomIndex]);
            ShapesTemplates.erase(ShapesTemplates.begin() + randomIndex);
            IsThereAShape = true;
        }
    }
}