/*
 * pokemon_go.cc
 *
 *  Created on: 29 בינו׳ 2017
 *      Author: Dvir
 */

#include "pokemon_go.h"
using namespace mtm::pokemongo;

PokemonGo::PokemonGo(const World* w) :
		world(w), allTrainers(std::map<std::string, Trainer*>()) {
	Trainer::ScoreBlueTeam = 0;
	Trainer::ScoreRedTeam = 0;
	Trainer::ScoreYellowTeam = 0;
}

void PokemonGo::AddTrainer(const std::string& name, const Team& team,
		const std::string& location) {
	if (name == "")
		throw PokemonGoInvalidArgsException();
	if (this->allTrainers.find(name) != this->allTrainers.end())
		throw PokemonGoTrainerNameAlreadyUsedExcpetion();
	if (!this->world->Contains(location))
		throw PokemonGoLocationNotFoundException();

	Trainer* newTrainer = new Trainer(name, team, location);
	this->allTrainers[name] = newTrainer;
	this->world->operator [](location)->Arrive(*newTrainer);
}
void PokemonGo::MoveTrainer(std::string trainer_name, int dir) {
	if (this->allTrainers.find(trainer_name) == this->allTrainers.end())
		throw PokemonGoTrainerNotFoundExcpetion();
	Trainer* trainer = this->allTrainers.find(trainer_name)->second;
	std::string curr_loc_name = trainer->getLocationByName();
	std::string des_loc_name;
	try {
		des_loc_name = this->world->getLocationNameByEdgeI(curr_loc_name, dir);
	} catch (WorldLocationIsOutOfRange& a) {
		throw PokemonGoReachedDeadEndException();
	} catch (WorldIsConnectedToItsLimit& b) {
		throw PokemonGoReachedDeadEndException();
	}

	Location* curr_loc;
	Location* des_loc;
	try {
		curr_loc = (*this->world)[curr_loc_name];
		des_loc = (*this->world)[des_loc_name];
	} catch (WorldLocationNameCannotBeFound& a) {
		throw PokemonGoLocationNotFoundException();
	}

	curr_loc->Leave(*trainer);
	des_loc->Arrive(*trainer);
	trainer->setLocationByName(des_loc_name);
}
std::string PokemonGo::WhereIs(const std::string& trainer_name) {
	if (this->allTrainers.find(trainer_name) == this->allTrainers.end())
		throw PokemonGoTrainerNotFoundExcpetion();
	return this->allTrainers.at(trainer_name)->getLocationByName();
}
//using GetTrainersFunc in location.h
const std::vector<Trainer*>& PokemonGo::GetTrainersIn(
		const std::string& location) {
	Location* loc;
	try {
		loc = (*this->world)[location]; // final day

	} catch (WorldLocationNameCannotBeFound& a) {
		throw PokemonGoLocationNotFoundException();
	}
	return loc->GetTrainers();
}

// Returns the score of a given team in the game.
int PokemonGo::GetScore(const Team& team) {
	//check the leaders inside all the locations if they are team kind
	//add that to the general teem score (from wining/losing battels) - a val in world
	int score = 0;

	std::vector<string> asd = this->world->getVectorLoc();

	int size = this->world->getVectorLoc().size();
	Trainer* key_tariner = NULL;
	std::vector<Trainer*> vectrain;
	for (int i = 0; i < size; i++) {
		std::string name = this->world->getVectorLoc()[i];
		Location* loc = this->world->operator [](name);
		std::vector<Trainer*> vectrain = loc->GetTrainers();
		int trainsize = vectrain.size();
		for (int j = 0; j < trainsize; j++) {
			if (vectrain[j]->GetTeam() == team) {
				score += vectrain[j]->getLevel();
				key_tariner = vectrain[j];
				if (vectrain[j]->getLeader() == 1)
					score += 10;
			}
		}
	}
	if (key_tariner) {
		if (key_tariner->GetTeam() == mtm::pokemongo::BLUE)
			return score + key_tariner->ScoreBlueTeam;
		//	return score + this->BlueTeam;
		else if (key_tariner->GetTeam() == mtm::pokemongo::RED)
			return score + key_tariner->ScoreRedTeam;
		//return score + this->RedTeam;
		else if (key_tariner->GetTeam() == mtm::pokemongo::YELLOW)
			return score + key_tariner->ScoreYellowTeam;
		//return score + this->YellowTeam;
	}
	return score;
}
