#include "ability.h"
#include "board.h"
#include "card.h"
#include "cardfactory.h"
#include "enchantment.h"
#include "game.h"
#include "minion.h"
#include "player.h"
#include "ritual.h"
#include "spell.h"
#include <iostream>
#include <vector>
#include <memory>

struct InitOptions {
    bool testingMode = false;
    bool graphicsMode = false;
    std::string deck1 = "default.deck";
    std::string deck2 = "default.deck";
    std::string initFile = "";
};

InitOptions processArguments(int argc, char* argv[]) {
    InitOptions options;

    for (int i = 1; i < argc; i++) {
        std::string curr = argv[i];
        if (curr == "-deck1") {
            if (i + 1 < argc) {
                options.deck1 = argv[++i];
            } else {
                throw std::invalid_argument("Missing argument for -deck1");
            }
        } else if (curr == "-deck2") {
            if (i + 1 < argc) {
                options.deck2 = argv[++i];
            } else {
                throw std::invalid_argument("Missing argument for -deck2");
            }
        } else if (curr == "-init") {
            if (i + 1 < argc) {
                options.initFile = argv[++i];
            } else {
                throw std::invalid_argument("Missing argument for -init");
            }
        } else if (curr == "-testing") {
            options.testingMode = true;
        } else if (curr == "-graphics") {
            options.graphicsMode = true;
        } else {
            throw std::invalid_argument("Unknown argument: " + curr);
        }
    }

    return options;
}


int main(int argc, char* argv[]) {
    try {
        InitOptions options = processArguments(argc, argv);
        Game game{options.testingMode, options.graphicsMode, options.deck1, options.deck2, options.initFile};
        game.init();
        game.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } 
    return 0;
}