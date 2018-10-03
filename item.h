/*
 * item.h
 *
 *  Created on: 29 בינו׳ 2017
 *      Author: Dvir
 */

#ifndef ITEM_H_
#define ITEM_H_

namespace mtm {
namespace pokemongo {

class Item {
	int level;
	char type;
public:
	Item():level(1),type('j'){};
	Item(int v,char t):level(v),type(t){}
	virtual int getLevel()=0;
	virtual char getType()=0;
	virtual ~Item() {
	}
	;

};

class Potions: public Item {
	int level;
	char type;
public:
	Potions(int v,char t='P') :
			level(v), type(t) {
	}
	int getLevel() override {
		return this->level;
	}
	char getType() override {
		return this->type;
	}

};
class Candies: public Item {
	int level;
	char type;
public:
	Candies(int v,char t='C') :
		level(v), type(t) {
	}

	int getLevel() override {
		return this->level;
	}
	char getType() override {
		return this->type;
	}
};
}
}
#endif /* ITEM_H_ */
