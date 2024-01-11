#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#define BLANC sf::Color::White
#define NOIR sf::Color::Black
#define GRIS sf::Color(153, 153, 153)

int envelopperValeur(int v, int vMax)
{
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

int main()
{
    const int TAILLE_CELLULE = 30;
    const sf::Vector2f VECTEUR_CELLULE(TAILLE_CELLULE, TAILLE_CELLULE);
    const int LARGEUR_GRILLE = 30;
    const int HAUTEUR_GRILLE = 20;
    const int N_CELLULES = LARGEUR_GRILLE * HAUTEUR_GRILLE;
    int grille[N_CELLULES] = {};
    int grilleSuivante[N_CELLULES];
    srand(time(NULL));
    for (int i = 0; i < N_CELLULES; i++)
        grille[i] = (double(rand()) / RAND_MAX < 0.1) ? 1 : 0;

    const int INCREMENT_DELAI = 50;
    int delai = 100;

    bool isPlaying = true;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text textePause("Appuyez sur 'p' pour mettre en pause.", font);
    textePause.setCharacterSize(15);
    textePause.setPosition(10, TAILLE_CELLULE * HAUTEUR_GRILLE + 5);
    textePause.setFillColor(NOIR);

    sf::Text textePlay("Appuyez sur 'p' pour jouer.", font);
    textePlay.setCharacterSize(15);
    textePlay.setPosition(10, TAILLE_CELLULE * HAUTEUR_GRILLE + 5);
    textePlay.setFillColor(NOIR);

    sf::Text texteVitesse("Utilisez les flèches gauche/droite pour changer la vitesse.", font);
    texteVitesse.setCharacterSize(15);
    texteVitesse.setPosition(300, TAILLE_CELLULE * HAUTEUR_GRILLE + 5);
    texteVitesse.setFillColor(NOIR);

    sf::RenderWindow window(sf::VideoMode(TAILLE_CELLULE * LARGEUR_GRILLE, TAILLE_CELLULE * HAUTEUR_GRILLE + 50), "Game of Life");
    while (window.isOpen())
    {
        sf::Event evenement;
        while (window.pollEvent(evenement))
        {
            switch (evenement.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (evenement.key.code == sf::Keyboard::P)
                    isPlaying = !isPlaying;
                else if (evenement.key.code == sf::Keyboard::Right)
                    delai = std::max(delai - INCREMENT_DELAI, 0);
                else if (evenement.key.code == sf::Keyboard::Left)
                    delai += INCREMENT_DELAI;
                break;
            }
        }

        window.clear(BLANC);
        for (int x = 0; x < LARGEUR_GRILLE; x++)
        {
            for (int y = 0; y < HAUTEUR_GRILLE; y++)
            {
                sf::RectangleShape formeCellule;
                formeCellule.setPosition(x * TAILLE_CELLULE, y * TAILLE_CELLULE);
                formeCellule.setSize(VECTEUR_CELLULE);
                formeCellule.setOutlineThickness(1);
                formeCellule.setOutlineColor(GRIS);
                if (grille[x + y * LARGEUR_GRILLE] == 1)
                    formeCellule.setFillColor(NOIR);
                else
                    formeCellule.setFillColor(BLANC);
                window.draw(formeCellule);

                if (isPlaying)
                {
                    int Sum = 0;
                    for (int i = -1; i < 2; i++)
                        for (int j = -1; j < 2; j++)
                        {
                            int xi = envelopperValeur(x + i, LARGEUR_GRILLE);
                            int yj = envelopperValeur(y + j, HAUTEUR_GRILLE);
                            Sum += grille[xi + yj * LARGEUR_GRILLE];
                        }

                    int courant = x + y * LARGEUR_GRILLE;
                    Sum -= grille[courant];
                    grilleSuivante[courant] = grille[courant];
                    if (grille[courant] == 1 && (Sum < 2 || Sum > 3))
                        grilleSuivante[courant] = 0;
                    else if (Sum == 3)
                        grilleSuivante[courant] = 1;
                }
            }
        }

        if (isPlaying)
            for (int i = 0; i < N_CELLULES; i++)
                grille[i] = grilleSuivante[i];

        window.draw(texteVitesse);
        if (isPlaying)
            window.draw(textePause);
        else
            window.draw(textePlay);

        window.display();
        sf::sleep(sf::milliseconds(delai));
    }
}
