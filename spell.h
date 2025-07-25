#ifndef SPELL_H
#define SPELL_H

#include "card.h"
#include "minion.h"
#include "ascii_graphics.h"

class Game;

class Spell : public Card {
public:
    Spell(const std::string &name, int cost, std::string description);
    CardType getType() const override;
    card_template_t display() const override;
    virtual bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) = 0;
};

class Banish : public Spell {
public:
    Banish();
    bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) override;
};

class Unsummon : public Spell {
public:
    Unsummon();
    bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) override;
};

class Recharge : public Spell {
public:
    Recharge();
    bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) override;
};

class Disenchant : public Spell {
public:
    Disenchant();
    bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) override;
};

class RaiseDead : public Spell {
public:
    RaiseDead();
    bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) override;
};

class Blizzard : public Spell {
public:
    Blizzard();
    bool effect(Game *game, int targetPlayer = -1, int targetCard = -1) override;
};

#endif
