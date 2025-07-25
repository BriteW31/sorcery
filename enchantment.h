#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include "minion.h"
#include "ascii_graphics.h"
#include <memory>

class Enchantment : public Minion {
    std::unique_ptr<Minion> target;
    int attackModifier = 0;
    int defenseModifier = 0;
    bool overrideStats = false;
    int newAttack = 0;
    int newDefense = 0;

public:
    Enchantment(const std::string &name, int cost, std::string description, std::unique_ptr<Minion> target);

    CardType getType() const override;
    card_template_t display() const override;

    void setStatOverride(int atk, int def);
    void setStatModifier(int atkDelta, int defDelta);

    Minion &getBase() const;
    std::unique_ptr<Minion> copyBase() const;
};

class GiantStrength : public Enchantment {
public:
    GiantStrength(std::unique_ptr<Minion> target);
    GiantStrength();
    std::unique_ptr<Minion> clone() const;
};

class Enrage : public Enchantment {
public:
    Enrage(std::unique_ptr<Minion> target);
    Enrage();
    std::unique_ptr<Minion> clone() const;
    int getAttack() const;
    int getDefense() const;
};

class Haste : public Enchantment {
public:
    Haste(std::unique_ptr<Minion> target);
    Haste();
    std::unique_ptr<Minion> clone() const;
    void restoreAction();
};

class MagicFatigue : public Enchantment {
public:
    MagicFatigue(std::unique_ptr<Minion> target);
    MagicFatigue();
    std::unique_ptr<Minion> clone() const;
    Ability *getAbility() override;
};

class Silence : public Enchantment {
public:
    Silence(std::unique_ptr<Minion> target);
    Silence();
    std::unique_ptr<Minion> clone() const;
    Ability *getAbility() override;
};

#endif
