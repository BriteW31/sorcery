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

    virtual int getAttack() const;
    virtual int getDefense() const;
    virtual void setAttack(int val);
    virtual void setDefense(int val);
    virtual void modifyStats(int atkDelta, int defDelta);

    virtual bool canAct() const;
    virtual void restoreAction();
    virtual void spendAction();

    virtual Ability *getAbility();
    virtual void setAbility(std::unique_ptr<Ability> ability);

    card_template_t display() const override;
};

class AirElemental : public Minion {
public:
    AirElemental();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<AirElemental>(*this);  // use copy constructor
    }
};

class EarthElemental : public Minion {
public:
    EarthElemental();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<EarthElemental>(*this);  // use copy constructor
    }
};

class BoneGolem : public Minion {
public:
    BoneGolem();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<BoneGolem>(*this);  // use copy constructor
    }
};

class FireElemental : public Minion {
public:
    FireElemental();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<FireElemental>(*this);  // use copy constructor
    }
};

class PotionSeller : public Minion {
public:
    PotionSeller();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<PotionSeller>(*this);  // use copy constructor
    }
};

class NovicePyromancer : public Minion {
public:
    NovicePyromancer();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<NovicePyromancer>(*this);  // use copy constructor
    }
};

class ApprenticeSummoner : public Minion {
public:
    ApprenticeSummoner();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<ApprenticeSummoner>(*this);  // use copy constructor
    }
};

class MasterSummoner : public Minion {
public:
    MasterSummoner();
    std::unique_ptr<Minion> clone() const override {
        return std::make_unique<MasterSummoner>(*this);  // use copy constructor
    }
};

#endif
