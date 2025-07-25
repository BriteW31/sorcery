#include "ability.h"
#include <iostream>

ActivatedAbility::ActivatedAbility(int cost) : cost{cost} {}

int ActivatedAbility::getCost() const {
    return cost;
}

void ActivatedAbility::addExtraCost(int extra) {
    cost += extra;
}

void ActivatedAbility::execute(Game &game, int targetPlayer, int targetCard) {
    std::cout << "ActivatedAbility::execute not implemented.\n";
}

AbilityType ActivatedAbility::getType() const {
    return AbilityType::Activated;
}

TriggeredAbility::TriggeredAbility(std::string key) : triggerKey{std::move(key)} {}

const std::string &TriggeredAbility::key() const {
    return triggerKey;
}

AbilityType TriggeredAbility::getType() const {
    return AbilityType::Triggered;
}

void TriggeredAbility::execute(Game &game, int targetPlayer, int targetCard) {
    std::cout << "TriggeredAbility::execute not implemented.\n";
}
