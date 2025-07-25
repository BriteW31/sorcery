#include "spell.h"
#include "game.h"
#include "ritual.h"
#include "enchantment.h"
#include <iostream>
#include <iomanip>

Spell::Spell(const std::string &name, int cost, std::string description) : Card{name, cost, description} {}

CardType Spell::getType() const { return CardType::Spell; }

Banish::Banish(): Spell{"Banish", 2, "Destroy target minion or ritual"} {}
Unsummon::Unsummon(): Spell{"Unsummon", 1, "Return target minion to its owner's hand"}{}
Recharge::Recharge(): Spell{"Recharge", 1, "Your ritual gains 3 charges"} {}
Disenchant::Disenchant(): Spell{"Disenchant", 1, "Destroy the top enchantment on target minion"} {}
RaiseDead::RaiseDead(): Spell{"Raise Dead", 1, "Resurrect the top minion in your graveyard and set its defence to 1"} {}
Blizzard::Blizzard(): Spell{"Blizzard", 3, "Deal 2 damage to all minions"} {}

card_template_t Spell::display() const {
    return display_spell(name, cost, description);
}

void Banish::effect(Game *game, int targetPlayer, int targetCard) {
    targetCard--;
    auto &p = game->getPlayer(targetPlayer);
    if (targetCard == 6) {
        p.removeRitual();
    } else {
        p.destroyMinion(targetCard);
    }
}
void Unsummon::effect(Game *game, int targetPlayer, int targetIndex) {
    auto &player = game -> getPlayer(targetPlayer);
    auto &board = player.getBoard();
    auto &hand = player.getHand();
    if (targetIndex < 1 || targetIndex > board.size() || hand.size() >= 5) return;

    std::unique_ptr<Card> card = std::move(board[targetIndex - 1]);
    board.erase(board.begin() + (targetIndex - 1));
    hand.emplace_back(std::move(card));
}
void Recharge::effect(Game *game, int targetPlayer, int targetCard) {
    auto ritual = game->getPlayer(targetPlayer).getRitual();
    if (ritual) {
        ritual->addCharges(3);
    }
}
void Disenchant::effect(Game *game, int targetPlayer, int targetCard) {
    auto &player = game -> getPlayer(targetPlayer);
    auto &board = player.getBoard();
    auto &card = board.at(targetCard);

    Enchantment *enchantment = dynamic_cast<Enchantment *>(card.get());
    if (enchantment) {
        card = enchantment->copyBase();
    } else {
        std::cout << "This minion has no enchantments to remove" << std::endl;
    }
}
void RaiseDead::effect(Game *game, int targetPlayer, int targetCard) {
    auto &player = game -> getPlayer(targetPlayer);
    auto &graveyard = player.getGraveyard();
    auto &board = player.getBoard();
    if (graveyard.empty() || board.size() >= 5) {
        std::cerr << "Graveyard is empty or board is full" << std::endl;
        return;
    }
    std::unique_ptr<Card> card = std::move(graveyard.back());
    graveyard.pop_back();
    Minion *minion = dynamic_cast<Minion*>(card.get());
    minion->setDefense(1);
    board.emplace_back(std::move(card));
}
void Blizzard::effect(Game *game, int targetPlayer, int targetCard) {
    for (int p = 0; p < 2; p++) {
        auto &player = game -> getPlayer(p);
        for (size_t i = 0; i < player.getBoard().size(); i++) {
            Card *card = player.getBoard()[i].get();
            Minion *minion = dynamic_cast<Minion*>(card);
            minion -> setDefense(minion->getDefense() - 2);
            if (minion->getDefense() <= 0) {
                player.destroyMinion(i);
                i--;
            }
        }
    }
}