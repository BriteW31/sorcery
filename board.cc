#include "board.h"
#include "ritual.h"
#include <iostream>

Board::Board(Game *game,std::unique_ptr<Player> p1, std::unique_ptr<Player> p2)
    : player1{std::move(p1)}, player2{std::move(p2)} {}

Player &Board::getPlayer(int id) {
    return (id == 1) ? *player1 : *player2;
}

Player &Board::getOpponent(int id) {
    return (id == 1) ? *player2 : *player1;
}

WinState Board::whoWin() const {
    if (player1->getLife() <= 0 && player2->getLife() <= 0) return WinState::Tie;
    else if (player1->getLife() <= 0) return WinState::Player2;
    else if (player2->getLife() <= 0) return WinState::Player1;
    else return WinState::NotEnd;
}

void Board::print(const std::vector<card_template_t> &cards, int height) const {
    for (int line = 0; line < height; line++) {
        for (const auto &card : cards) {
            std::cout << card[line];
        }
        std::cout << std::endl;
    }
}

void Board::display() const {
    const int BOARD_LIMIT = 5;

    std::vector<card_template_t> p1Minions;
    std::vector<card_template_t> p2Minions;

    Player& p1 = *player1;
    Player& p2 = *player2;

    auto &p1Board = player1->getBoard();
    auto &p2Board = player2->getBoard();

    card_template_t p1ritual = p1.getRitual() ? p1.getRitual()->display() : CARD_TEMPLATE_BORDER;
    card_template_t p1graveyard = !p1.getGraveyard().empty() ? p1.getGraveyard().back()->display() : CARD_TEMPLATE_BORDER;

    card_template_t p2ritual = p2.getRitual() ? p2.getRitual()->display() : CARD_TEMPLATE_BORDER;
    card_template_t p2graveyard = !p2.getGraveyard().empty() ? p2.getGraveyard().back()->display() : CARD_TEMPLATE_BORDER;

    card_template_t p1Info = display_player_card(1, p1.getName(), p1.getLife(), p1.getMagic());
    card_template_t p2Info = display_player_card(2, p2.getName(), p2.getLife(), p2.getMagic());

    for (const auto &minion : p1Board) {
        p1Minions.emplace_back(minion->display());
    }
    while (p1Minions.size() < BOARD_LIMIT) {
        p1Minions.push_back(CARD_TEMPLATE_BORDER);
    }
    for (const auto &minion : p2Board) {
        p2Minions.emplace_back(minion->display());
    }
    while (p2Minions.size() < BOARD_LIMIT) {
        p2Minions.push_back(CARD_TEMPLATE_BORDER);
    }

    print(p1Minions, BLOCK_HEIGHT);
    print({p1ritual, CARD_TEMPLATE_EMPTY, p1Info, CARD_TEMPLATE_EMPTY, p1graveyard}, BLOCK_HEIGHT);
    print({CENTRE_GRAPHIC}, BLOCK_HEIGHT - 1);
    print({p2ritual, CARD_TEMPLATE_EMPTY, p2Info, CARD_TEMPLATE_EMPTY, p2graveyard}, BLOCK_HEIGHT);
    print(p2Minions, BLOCK_HEIGHT);
}

void Board::displayHand(Player& player) const {
    std::vector<card_template_t> handGraphics;

    for (const auto& card : player.getHand()) {
        handGraphics.push_back(card->display());
    }

    if (handGraphics.empty()) {
        std::cout << "Hand is empty." << std::endl;
        return;
    }

    print(handGraphics, BLOCK_HEIGHT);
}


void Board::registerListener(const std::string& key, Ability* ability) {
    listeners.push_back({key, ability});
}

void Board::notify(const std::string& key, int p, int idx) {
    for (auto& L : listeners)
        if (L.key == key) L.cb->execute(*game, p, idx);
}
void Board::start_turn()              { notify("StartTurn"); }
void Board::end_turn()                { notify("EndTurn");   }
void Board::minion_enter(int p,int i) { notify("MinionEnters", p, i); }
void Board::minion_leave(int p,int i) { notify("MinionLeaves", p, i); }
