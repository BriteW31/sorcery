#ifndef ABILITY_H
#define ABILITY_H

#include <string>
#include <memory>

class Game;

enum class AbilityType { Activated, Triggered };

class Ability {
public:
    virtual std::unique_ptr<Ability> clone() const = 0;    
    virtual void execute(Game &game, int targetPlayer = -1, int targetCard = -1) = 0;
    virtual AbilityType getType() const = 0;
    virtual ~Ability() = default;
};

class ActivatedAbility : public Ability {
    int cost;

public:
    ActivatedAbility(int cost);

    int getCost() const;
    void addExtraCost(int extra); // <-- Add this

    void execute(Game &game, int targetPlayer = -1, int targetCard = -1) override;
    AbilityType getType() const override;
};

class TriggeredAbility : public Ability {
    std::string triggerKey;

public:
    explicit TriggeredAbility(std::string key);
    const std::string &key() const;
    AbilityType getType() const override;
    void execute(Game &game, int targetPlayer = -1, int targetCard = -1) override;
};

#endif
