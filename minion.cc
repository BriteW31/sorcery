#include "minion.h"
#include "minion_abilities.h"
#include <iostream>
#include <iomanip>
#include "minion_abilities.h"

Minion::Minion(const std::string &name, int cost, std::string description, int atk, int def)
    : Card{name, cost, description}, atk{atk}, def{def}, actions{0} {}

CardType Minion::getType() const { return CardType::Minion; }

int Minion::getAttack() const { return atk; }
int Minion::getDefense() const { return def; }
void Minion::setAttack(int val) { atk = val; }
void Minion::setDefense(int val) { def = val; }
void Minion::modifyStats(int atkDelta, int defDelta) { atk += atkDelta; def += defDelta; }

bool Minion::canAct() const { return actions > 0; }
void Minion::restoreAction() { actions = 1; }
void Minion::spendAction() { if (actions > 0) --actions; }

Ability* Minion::getAbility() { return ability.get(); }
void Minion::setAbility(std::unique_ptr<Ability> ability) { this -> ability = std::move(ability); }

AirElemental::AirElemental(): Minion{"Air Elemental", 0, "", 1, 1} {}
EarthElemental::EarthElemental(): Minion{"Earth Elemental", 3, "", 4, 4} {}

BoneGolem::BoneGolem()
  : Minion{"Bone Golem", 2, "Gain +1/+1 whenever a minion dies", 1, 3}
{
    setAbility(std::make_unique<BoneGolemTrigger>(this));
}

FireElemental::FireElemental()
  : Minion{"Fire Elemental", 2, "Deal 1 damage to minion entering play", 2, 2}
{
    setAbility(std::make_unique<FireElementalTrigger>(this));
}

PotionSeller::PotionSeller()
  : Minion{"Potion Seller", 2, "End of turn: friendly minions get +0/+1", 1, 3}
{
    setAbility(std::make_unique<PotionSellerTrigger>(this));
}

NovicePyromancer::NovicePyromancer()
  : Minion{"Novice Pyromancer", 1, "Deal 1 damage to a minion", 0, 1}
{
    setAbility(std::make_unique<NovicePyromancerAbility>());
}

ApprenticeSummoner::ApprenticeSummoner()
  : Minion{"Apprentice Summoner", 1, "Summon one Air Elemental", 1, 1}
{
    setAbility(std::make_unique<ApprenticeSummonerAbility>());
}

MasterSummoner::MasterSummoner()
  : Minion{"Master Summoner", 3, "Summon up to three Air Elementals", 2, 3}
{
    setAbility(std::make_unique<MasterSummonerAbility>());
}


card_template_t Minion::display() const {
    /*
    if (ability) {
        return display_minion_activated_ability(name, cost, atk, def, abilityCost, abilityDesc);
    } else {
        return display_minion_no_ability(name, cost, atk, def);
    }
    */
    return display_minion_no_ability(name, cost, atk, def);
}


/*
BoneGolem::BoneGolem(): Minion{"Bone Golem", 2, "Gain +1/+1 whenever a minion leaves play.", 1, 3} {}
FireElemental::FireElemental(): Minion{"Fire Elemental", 2, "Whenever an opponent's minion enters play, deal 1 damage to it.", 2, 2} {}
PotionSeller::PotionSeller(): Minion{"Potion Seller", 2, "At the end of your turn, all your minions gain +0/+1.", 1, 3} {}
NovicePyromancer::NovicePyromancer(): Minion{"Novice Pyromancer", 1, "Deal 1 damage to target minion", 0, 1} {}
ApprenticeSummoner::ApprenticeSummoner(): Minion{"Apprentice Summoner", 1, "Summon a 1/1 air elemental", 1, 1} {}
MasterSummoner::MasterSummoner(): Minion{"Master Summoner", 3, "Summon up to three 1/1 air elementals", 2, 3} {}
*/

