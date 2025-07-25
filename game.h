#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <string>
#include "player.h"
#include "board.h"
#include "ascii_graphics.h"

class Game {
    std::unique_ptr<Board> board;
    int currentPlayer = 1;
    bool testingMode = false;
    bool graphicsEnabled = false;
    std::string deckFile1;
    std::string deckFile2;
    std::string initFile;
    void helpMsg() const;

public:
    Game(bool testing, bool graphics, std::string deckFile1, std::string deckFile2);
    void init(const std::string &initFile = "");
    void start();
    void processCommand(const std::string &line);
    Player &getCurrentPlayer();
    Player &getOtherPlayer();
    Player &getPlayer(int index);
    void togglePlayer();
    bool isTestingMode() const;
};

#endif
