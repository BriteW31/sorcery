#include "player.h"
#include "minion.h"
#include "ritual.h"
#include "spell.h"
#include "enchantment.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

Player::Player(const std::string &name, int id, std::vector<std::unique_ptr<Card>> &&deck, Game *game)
    : name{name}, id{id}, deck{std::move(deck)}, game{game} {}

void Player::drawCard() {
    if (hand.size() < 5 && !deck.empty()) {
        hand.emplace_back(std::move(deck.back()));
        deck.pop_back();
    }
}

void Player::drawInitialHand(bool isTestingMode) {
    for (int i = 0; i < 5; i++) shuffleAndDraw(isTestingMode, 12345);
}

void Player::playCard(int index, int targetPlayer, int targetCard, bool isTestingMode) {
    if (index < 1 || index > hand.size()) {
        std::cout << hand.size() << std::endl;
        std::cerr << "Invalid hand index" << std::endl;
        return;
    }
    auto &currentCard = hand.at(index - 1);
    if (currentCard->getCost() <= magic) {
        changeMagic(-currentCard->getCost());
    } else {
        if (isTestingMode && currentCard->getType() == CardType::Spell) {
            changeMagic(-getMagic());
        } else {
            std::cout << "No enough magic " << std::endl;
            return;
        }
    }
    switch (currentCard->getType()) {
        case CardType::Minion: {
            if (board.size() >= 5) {
                std::cout << "Board full!" << std::endl;
                changeMagic(currentCard->getCost());
                return;
            }
            std::cout << name << " placed " << currentCard->getName() << std::endl;
            board.emplace_back(std::move(currentCard));
            break;
        }
            
        case CardType::Spell: {
            Spell *spell = dynamic_cast<Spell*>(currentCard.get());
            if (spell->effect(game, targetPlayer, targetCard - 1)) {
                std::cout << name << " played spell: " << spell->getName() << std::endl;
            }
            else {
                changeMagic(currentCard->getCost());
            }
            break;
        }

        case CardType::Ritual: {
            ritual = std::move(hand.at(index - 1));
            std::cout << name << " played ritual: " << ritual->getName() << std::endl;
            break;
        }

        case CardType::Enchantment: {
            if (targetPlayer != id) {
                std::cerr << "Can only enchant your own minions." << std::endl;
                changeMagic(currentCard->getCost());
                return;
            }
            if (targetCard < 1 || targetCard > static_cast<int>(board.size())) {
                std::cerr << "Invalid target minion index for enchantment." << std::endl;
                changeMagic(currentCard->getCost());
                return;
            }

            std::unique_ptr<Card> targetCardPtr = std::move(board.at(targetCard - 1));
            Minion *targetMinion = dynamic_cast<Minion*>(targetCardPtr.get());
            if (!targetMinion) {
                std::cerr << "Target is not a minion." << std::endl;
                changeMagic(currentCard->getCost());
                return;
            }

            // Transfer ownership of the target to a new enchantment
            std::unique_ptr<Minion> targetMinionPtr{dynamic_cast<Minion*>(targetCardPtr.release())};

            std::string enchantmentName = currentCard->getName();
            std::unique_ptr<Minion> wrapped;

            if (enchantmentName == "Giant Strength") {
                wrapped = std::make_unique<GiantStrength>(std::move(targetMinionPtr));
            } else if (enchantmentName == "Enrage") {
                wrapped = std::make_unique<Enrage>(std::move(targetMinionPtr));
            } else if (enchantmentName == "Haste") {
                wrapped = std::make_unique<Haste>(std::move(targetMinionPtr));
            } else if (enchantmentName == "Magic Fatigue") {
                wrapped = std::make_unique<MagicFatigue>(std::move(targetMinionPtr));
            } else if (enchantmentName == "Silence") {
                wrapped = std::make_unique<Silence>(std::move(targetMinionPtr));
            } else {
                std::cerr << "Unknown enchantment." << std::endl;
                return;
            }

            // Replace minion on board
            board.at(targetCard - 1) = std::move(wrapped);

            std::cout << name << " enchanted minion with " << enchantmentName << std::endl;

            break;
        }
    }
    hand.erase(hand.begin() + (index - 1));
}

void Player::attack(int attackerIdx, int defenserIdx, Player &opponent) {
    if (attackerIdx < 1 || attackerIdx > static_cast<int>(board.size())) {
        std::cout << "Invalid attacker" << std::endl;
        return;
    }
    Card *attackCard = board.at(attackerIdx - 1).get();
    Minion *attacker = dynamic_cast<Minion*>(attackCard);
    if (!attacker->canAct()) {
        std::cout << attacker->getName() << " cannot act" << std::endl;
        return;
    }
    attacker->spendAction();
    if (defenserIdx == -1) {
        std::cout << "attacks opponent directly" << std::endl;
        opponent.changeLife(-attacker->getAttack());
    } else {
        if (defenserIdx < 1 || defenserIdx > static_cast<int>(opponent.board.size())) {
            std::cout << "Invalid target minion index" << std::endl;
            return;
        }
        Card *targetCard = opponent.board.at(defenserIdx - 1).get();
        Minion *target = dynamic_cast<Minion*>(targetCard);
        if (!target) {
            std::cout << "Target card is not a minion" << std::endl;
            return;
        }
        
        attacker->setDefense(attacker->getDefense() - target->getAttack());
        target->setDefense(target->getDefense() - attacker->getAttack());

        if (attacker->getDefense() <= 0) {
            graveyard.emplace_back(std::move(board.at(attackerIdx - 1)));
            board.erase(board.begin() + (attackerIdx - 1));
        }
        if (target->getDefense() <= 0) {
            opponent.graveyard.emplace_back(std::move(opponent.board.at(defenserIdx - 1)));
            opponent.board.erase(opponent.board.begin() + (defenserIdx - 1));
        }
    }
}

void Player::startTurn() {
    magic++;
    drawCard();
    std::cout << name << " starts turn with " << magic << " magic.\n";

    for (auto& card : board) {
        Minion* minion = dynamic_cast<Minion*>(card.get());
        if (minion) {
            minion->restoreAction();
        }
    }
}

void Player::endTurn() {
    std::cout << name << " ends turn.\n";
    // TODO: Trigger end-of-turn effects
}

void Player::shuffleAndDraw(bool testingMode, unsigned seed) {
    if (!testingMode) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    std::default_random_engine rng{seed};
    std::shuffle(deck.begin(), deck.end(), rng);

    if (!deck.empty() && hand.size() < 5) {
        hand.emplace_back(std::move(deck.back()));
        deck.pop_back();
    }
}

std::string Player::getName() const { return name; }
int Player::getLife() const { return life; }
int Player::getMagic() const { return magic; }
void Player::changeLife(int delta) { life += delta; }
void Player::changeMagic(int delta) { magic += delta; }
std::vector<std::unique_ptr<Card>> &Player::getHand() { return hand; }
std::vector<std::unique_ptr<Card>> &Player::getBoard() { return board; }
std::vector<std::unique_ptr<Card>> &Player::getGraveyard() { return graveyard; }
Game *Player::getGame() { return game; }
Ritual *Player::getRitual() { return dynamic_cast<Ritual *>(ritual.get()); }

bool Player::destroyMinion(int index) {
    if (index >= 0 && index < static_cast<int>(board.size())) {
        std::cout << board.at(index)->getName() << "died" << std::endl;
        graveyard.emplace_back(std::move(board.at(index)));
        board.erase(board.begin() + index);
        return true;
    } else {
        return false;
    }
}

bool Player::removeRitual() {
    if (ritual) {
        std::cout << ritual->getName() << "removed" << std::endl;
        ritual.reset();
        return true;
    } else {
        return false;
    }
}

void Player::discardCard(int index) {
    if (index < 1 || index > hand.size()) {
        std::cout << "Wrong index" << std::endl;
    } else {
        hand.erase(hand.begin() + index - 1);
    }
}
