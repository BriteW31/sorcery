#ifndef MINION_H
#define MINION_H

#include "ability.h"
#include "card.h"
#include "ascii_graphics.h"
#include <memory>


class Minion : public Card {
protected:
    int atk;
    int def;
    int actions;
    std::unique_ptr<Ability> ability;

public:
    Minion(const std::string &name, int cost, std::string description, int atk, int def);

    virtual std::unique_ptr<Minion> clone() const = 0;
    Minion(const Minion &other);
    CardType getType() const override;

    int getAttack() const;
    int getDefense() const;
    void setAttack(int val);
    void setDefense(int val);
    void modifyStats(int atkDelta, int defDelta);

    bool canAct() const;
    void restoreAction();
    void spendAction();

    virtual Ability *getAbility();
    void setAbility(std::unique_ptr<Ability> ability);

    card_template_t display() const override;
};

class AirElemental : public Minion {
public:
    AirElemental();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<AirElemental>(*this); 
    }
};

class EarthElemental : public Minion {
public:
    EarthElemental();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<EarthElemental>(*this); 
    }
};

class BoneGolem : public Minion {
public:
    BoneGolem();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<BoneGolem>(*this); 
    }
};

class FireElemental : public Minion {
public:
    FireElemental();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<FireElemental>(*this); 
    }
};

class PotionSeller : public Minion {
public:
    PotionSeller();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<PotionSeller>(*this); 
    }
};

class NovicePyromancer : public Minion {
public:
    NovicePyromancer();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<NovicePyromancer>(*this); 
    }
};

class ApprenticeSummoner : public Minion {
public:
    ApprenticeSummoner();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<ApprenticeSummoner>(*this); 
    }
};

class MasterSummoner : public Minion {
public:
    MasterSummoner();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<MasterSummoner>(*this); 
    }
};

#endif
