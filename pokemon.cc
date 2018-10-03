/*
 * pokemon.cc
 *
 *  Created on: 18 בינו׳ 2017
 *      Author: Dvir
 */

#include "pokemon.h"

using namespace mtm::pokemongo;

/* Constructs a new Pokemon with the specified data.
 *
 * @param species the species of the Pokemon.
 * @param types the types of the Pokemon.
 * @param cp the CP value of the Pokemon.
 * @param level the level of the Pokemon.
 * @throw PokemonInvalidArgsException if a non-positive level or CP value were
 * passed, or if species is an empty string.
 */
Pokemon::Pokemon(const std::string& species, const std::set<PokemonType>& types,
		const double& cp, const int& level) :
		species(species), hp(100), level(level), cp(cp), types(types) {
	if (cp <= 0 || level <= 0 || species == "")
		throw PokemonInvalidArgsException();
}
/*Constructs a new Pokemon with the specified data and the default types for
 * its species.
 *
 * @param species the species of the Pokemon.
 * @param cp the CP value of the Pokemon.
 * @param level the level of the Pokemon.
 * @throw PokemonInvalidArgsException if a non-positive level or CP value were
 * passed, or if species is an empty string.
 */
Pokemon::Pokemon(const std::string& species, const double& cp, const int& level) :
		species(species), hp(100), level(level), cp(cp) {
	if (cp <= 0 || level <= 0 || species == "")
		throw PokemonInvalidArgsException();
	this->types = GetDefaultTypes(species);
}
/*
 * Copy constructor.
 *
 * @param pokemon the pokemon to copy.
 */

Pokemon::Pokemon(const Pokemon& pokemon) :
		species(pokemon.species), hp(pokemon.hp), level(pokemon.level), cp(
				pokemon.cp), types(pokemon.types) {
}
/*
 * Copy constructor.
 * @param pokemon the pokemon to copy.
 * Pokemon(const Pokemon& pokemon);
 * Assignment operator.
 * @param pokemon assignee.
 */
Pokemon& Pokemon::operator=(const Pokemon& pokemon) {
	this->cp = pokemon.cp;
	this->hp = pokemon.hp;
	this->level = pokemon.level;
	this->types = pokemon.types;
	this->species = pokemon.species;

	return *this;
}

/*
 * Comparison operators for Pokemons. Pokemons are compared as described in
 * the exercise sheet.
 * @param rhs the right-hand side operand.
 */
bool Pokemon::operator==(const Pokemon& rhs) const {
	if (this->getHitPower() == rhs.getHitPower())
		return true;
	return false;
}
bool Pokemon::operator!=(const Pokemon& rhs) const {
	return !(*this == rhs);
}
bool Pokemon::operator<(const Pokemon& rhs) const {
	if (this->getHitPower() < rhs.getHitPower())
		return true;
	return false;
}
bool Pokemon::operator>(const Pokemon& rhs) const {
	if (this->getHitPower() > rhs.getHitPower())
		return true;
	return false;
}
bool Pokemon::operator<=(const Pokemon& rhs) const {
	return (*this < (rhs) || *this == rhs);
}
bool Pokemon::operator>=(const Pokemon& rhs) const {
	return (*this > (rhs) || *this == (rhs));
}
/*
 * Returns the level of the Pokemon.
 * @return the level of the Pokemon.
 */
int Pokemon::Level() const {
	return this->level;
}
/*
 * "Hits" the given Pokemon by reducing its HP value by the hit power of this
 * Pokemon.
 * @param the Pokemon to hit.
 * @return true iff the hitten Pokemon died (HP reached zero).
 */
bool Pokemon::Hit(Pokemon& victim) {
	double new_hp = victim.hp - (this->getHitPower());
	if (new_hp < 0)
		new_hp = 0;
	victim.setHP(new_hp);
	return victim.hp <= 0;
}
/*
 * Heals the Pokemon by increasing its HP to 100.
 */
void Pokemon::Heal() {
	this->hp = 100;
}
/*
 * Trains the Pokemon by multiplying its CP value by the given boost.
 * @param boost the boost by which the CP value is multiplied.
 * @throw PokemonInvalidArgsException if boost is not bigger than 1.
 */
void Pokemon::Train(const double& boost) {
	if (boost < 2)
		throw PokemonInvalidArgsException();
	this->cp = this->cp * boost;
}
/*
 * this function returns the HitPower of the pokemon it is operated on
 * HitPower = cp*level
 */
double Pokemon::getHitPower() const {
	return ((this->cp) * (this->level));
}
/*
 * this function gets a new hp and changes the current hp to the new one
 */
void Pokemon::setHP(const double newHP) {
	this->hp = newHP < 0 ? 0 : newHP;
}

std::string Pokemon::createAStringOutOfTypes() const {
	std::string types_str;
	std::set<PokemonType>::iterator it;
	for (it = this->types.begin(); it != this->types.end(); ++it) {
		types_str += " ";
		types_str += Pokemon::getPokemonTypeString(*it);
	}
	return types_str;
}
std::string Pokemon::getPokemonTypeString(int i) const {
	std::string one_type;
	switch (i) {
	case 0:
		one_type = "NORMAL";
		break;
	case 1:
		one_type = "ROCK";
		break;
	case 2:
		one_type = "BUG";
		break;
	case 3:
		one_type = "FAIRY";
		break;
	case 4:
		one_type = "GROUND";
		break;
	case 5:
		one_type = "GRASS";
		break;
	case 6:
		one_type = "WATER";
		break;
	case 7:
		one_type = "ICE";
		break;
	case 8:
		one_type = "GHOST";
		break;
	case 9:
		one_type = "POISON";
		break;
	case 10:
		one_type = "ELECTRIC";
		break;
	case 11:
		one_type = "FIRE";
		break;
	case 12:
		one_type = "FLYING";
		break;
	case 13:
		one_type = "PSYCHIC";
		break;
	}
	return one_type;
}

