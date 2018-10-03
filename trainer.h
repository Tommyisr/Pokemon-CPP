#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "pokemon.h"
#include "exceptions.h"
#include "item.h"
namespace mtm {
namespace pokemongo {

// Teams in game.
typedef enum {
	BLUE, YELLOW, RED, NEUTRAL
} Team;

class Trainer {
//private:
private:
	std::string name;
	Team team;
	int level;
	std::vector<Pokemon> pokemons;
	std::string location_name; //TODO whats that - what happen when it moves to diff location?
	std::vector<Item*> items;
	int isLeader;
public:
	static int ScoreRedTeam;
	static int ScoreBlueTeam;
	static int ScoreYellowTeam;
	Trainer();
	// Constructs a new trainer with the given name and team.
	//
	// @param name the name of the trainer.
	// @param team the team to which the trainer belongs.
	// @throw TrainerInvalidArgsException if name is an empty string.
	Trainer(const std::string& name, const Team& team,
			const std::string& location_name);

	// Returns a reference to the strongest Pokemon the trainer owns. Strongest
	// Pokemon is determined using the comparison operators provided by the class
	// Pokemon. If two Pokemons are of equal strength, the function returns the
	// one that was caught earlier by the trainer.
	//
	// @return the strongest pokemon.
	// @throw TrainerNoPokemonsException if trainer has no Pokemons.
	Pokemon& GetStrongestPokemon();
	const Pokemon& GetStrongestPokemon() const;

	// Kills the strongest Pokemon. Removes the Pokemon that is returned from
	// GetStrongestPokemon() from the collection of Pokemons owned by the trainer.
	//
	// @throw TrainerNoPokemonsException if trainer has no Pokemons.
	void KillStrongestPokemon();

	// Comparison operators for trainers. Trainers are compared as described in
	// the exercise sheet.
	//
	// @param rhs the right-hand side operand.
	bool operator==(const Trainer& rhs) const;
	bool operator!=(const Trainer& rhs) const;
	bool operator<(const Trainer& rhs) const;
	bool operator>(const Trainer& rhs) const;
	bool operator<=(const Trainer& rhs) const;
	bool operator>=(const Trainer& rhs) const;

	// Checks whether a given trainer is in the same team as this trainer.
	//
	// @param trainer
	// @return true iff trainer (argument) and this trainer belong to the same
	//         team.
	bool IsAlly(const Trainer& trainer) const;

	// Returns the team to which the trainer belongs.
	//
	// @return the team to which the trainer belongs.
	Team GetTeam() const;

	// Tries to catch a Pokemon.
	//
	// @param pokemon the Pokemon the trainer wishes to catch.
	// @return true if the attempt succeeded.
	bool TryToCatch(Pokemon& pokemon);

	// Prints the data of the trainer in the following format:
	//
	//     "<name> (<level>) <team>\n
	//      <pokemon1>\n
	//      <pokemon2>\n
	//      ..."
	//
	// where the Pokemons are ordered from least recently caught to most recently.
	// e.g.
	//
	//     ash (4) YELLOW
	//     pikachu (3/5.5/87) NORMAL FIRE FLYING
	//     charmander (2/2.5/100) NORMAL BUG
	//     pikachu (7/6.5/56) FIRE
	//
	//
	// @param output the output stream to which the data is printed.
	// @param pokemon the pokemon whose data is printed.
	// @return the output stream to which the data is printed.
	friend std::ostream& operator<<(std::ostream& output,
			const Trainer& trainer) {
		std::string str;
		if (trainer.team == RED)
			str = "RED";
		if (trainer.team == BLUE)
			str = "BLUE";
		if (trainer.team == YELLOW)
			str = "YELLOW";
		output << trainer.name << " " << "(" << trainer.level << ")" << " "
				<< str << std::endl;

		for (int i = trainer.pokemons.size() - 1; i >= 0; i--) {
			Pokemon poke = trainer.pokemons[i];
			output << poke;
			//<< std::endl;
		}

		return output;
	}

// function for battle between two trainers
	Trainer& fight(Trainer& trainer);

// returns true if team of the first trainer is "stronger"
	bool StrongestTeam(Trainer& trainer);

//using an item from the trainer "store"
	void UseItem();
//returns the trainer level
	int getLevel();
//purchasing an Item for Trainer
	void copyItem(Item* ptr);
//adding 2 points to the team score as a winning fight result
	void addWiningScore();
//decreasing 1 point as a loosing fight result
	void decLoosingScore();
//return the location's name of a trainer. TODO ww - can we do it??
	const std::string& getLocationByName();
	void setLocationByName(const std::string new_loc);

	void setLeader(int i);
	int getLeader();
	std::string getName();

};

std::ostream& operator<<(std::ostream& output, const Trainer& trainer);

}  // namespace mtm
}  // namespace pokemongo

#endif  // TRAINER_H
