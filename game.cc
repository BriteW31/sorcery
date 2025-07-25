#include "game.h"
#include "cardfactory.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Game::helpMsg() const {
    std::cout << "Commands: help -- Display this message." << std::endl;
    std::cout << "          end  -- End the current player's turn." << std::endl;
    std::cout << "          quit -- End the game." << std::endl;
    std::cout << "          attack minion other-minion -- Orders minion to attack other-minion." << std::endl;
    std::cout << "          attack minion -- Orders minion to attack the opponent." << std::endl;
    std::cout << "          play card [target-player target-card] -- Play card, optionally targeting target-card owned by target-player." << std::endl;
    std::cout << "          use minion [target-player target-card] -- Use minion's special ability, optionally targeting target-card owned by target-player." << std::endl;
    std::cout << "          inspect minion -- View a minion's card and all enchantments on that minion." << std::endl;
    std::cout << "          hand -- Describe all cards in your hand." << std::endl;
    std::cout << "          board -- Describe all cards on the board." << std::endl;
    if (testingMode) {
        std::cout << "          draw -- draw a card." << std::endl;
        std::cout << "          discard -- discards the ith card in the player's hand." << std::endl;
    }
}

Game::Game(bool testing, bool graphics, std::string deckFile1, std::string deckFile2)
    : testingMode{testing}, graphicsEnabled{graphics}, deckFile1{deckFile1}, deckFile2{deckFile2} { }

void Game::init(const std::string &initFile) {
    this->initFile = initFile;
    std::istream *in = &std::cin;
    std::ifstream file;
    if (!initFile.empty()) {
        file.open(initFile);
        if (file) in = &file;
    }

    std::string name1, name2;
    std::cout << "Enter Player 1's name: ";
    std::getline(*in, name1);
    std::cout << "Enter Player 2's name: ";
    std::getline(*in, name2);

    auto p1 = std::make_unique<Player>(name1, 1, CardFactory::loadDeck(deckFile1), this);
    auto p2 = std::make_unique<Player>(name2, 2, CardFactory::loadDeck(deckFile2), this);

    p1->drawInitialHand(testingMode);
    p2->drawInitialHand(testingMode);

    board = std::make_unique<Board>(this, std::move(p1), std::move(p2));
}

void Game::start() {
    while (true) {
        Player &p = getCurrentPlayer();
        p.startTurn();
        std::string cmd;
        while (std::getline(std::cin, cmd)) {
            if (cmd == "end") break;
            processCommand(cmd);
        }
        p.endTurn();
        togglePlayer();
    }
}

void Game::processCommand(const std::string &line) {
    Player& currentPlayer = getCurrentPlayer();
    std::istringstream iss{line};
    std::string cmd;
    iss >> cmd;

    if (cmd == "help") {
        helpMsg();
    } else if (cmd == "end") {
        // placeholder, "end" already processed at Game::start()
    } else if (cmd == "quit") {
        std::cout << "Game Over!" << std::endl;
        std::exit(EXIT_SUCCESS);
    } else if (cmd == "attack") {
        int i, j = -1;
        if (iss >> i) {
            if (!(iss >> j)) {
                j = -1;
            }
            currentPlayer.attack(i, j, getOtherPlayer());
        }
    } else if (cmd == "play") {
        int i, p = -1, t = -1;
        if (iss >> i) {
            if (iss >> p) {
                if (!(iss >> t)) {
                    t = -1;
                }
            }
            currentPlayer.playCard(i, p, t, testingMode);
        }
    } else if (cmd == "inspect") {

    } else if (cmd == "hand") {
        board->displayHand(currentPlayer);
    } else if (cmd == "board") {
        board->display();
    } else if (testingMode && cmd == "draw") {
        currentPlayer.drawCard();
    } else if (testingMode && cmd == "discard") {
        int i;
        if (iss >> i) {
            currentPlayer.discardCard(i);
        }  
    }
    else {
        std::cerr << "Unknown command." << std::endl;
    }
}

Player &Game::getPlayer(int index) {
    return board->getPlayer(index);
}

Player &Game::getCurrentPlayer() {
    return board->getPlayer(currentPlayer);
}

Player &Game::getOtherPlayer() {
    return board->getOpponent(currentPlayer);
}

void Game::togglePlayer() {
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

bool Game::isTestingMode() const {
    return testingMode;
}
