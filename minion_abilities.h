#ifndef MINION_ABILITIES_H
#define MINION_ABILITIES_H

#include "ability.h"
#include "minion.h"
#include "player.h"
#include "game.h"

class NovicePyromancerAbility : public ActivatedAbility {
public:
    NovicePyromancerAbility() : ActivatedAbility{1} {}
    void execute(Game &game, int targetPlayer, int targetCard) override;
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<NovicePyromancerAbility>(*this);
    }
};

class ApprenticeSummonerAbility : public ActivatedAbility {
public:
    ApprenticeSummonerAbility() : ActivatedAbility{1} {}
    void execute(Game &game, int, int) override;       
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<ApprenticeSummonerAbility>(*this);
    }
};

class MasterSummonerAbility : public ActivatedAbility {
public:
    MasterSummonerAbility() : ActivatedAbility{2} {}
    void execute(Game &game, int, int) override;     
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<MasterSummonerAbility>(*this);
    }
};


class BoneGolemTrigger : public TriggeredAbility {
    Minion *host;
public:
    explicit BoneGolemTrigger(Minion *self)
        : TriggeredAbility{"MinionLeaves"}, host{self} {}
    void execute(Game &game, int, int) override;
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<BoneGolemTrigger>(*this);
    }
};     

class FireElementalTrigger : public TriggeredAbility {
    Minion *host;
public:
    explicit FireElementalTrigger(Minion *self)
        : TriggeredAbility{"MinionEnters"}, host{self} {}
    void execute(Game &game, int targetPlayer, int targetCard) override;
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<FireElementalTrigger>(*this);
    }
};

class PotionSellerTrigger : public TriggeredAbility {
    Minion *host;
public:
    explicit PotionSellerTrigger(Minion *self)
        : TriggeredAbility{"EndOfTurn"}, host{self} {}
    void execute(Game &game, int, int) override;
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<PotionSellerTrigger>(*this);
    }
};

#endif
