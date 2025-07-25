#include "ritual.h"
#include "minion.h"
#include <iostream>

Ritual::Ritual(const std::string &name, int cost, std::string description, int activationCost, int charges)
    : Card{name, cost, description}, activationCost{activationCost}, charges{charges} {}

CardType Ritual::getType() const { return CardType::Ritual; }

int Ritual::getActivationCost() const { return activationCost; }
int Ritual::getCharges() const { return charges; }
void Ritual::addCharges(int n) { charges += n; }
bool Ritual::canActivate() const { return charges >= activationCost; }
void Ritual::consumeCharge() { if (canActivate()) charges -= activationCost; }

DarkRitual::DarkRitual()
    : Ritual{"Dark Ritual", 0, "At the start of your, gain 1 magic", 1, 5} {}
void DarkRitual::trigger(Game &game) {
    std::cout << "Dark Ritual Triggered" << std::endl;
    Player &owner = game.getCurrentPlayer();
    owner.changeMagic(1);
}

AuraOfPower::AuraOfPower()
    : Ritual{"Aura of Power", 1, "Whenever a minion enters play under your control, it gains +1/+1", 1, 4} {}
void AuraOfPower::trigger(Game &game) {
    std::cout << "Aura of Power Triggered" << std::endl;
    Player &owner = game.getCurrentPlayer();
    for (auto& card : owner.getBoard()) {
        Minion *minion = dynamic_cast<Minion*>(card.get());
        if (minion) {
            minion->modifyStats(1, 1);
        }
    }
}

StandStill::StandStill()
    : Ritual{"Standstill", 3, "Whenever a minion enters play, destroy it", 2, 4} {}
void StandStill::trigger(Game &game) {
    std::cout << "Standstill Triggered" << std::endl;
    Player &opponent = game.getOtherPlayer();
    if (!opponent.getBoard().empty()) {
        auto &board = opponent.getBoard();
        opponent.getGraveyard().emplace_back(std::move(board.back()));
        board.pop_back();
    }
}

card_template_t Ritual::display() const {
    return display_ritual(name, cost, activationCost, description, charges);
}
