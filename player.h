#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include "card.h"
#include "ascii_graphics.h"

class Ritual;
class Game;

class Player {
    std::string name;
    int id;
    int life = 20;
    int magic = 3;
    Game *game;
    std::vector<std::unique_ptr<Card>> hand;
    std::vector<std::unique_ptr<Card>> deck;
    std::vector<std::unique_ptr<Card>> graveyard;
    std::vector<std::unique_ptr<Card>> board; // size <= 5
    std::unique_ptr<Card> ritual;

public:
    Player(const std::string &name, int id, std::vector<std::unique_ptr<Card>> &&deck, Game *game);

    void drawCard();
    void drawInitialHand(bool isTestingMode);
    void playCard(int index, int targetPlayer, int targetCard, bool isTestingMode);
    void attack(int whoAttack, int whoAttacked, Player &opponent);
    void startTurn();
    void endTurn();
    void shuffleAndDraw(bool testingMode, unsigned seed);
    void discardCard(int index);

    std::string getName() const;
    int getLife() const;
    int getMagic() const;
    void changeLife(int delta);
    void changeMagic(int delta);
    std::vector<std::unique_ptr<Card>> &getHand();
    std::vector<std::unique_ptr<Card>> &getBoard();
    std::vector<std::unique_ptr<Card>> &getGraveyard();
    Game *getGame();
    Ritual *getRitual();

    bool destroyMinion(int index);
    bool removeRitual();
};

#endif
