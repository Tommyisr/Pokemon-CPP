#include "trainer.h"
#include <iostream>
#include <sstream>
#include "pokemon_go.h"
using namespace mtm::pokemongo;
int dick = 0;
int Trainer::ScoreRedTeam = 0;
int Trainer::ScoreBlueTeam = 0;
int Trainer::ScoreYellowTeam = 0;

// Constructs a new trainer with the given name and team.
Trainer::Trainer(const std::string& name, const Team& team,
		const std::string& location_name) :
		name(name), team(team), level(1), pokemons(std::vector<Pokemon>()), location_name(
				location_name), items(std::vector<Item*>()), isLeader(0) {
	if (name == "")
		throw TrainerInvalidArgsException();
}
Trainer::Trainer() :
		name(""), team(NEUTRAL), level(1), pokemons(std::vector<Pokemon>()), location_name(
				""), items(std::vector<Item*>()), isLeader(0) {
}
// Returns a reference to the strongest Pokemon the trainer owns. Strongest
// Pokemon is determined using the comparison operators provided by the class
// Pokemon. If two Pokemons are of equal strength, the function returns the
// one that was caught earlier by the trainer.
//
Pokemon& Trainer::GetStrongestPokemon() {
	if (this->pokemons.size() == 0) {
		throw TrainerNoPokemonsFoundException();
	}

	std::vector<Pokemon>::iterator it;
	it = this->pokemons.begin();
	Pokemon strongestPoke = this->pokemons[0];

	int max = 0;
	int i = 0;
	for (it = this->pokemons.begin(); it != this->pokemons.end(); ++it) { // POKE1 > POKE2

		if (strongestPoke < this->pokemons[i]) // newFunction of GETCP....what if there is no Pokemon at (j)???
				{
			strongestPoke = this->pokemons[i];
			max = i;
		}
		i++;
	}
	Pokemon& pokmax = this->pokemons[max];
	return pokmax;
}

// Returns a reference to the strongest Pokemon the trainer owns. Strongest
// Pokemon is determined using the comparison operators provided by the class
// Pokemon. If two Pokemons are of equal strength, the function returns the
// one that was caught earlier by the trainer.
//
const Pokemon& Trainer::GetStrongestPokemon() const {
	if (this->pokemons.size() == 0) {
		throw TrainerNoPokemonsFoundException();
	}

	std::vector<Pokemon>::const_iterator it;
	it = this->pokemons.begin();
	Pokemon strongestPoke = this->pokemons[0];

	int max = 0;
	int i = 0;
	for (it = this->pokemons.begin(); it != this->pokemons.end(); ++it) { // POKE1 > POKE2

		if (strongestPoke < this->pokemons[i]) // newFunction of GETCP....what if there is no Pokemon at (j)???
				{
			strongestPoke = this->pokemons[i];
			max = i;
		}
		i++;
	}
	const Pokemon& pokmax = this->pokemons[max];
	return pokmax;

}

// Kills the strongest Pokemon. Removes the Pokemon that is returned from
// GetStrongestPokemon() from the collection of Pokemons owned by the trainer.
//
void Trainer::KillStrongestPokemon() {
	if (this->pokemons.empty()) {
		throw TrainerNoPokemonsFoundException();
	}

	std::vector<Pokemon>::iterator it;
	Pokemon& strongestPoke = this->GetStrongestPokemon();
	int i = 0;
	for (it = this->pokemons.begin(); it != this->pokemons.end(); ++it) {
		if (strongestPoke == this->pokemons[i]) {
			this->pokemons.erase(it);
			break;
		}
		i++;
	}

}
// Comparison operators for trainers.

bool Trainer::operator==(const Trainer& rhs) const {
	if (this->pokemons.size() == 0 && rhs.pokemons.size() == 0) {
		return true;
	}

	if ((this->pokemons.size() == 0 && rhs.pokemons.size() != 0)
			|| (this->pokemons.size() != 0 && rhs.pokemons.size() == 0))
		return false;

	if (this->GetStrongestPokemon() == rhs.GetStrongestPokemon()) {
		return true;
	}

	return false;
}

bool Trainer::operator!=(const Trainer& rhs) const {
	if (*this == rhs)
		return false;

	return true;
}

bool Trainer::operator<(const Trainer& rhs) const {
	if (this->pokemons.size() == 0 && !rhs.pokemons.size() == 0) {
		return true;
	}

	if (!(this->pokemons.empty()) && !(rhs.pokemons.empty())) {
		if (this->GetStrongestPokemon() < rhs.GetStrongestPokemon()) {
			return true;
		}
	}
	return false;
}

bool Trainer::operator>(const Trainer& rhs) const {
	if (*this < rhs)
		return false;

	return true;
}

bool Trainer::operator<=(const Trainer& rhs) const {
	if (*this < rhs || *this == rhs) {
		return true;
	}
	return false;

}

bool Trainer::operator>=(const Trainer& rhs) const {
	if (*this <= rhs)
		return false;

	return true;

}

// Checks whether a given trainer is in the same team as this trainer.
//
bool Trainer::IsAlly(const Trainer& trainer) const {
	if (this->team == trainer.team) {
		return true;
	}
	return false;

}

// Returns the team to which the trainer belongs.
Team Trainer::GetTeam() const {
	return this->team;
}

// Tries to catch a Pokemon.
bool Trainer::TryToCatch(Pokemon& pokemon) {
	if (pokemon.Level() <= this->level) {
		//this->numPokemons++;
		this->pokemons.push_back(pokemon);
		return true;
	}
	return false;
}

// function for battle between two trainers
Trainer& Trainer::fight(Trainer& trainer) {
	if (this->pokemons.size() == 0 && trainer.pokemons.size() == 0) {
		if (this->StrongestTeam(trainer)) {
			this->addWiningScore();
			trainer.decLoosingScore();
			return *this;
		} else {
			this->decLoosingScore();
			trainer.addWiningScore();
			return trainer;
		}
	}
	int first = this->level / 2;
	int second = trainer.level / 2;
	if ((double) this->level / 2 > this->level / 2)
		first++;
	if ((double) trainer.level / 2 > trainer.level / 2)
		second++;
	if (this->pokemons.size() == 0) {
		trainer.level = trainer.level + first;
		this->decLoosingScore();
		trainer.addWiningScore();
		return trainer;
	}
	if (trainer.pokemons.size() == 0) {
		this->level = this->level + second;
		this->addWiningScore();
		trainer.decLoosingScore();
		return *this;
	}
	Pokemon& pok1 = this->GetStrongestPokemon();
	Pokemon& pok2 = trainer.GetStrongestPokemon();
	this->UseItem();
	trainer.UseItem();
	bool a = pok1.Hit(pok2);
	bool b = pok2.Hit(pok1);
	if (a && b) {
		this->KillStrongestPokemon();
		trainer.KillStrongestPokemon();
		if (this->StrongestTeam(trainer)) {
			this->addWiningScore();
			trainer.decLoosingScore();
			return *this;
		} else {
			this->decLoosingScore();
			trainer.addWiningScore();
			return trainer;
		}
	} else if (a && !b) {
		this->KillStrongestPokemon();
		trainer.level = trainer.level + first;
		this->decLoosingScore();
		trainer.addWiningScore();
		return trainer;
	} else if (!a && b) {
		trainer.KillStrongestPokemon();
		this->level = this->level + second;
		this->addWiningScore();
		trainer.decLoosingScore();
		return *this;
	} else if (pok1 > pok2) {
		this->level = this->level + second;
		this->addWiningScore();
		trainer.decLoosingScore();
		return *this;
	} else if (pok2 > pok1) {
		trainer.level = trainer.level + first;
		this->decLoosingScore();
		trainer.addWiningScore();
		return trainer;
	}
	if (this->StrongestTeam(trainer)) {
		this->addWiningScore();
		trainer.decLoosingScore();
		return *this;
	} else {
		this->decLoosingScore();
		trainer.addWiningScore();
		return trainer;
	}
}

void Trainer::UseItem() {
	Pokemon& pok = this->GetStrongestPokemon();
	if (this->items.size() == 0)
		return;

	Item* item(this->items[0]);

	if (item->getType() == 'P') {
		pok.Heal();
		items.erase(items.begin());
	}

	if (item->getType() == 'C') {
		pok.Train(1 + pok.Level() / 10);
		items.erase(items.begin());
	}

}

bool Trainer::StrongestTeam(Trainer& trainer) {
	if ((this->GetTeam() == BLUE && trainer.GetTeam() == RED)
			|| (this->GetTeam() == RED && trainer.GetTeam() == YELLOW)
			|| (this->GetTeam() == YELLOW && trainer.GetTeam() == BLUE))
		return true;

	return false;
}
int Trainer::getLevel() {
	return this->level;
}

void Trainer::copyItem(Item* ptr) {
	if (ptr->getType() == 'C')
		this->items.push_back(new Candies(ptr->getLevel()));
	else if (ptr->getType() == 'P')
		this->items.push_back(new Potions(ptr->getLevel()));
}
void Trainer::addWiningScore() {
	if (this->team == BLUE)
		ScoreBlueTeam += 2;
	else if (this->team == YELLOW)
		ScoreYellowTeam += 2;
	else if (this->team == RED)
		ScoreRedTeam += 2;

}
void Trainer::decLoosingScore() {
	if (this->team == BLUE)
		ScoreBlueTeam--;
	else if (this->team == YELLOW)
		ScoreYellowTeam--;
	else if (this->team == RED)
		ScoreRedTeam--;
}
const std::string& Trainer::getLocationByName() {
	return this->location_name;
}
void Trainer::setLocationByName(const std::string new_loc) {
	this->location_name = new_loc;
}

void Trainer::setLeader(int i) {
	this->isLeader = i;
}
int Trainer::getLeader() {
	return this->isLeader;
}

std::string Trainer::getName() {
	return this->name;
}

