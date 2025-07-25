#include "enchantment.h"
#include <iostream>
#include <iomanip>
using namespace std;
Enchantment::Enchantment(const std::string &name, int cost, std::string description, std::unique_ptr<Minion> target)
    : Minion{name, cost, description, 0, 0}, target{std::move(target)} {}

CardType Enchantment::getType() const {
    return CardType::Enchantment;
}

card_template_t Enchantment::display() const {
    if (overrideStats || attackModifier != 0 || defenseModifier != 0) {
        int attack = overrideStats ? newAttack : target->getAttack() + attackModifier;
        int defence = overrideStats ? newDefense : target->getDefense() + defenseModifier;

        return display_enchantment_attack_defence(
            getName(),
            getCost(),
            getDescription(),
            std::to_string(attack),
            std::to_string(defence)
        );
    } else {
        return display_enchantment(
            getName(),
            getCost(),
            getDescription()
        );
    }
}

void Enchantment::setStatOverride(int atk, int def) {
    overrideStats = true;
    newAttack = atk;
    newDefense = def;
}

void Enchantment::setStatModifier(int atkDelta, int defDelta) {
    attackModifier = atkDelta;
    defenseModifier = defDelta;
}

Minion &Enchantment::getBase() const {
    return *target;
}

std::unique_ptr<Minion> Enchantment::copyBase() const {
    return target->clone();
}

// Giant Strength: +2/+2
GiantStrength::GiantStrength(std::unique_ptr<Minion> target)
    : Enchantment{"Giant Strength", 1, "Enchanted minion gains +2/+2", std::move(target)} {
    setStatModifier(2, 2);
}

std::unique_ptr<Minion> GiantStrength::clone() const {
    return std::make_unique<GiantStrength>(copyBase());
}

GiantStrength::GiantStrength()
    : GiantStrength(std::make_unique<AirElemental>()) {}

// Enrage: doubles atk/def
Enrage::Enrage(std::unique_ptr<Minion> target)
    : Enchantment{"Enrage", 2, "Enchanted minion's ATK and DEF are doubled", std::move(target)} {}

int Enrage::getAttack() const {
    return getBase().getAttack() * 2;
}

int Enrage::getDefense() const {
    return getBase().getDefense() * 2;
}

std::unique_ptr<Minion> Enrage::clone() const {
    return std::make_unique<Enrage>(copyBase());
}

Enrage::Enrage()
    : Enrage(std::make_unique<AirElemental>()) {}

// Haste: +1 extra action
Haste::Haste(std::unique_ptr<Minion> target)
    : Enchantment{"Haste", 1, "Enchanted minion gains +1 action per turn", std::move(target)} {}

void Haste::restoreAction() {
    getBase().restoreAction(); // base action
    getBase().restoreAction(); // +1 from haste
}

std::unique_ptr<Minion> Haste::clone() const {
    return std::make_unique<Haste>(copyBase());
}

Haste::Haste()
    : Haste(std::make_unique<AirElemental>()) {}

// Magic Fatigue: +2 cost to activated abilities
MagicFatigue::MagicFatigue(std::unique_ptr<Minion> target)
    : Enchantment{"Magic Fatigue", 2, "Enchanted minion's activated ability costs 2 more", std::move(target)} {}

Ability *MagicFatigue::getAbility() {
    Ability *base = getBase().getAbility();
    if (base && base->getType() == AbilityType::Activated) {
        auto *aa = dynamic_cast<ActivatedAbility *>(base);
        if (aa) {
            aa->addExtraCost(2);  // Increase cost
        }
    }
    return base;
}

std::unique_ptr<Minion> MagicFatigue::clone() const {
    return std::make_unique<MagicFatigue>(copyBase());
}

MagicFatigue::MagicFatigue()
    : MagicFatigue(std::make_unique<AirElemental>()) {}

// Silence: disables all abilities
Silence::Silence(std::unique_ptr<Minion> target)
    : Enchantment{"Silence", 1, "Enchanted minion loses all abilities", std::move(target)} {}

Ability *Silence::getAbility() {
    return nullptr;
}

std::unique_ptr<Minion> Silence::clone() const {
    return std::make_unique<Silence>(copyBase());
}

Silence::Silence()
    : Silence(std::make_unique<AirElemental>()) {}
