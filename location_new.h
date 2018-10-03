/*
 * location_new.h
 *
 *  Created on: 29 בינו׳ 2017
 *      Author: Dvir
 */

#ifndef LOCATION_NEW_H_
#define LOCATION_NEW_H_
#include "location.h"
#include <string>
#include "trainer.h"
#include <vector>
#include "item.h"

class Gym: public mtm::pokemongo::Location {
private:
	mtm::pokemongo::Team team;
	mtm::pokemongo::Trainer* leader;
public:
	Gym() :
		Location(),team(mtm::pokemongo::NEUTRAL), leader(NULL) {
	}

	void Arrive(mtm::pokemongo::Trainer& t) override {
		if (std::find(trainers_.begin(), trainers_.end(), &t) != trainers_.end())
			throw mtm::pokemongo::LocationTrainerAlreadyInLocationException();
		this->trainers_.push_back(&t);
		if (this->leader == NULL) {
			this->leader = &t;
			t.setLeader(1);
			this->team = t.GetTeam();
			return;
			//break;
		}
		if (this->leader->GetTeam() != t.GetTeam()) {
			this->leader->setLeader(0);
			mtm::pokemongo::Trainer* winner = &this->leader->fight(t);
			this->leader = winner;
			this->leader->setLeader(1);
			this->team = winner->GetTeam();

		}
	}
	void Leave(mtm::pokemongo::Trainer& t) override {
		std::vector<mtm::pokemongo::Trainer*>::iterator position = std::find(
				trainers_.begin(), trainers_.end(), &t);
		if (position == trainers_.end())
			throw mtm::pokemongo::LocationTrainerNotFoundException();
		if (this->leader == &t) { // & *
			t.setLeader(0);
			if (this->trainers_.size() == 1) {
				this->trainers_.pop_back();
				this->team = mtm::pokemongo::NEUTRAL; // maybe we should add the neutral color?? huh?!?!?
				this->leader = NULL;
				return;
			} else {
				mtm::pokemongo::Trainer* tr = &existInTheSameTeam(this->team); //is there anyone from my team?
				if (tr == NULL) {
					if (howManyColorsWeHave(this->team) == 1) {
						mtm::pokemongo::Team t1 = findingExtraColor(this->team,
								this->team);
						mtm::pokemongo::Trainer* tr = &existInTheSameTeam(t1);
						//this->trainers_.push_back(tr); //?????????????????????
						this->team = tr->GetTeam();
						this->leader = tr;
						tr->setLeader(1);
					} else {
						mtm::pokemongo::Team t1 = findingExtraColor(this->team,
								this->team);
						mtm::pokemongo::Team t2 = findingExtraColor(this->team,
								t1);
						mtm::pokemongo::Trainer* tr1 = &existInTheSameTeam(t1);
						mtm::pokemongo::Trainer* tr2 = &existInTheSameTeam(t2);
						mtm::pokemongo::Trainer* winner = &tr1->fight(*tr2);

						this->leader = winner;
						winner->setLeader(1);
						this->team = winner->GetTeam();

					}
				} else {
					//this->trainers_.push_back(tr); /???????????????????????????
					this->team = tr->GetTeam();
					this->leader = tr;
					tr->setLeader(1);
				}
			}
		}
		int i = 0, size = this->trainers_.size();
		for (; i < size; i++) {
			if ((*this->trainers_[i]).getName() == t.getName())
				break;
		}
		this->trainers_.erase(this->trainers_.begin() + i);
	}
	~Gym()=default;
private:
	mtm::pokemongo::Team findingExtraColor(mtm::pokemongo::Team cuur_team,
			mtm::pokemongo::Team one_team) {
		std::vector<mtm::pokemongo::Trainer>::iterator it;
		int size = trainers_.size();
		for (int i = 0; i < size; i++) {
			if (cuur_team == trainers_[i]->GetTeam()
					|| one_team == trainers_[i]->GetTeam())
				continue;
			else {
				return trainers_[i]->GetTeam();
			}
		}
		return cuur_team;
	}
	int howManyColorsWeHave(mtm::pokemongo::Team currTeam) {
		int j = 0;
		mtm::pokemongo::Team firstT;
		bool isF = true;
		std::vector<mtm::pokemongo::Trainer>::iterator it;
		int size = trainers_.size();
		for (int i = 0; i < size; i++) {
			if (trainers_[i]->GetTeam() != currTeam) {
				if (isF) {
					firstT = trainers_[i]->GetTeam();
					j++;
					isF = false;
				} else if (trainers_[i]->GetTeam() != firstT) {
					return 2;
				}
			}
		}
		return j;
	}
	mtm::pokemongo::Trainer& existInTheSameTeam(mtm::pokemongo::Team team) {
		std::vector<mtm::pokemongo::Trainer>::iterator it;
		mtm::pokemongo::Trainer* tr_ptr_max = NULL;
		int size = trainers_.size();
		for (int i = 0; i < size; i++) {
			if (team != trainers_[i]->GetTeam())
				continue;
			mtm::pokemongo::Trainer a = *trainers_[i];
			mtm::pokemongo::Trainer b = *this->leader;
			if (tr_ptr_max == NULL && a.getName()!=b.getName()) // TODO check these trainers by name!!!!
//			if (tr_ptr_max == NULL && *trainers_[i] != *this->leader)
				{
				tr_ptr_max = trainers_[i];
			}
			if (tr_ptr_max){
					if (*tr_ptr_max > *trainers_[i])
				tr_ptr_max = trainers_[i];
		}
				}
		return *tr_ptr_max;
	}
};
class Pokestop: public mtm::pokemongo::Location {
private:
	std::vector<mtm::pokemongo::Item*> items;
public:
	//the default C'tor
	Pokestop()=default;
	~Pokestop()=default;

	//insert an item to the items vector
	void insertItems(std::string& type, int level) {
		if (type == "POTION")
			items.push_back(new mtm::pokemongo::Potions(level));
		else
			items.push_back(new mtm::pokemongo::Candies(level));
	}
	void Arrive(mtm::pokemongo::Trainer& trainer) override {
		if (std::find(trainers_.begin(), trainers_.end(), &trainer) != trainers_.end())
					throw mtm::pokemongo::LocationTrainerAlreadyInLocationException();
				this->trainers_.push_back(&trainer);
		int trainer_lvl = trainer.getLevel();
		int size = items.size();
		for (int i = 0; i < size; i++) {
			if (trainer_lvl >= items[i]->getLevel()) {
				trainer.copyItem(items[i]);
				this->items.erase(this->items.begin() + i);
				return;
			}
		}
	}
	void Leave(mtm::pokemongo::Trainer& trainer) override {
		std::vector<mtm::pokemongo::Trainer*>::iterator position = std::find(
				trainers_.begin(), trainers_.end(), &trainer);
		int i = 0, size = this->trainers_.size();
		for (; i < size; i++) {
			if (*this->trainers_[i] == trainer)
				break;
		}
		if (size>0)
		this->trainers_.erase(this->trainers_.begin() + i);

	}
};
class Starbucks: public mtm::pokemongo::Location {
private:
	std::vector<mtm::pokemongo::Pokemon> pokemons;
public:
	//C'tor
	Starbucks()=default;
	~Starbucks()=default;

	void insertPokemons(std::string& species, double cp, int level) {
		mtm::pokemongo::Pokemon p(species, cp, level);
		pokemons.push_back(p);
	}
	void Arrive(mtm::pokemongo::Trainer& trainer) override {

		int size = pokemons.size();
		this->trainers_.push_back(&trainer);
		if (size==0)
			return;
			if (trainer.TryToCatch(pokemons[0])){
				this->pokemons.erase(
						std::remove(this->pokemons.begin(),
								this->pokemons.end(), pokemons[0]),
						this->pokemons.end());
				return;
			}


	}
	void Leave(mtm::pokemongo::Trainer& trainer) override {
		std::vector<mtm::pokemongo::Trainer*>::iterator position = std::find(
				trainers_.begin(), trainers_.end(), &trainer);
		int i = 0, size = this->trainers_.size();
		for (; i < size; i++) {
			if (*this->trainers_[i] == trainer)
				break;
		}
		this->trainers_.erase(this->trainers_.begin() + i);
	}

};

#endif /* LOCATION_NEW_H_ */
