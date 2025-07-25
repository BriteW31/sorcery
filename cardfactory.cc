#include "cardfactory.h"
#include "minion.h"
#include "spell.h"
#include "ritual.h"
#include "enchantment.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::unique_ptr<Card>> CardFactory::loadDeck(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<std::unique_ptr<Card>> deck;
    std::string name;
    while (std::getline(file, name)) {
        auto card = createCard(name);
        if (card) deck.emplace_back(std::move(card));
    }
    return deck;
}

std::unique_ptr<Card> CardFactory::createCard(const std::string &name) {
    if (name == "Air Elemental") return std::make_unique<AirElemental>();
    else if (name == "Earth Elemental") return std::make_unique<EarthElemental>();
    else if (name == "Bone Golem") return std::make_unique<BoneGolem>();
    else if (name == "Fire Elemental") return std::make_unique<FireElemental>();
    else if (name == "Potion Seller") return std::make_unique<PotionSeller>();
    else if (name == "Novice Pyromancer") return std::make_unique<NovicePyromancer>();
    else if (name == "Apprentice Summoner") return std::make_unique<ApprenticeSummoner>();
    else if (name == "Master Summoner") return std::make_unique<MasterSummoner>();
    else if (name == "Dark Ritual") return std::make_unique<DarkRitual>();
    else if (name == "Aura of Power") return std::make_unique<AuraOfPower>();
    else if (name == "Standstill") return std::make_unique<StandStill>();
    else if (name == "Banish") return std::make_unique<Banish>();
    else if (name == "Unsummon") return std::make_unique<Unsummon>();
    else if (name == "Recharge") return std::make_unique<Recharge>();
    else if (name == "Disenchant") return std::make_unique<Disenchant>();
    else if (name == "Raise Dead") return std::make_unique<RaiseDead>();
    else if (name == "Blizzard") return std::make_unique<Blizzard>();
    else if (name == "Giant Strength") return std::make_unique<GiantStrength>();
    else if (name == "Enrage") return std::make_unique<Enrage>();
    else if (name == "Haste") return std::make_unique<Haste>();
    else if (name == "Magic Fatigue") return std::make_unique<MagicFatigue>();
    else if (name == "Silence") return std::make_unique<Silence>();
    else {
        std::cerr << "Unknown card name: " << name << std::endl;
        return nullptr;
    }
}
