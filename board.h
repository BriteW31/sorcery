#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include "player.h"
#include "ability.h"
#include <vector>
#include <string>

#define BLOCK_HEIGHT 11
struct Listener {
        std::string key;
        Ability * cb;
};

class Game;

class Board {
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    Game *game;
    std::vector<Listener> listeners;
    void print(const std::vector<card_template_t> &cards, int height) const;

public:
    Board(Game *g,std::unique_ptr<Player> p1, std::unique_ptr<Player> p2);

    Player &getPlayer(int id);
    Player &getOpponent(int id);
    void display() const;
    void displayHand(Player& player) const;
    void registerListener(const std::string& key, Ability* ability);
    void notify(const std::string& key, int playerId = -1, int boardIdx = -1);
    void start_turn();
    void end_turn();
    void minion_enter(int p,int i);
    void minion_leave(int p,int i);
};

#endif
