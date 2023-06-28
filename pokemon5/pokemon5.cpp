#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

enum class PokemonType {
    Fire,
    Water,
    Grass
};

class Pokemon {
private:
    std::string name;
    PokemonType strength;
    PokemonType weakness;

public:
    Pokemon(const std::string& name, PokemonType strength, PokemonType weakness)
        : name(name), strength(strength), weakness(weakness) {}

    std::string getName() const {
        return name;
    }

    PokemonType getStrength() const {
        return strength;
    }

    PokemonType getWeakness() const {
        return weakness;
    }

    virtual void battleCry() const {} // make it virtual
};

class Charmander : public Pokemon {
public:
    Charmander(const std::string& name)
        : Pokemon(name, PokemonType::Fire, PokemonType::Water) {}

    void battleCry() const override {
        std::cout << "Charmander: Char-char!" << std::endl;
    }
};

class Squirtle : public Pokemon {
public:
    Squirtle(const std::string& name)
        : Pokemon(name, PokemonType::Water, PokemonType::Grass) {}

    void battleCry() const override {
        std::cout << "Squirtle: squirtle-squirtle!" << std::endl;
    }
};

class Bulbasaur : public Pokemon {
public:
    Bulbasaur(const std::string& name)
        : Pokemon(name, PokemonType::Grass, PokemonType::Fire) {}

    void battleCry() const override {
        std::cout << "Bulbasaur: Bulba-Bulba!" << std::endl;
    }
};

class Trainer {
private:
    std::string name;
    std::vector<Pokemon*> belt;
    static const int MAX_POKEBALLS = 6;

public:
    Trainer(const std::string& n)
        : name(n) {}

    ~Trainer() {
        for (Pokemon* pokemon : belt) {
            delete pokemon;
        }
    }

    void addPokemon(Pokemon* pokemon) {
        if (belt.size() < MAX_POKEBALLS) {
            belt.push_back(pokemon);
        }
        else {
            throw std::runtime_error("Trainer's belt is full. Cannot add more Pokemon.");
        }
    }

    Pokemon* throwPokeball() const {
        if (belt.empty()) {
            throw std::runtime_error("Trainer doesn't have any Pokemon on the belt.");
        }

        int index = rand() % belt.size();
        Pokemon* pokemon = belt[index];
        std::cout << name << ", I choose you, " << pokemon->getName() << "!" << std::endl;
        pokemon->battleCry();
        return pokemon;
    }

    void returnPokemon(Pokemon* pokemon) const {
        std::cout << name << ", rest well, " << pokemon->getName() << "!" << std::endl;
        pokemon->battleCry();
    }

    int getNumPokeballs() const {
        return belt.size();
    }

    std::string getName() const {
        return name;
    }
};

class Battle;

class Arena {
private:
    int totalRounds;
    int totalBattles;

public:
    Arena() : totalRounds(0), totalBattles(0) {}

    void startBattle(Trainer* trainer1, Trainer* trainer2);

    void displayScoreboard() const {
        std::cout << "Scoreboard" << std::endl;
        std::cout << "Total Rounds: " << totalRounds << std::endl;
        std::cout << "Total Battles: " << totalBattles << std::endl;
    }

    void resetScoreboard() {
        totalBattles;
    }
};

class Battle {
private:
    Trainer* trainer1;
    Trainer* trainer2;

public:
    Battle(Trainer* t1, Trainer* t2)
        : trainer1(t1), trainer2(t2) {}

    void fight() {
        std::cout << "Battle starts!" << std::endl;

        Pokemon* pokemon1 = trainer1->throwPokeball();
        Pokemon* pokemon2 = trainer2->throwPokeball();

        PokemonType strength1 = pokemon1->getStrength();
        PokemonType strength2 = pokemon2->getStrength();
        PokemonType weakness1 = pokemon1->getWeakness();
        PokemonType weakness2 = pokemon2->getWeakness();

        if (strength1 == weakness2 && strength2 == weakness1) {
            // Draw
            std::cout << "It's a draw!" << std::endl;
            trainer1->returnPokemon(pokemon1);
            trainer2->returnPokemon(pokemon2);
        }
        else if (strength1 == weakness2) {
            // Trainer 1 wins
            std::cout << trainer1->getName() << " wins the round!" << std::endl;
            trainer2->returnPokemon(pokemon2);
        }
        else if (strength2 == weakness1) {
            // Trainer 2 wins
            std::cout << trainer2->getName() << " wins the round!" << std::endl;
            trainer1->returnPokemon(pokemon1);
        }

        std::cout << "Round ends!" << std::endl;
    }
};

void Arena::startBattle(Trainer* trainer1, Trainer* trainer2) {
    Battle battle(trainer1, trainer2);
    battle.fight();
    totalRounds++;
    totalBattles++;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    Arena arena;

    std::cout << "Enter the name of Trainer 1: ";
    std::string trainer1Name;
    std::getline(std::cin, trainer1Name);
    Trainer challengerOne(trainer1Name);

    std::cout << "Enter the name of Trainer 2: ";
    std::string trainer2Name;
    std::getline(std::cin, trainer2Name);
    Trainer opponentOne(trainer2Name);

    std::cout << "\n";

    challengerOne.addPokemon(new Squirtle("Squirtle"));
    challengerOne.addPokemon(new Bulbasaur("Bulbasaur"));
    challengerOne.addPokemon(new Charmander("Charmander"));

    opponentOne.addPokemon(new Squirtle("Squirtle"));
    opponentOne.addPokemon(new Bulbasaur("Bulbasaur"));
    opponentOne.addPokemon(new Charmander("Charmander"));

    bool restart = true;
    while (restart) {
        arena.startBattle(&challengerOne, &opponentOne);

        std::cout << "\n\n";

        arena.displayScoreboard();

        std::cout << "Do you want to restart? (y/n): ";
        std::string input;
        std::cin >> input;

        restart = (input == "y" || input == "Y");
        if (restart) {
            arena.resetScoreboard();
        }
    }

    return 0;
}