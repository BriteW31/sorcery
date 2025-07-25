#include "minion_abilities.h"
#include "board.h"
#include "ritual.h"
#include <iostream>
#include "minion.h"   

static bool boardFull(Player &p) { return p.getBoard().size() >= 5; }
static void destroyIfDead(Player &p, int idx) {
    auto &b = p.getBoard();
    if (auto m = dynamic_cast<Minion*>(b[idx].get())) {
        if (m->getDefense() <= 0) p.destroyMinion(idx);
    }
}

void NovicePyromancerAbility::execute(Game &game,
                                      int targetPlayer, int targetCard) {
    Player &caster   = game.getCurrentPlayer();
    if (caster.getMagic() < getCost()) return;    
    Player &victim   = game.getPlayer(targetPlayer);
    auto &b          = victim.getBoard();
    if (targetCard < 0 || targetCard >= static_cast<int>(b.size())) return;

    caster.changeMagic(-getCost());
    if (auto m = dynamic_cast<Minion*>(b[targetCard].get())) {
        m->modifyStats(0, -1);
        destroyIfDead(victim, targetCard);
    }
}
void ApprenticeSummonerAbility::execute(Game &game, int, int) {
    Player &caster = game.getCurrentPlayer();
    if (caster.getMagic() < getCost() || boardFull(caster)) return;
    caster.changeMagic(-getCost());
    caster.getBoard().emplace_back(std::make_unique<AirElemental>());
}

void MasterSummonerAbility::execute(Game &game, int, int) {
    Player &caster = game.getCurrentPlayer();
    if (caster.getMagic() < getCost()) return;
    caster.changeMagic(-getCost());
    for (int i = 0; i < 3 && !boardFull(caster); ++i) {
        caster.getBoard().emplace_back(std::make_unique<AirElemental>());
    }
}

void BoneGolemTrigger::execute(Game &, int, int) {
    host->modifyStats(1, 1);
}

void FireElementalTrigger::execute(Game &game,int targetPlayer, int targetCard) {
    Player &enemy = game.getPlayer(targetPlayer);
    if (&enemy == &game.getCurrentPlayer()) return;
    auto &b = enemy.getBoard();
    if (targetCard < 0 || targetCard >= static_cast<int>(b.size())) return;
    if (auto m = dynamic_cast<Minion*>(b[targetCard].get())) {
        m->modifyStats(0, -1);
        destroyIfDead(enemy, targetCard);
    }
}

void PotionSellerTrigger::execute(Game &game, int, int) {
    Player &owner = game.getCurrentPlayer();
    for (auto &cardPtr : owner.getBoard()) {
        if (auto m = dynamic_cast<Minion*>(cardPtr.get())) {
            m->modifyStats(0, 1);
        }
    }
}
