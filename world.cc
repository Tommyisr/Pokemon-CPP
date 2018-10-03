/*
 * world.cc
 *
 *  Created on: 28 בינו׳ 2017
 *      Author: Dvir
 */

#include "world.h"
#include "k_graph.h"
/*
 *
 * template<typename KeyType, typename ValueType, int k> class KGraph
 * 					^string				^Location*	^4
 */
mtm::pokemongo::World::World() :
		world(new mtm::KGraph<std::string, Location*, 4>(new Location())) {
}
mtm::pokemongo::World::~World() {
	delete world;

}
void mtm::pokemongo::World::Insert(std::string key, Location* loc) {
	try {
		this->world->Insert(key, loc);
	} catch (KGraphKeyAlreadyExistsExpection& a) {
		throw WorldLocationNameAlreadyUsed();
	}
}
std::vector<std::string>& mtm::pokemongo::World::getVectorLoc() {
	return this->Gym_loc_names;
}

const std::vector<std::string>& mtm::pokemongo::World::getVectorLoc() const {
	return this->Gym_loc_names;
}
// Removes the location with the given name from the graph.
//
// @param name is the name of the location to be removed.
// @throw WorldLocationNameCannotBeFound when trying to remove a key that cannot
//        be found in the graph.
void mtm::pokemongo::World::Remove(std::string const& name) {
//	TODO - check what happens in case of removing a location (thinking about moving the allTrainers map to World class)
	try {
		this->world->Remove(name);

	} catch (KGraphKeyNotFoundException& a) {
		throw WorldLocationNameCannotBeFound();
	}
}
// The subscript operator. Returns a reference to the location_ptr assigned to
// the given name in the world. If the name does not exist, inserts a new location
// to the graph with the given name and the default location, then returns a
// refernce to its location.
//
// @param name the name to return its location_ptr.
// @return the location_ptr assigned to the given name.
mtm::pokemongo::Location*& mtm::pokemongo::World::operator[](
		std::string const& name) {
	return (*this->world)[name];
}
// A const version of the subscript operator. Returns the location_ptr assigned to
// the given name in the world. If the name does not exist, throws an exception.
//
// @param name the name to return its location_ptr.
// @return the location_ptr assigned to the given name.
// @throw WorldLocationNameCannotBeFound if the given name cannot be found in the
//        world.
mtm::pokemongo::Location* const & mtm::pokemongo::World::operator[](
		std::string const& name) const {
//	try {
//		//return (*this->world)[name];
//		(*this->world)[name];		// final day
//	} catch (...) {
//		throw WorldLocationNameCannotBeFound();
//	}
	if (this->world->Contains(name))
		return (*this->world)[name];

	throw WorldLocationNameCannotBeFound();
}
// Checks whether the graph contains the given key.
//
// @param key
// @return true if the graph contains the given key.
bool mtm::pokemongo::World::Contains(std::string const& name) const {
	return this->world->Contains(name);
}
void mtm::pokemongo::World::Connect(std::string origin_name,
		std::string des_name, int origin_index, int des_index) {
	try {
		this->world->Connect(origin_name, des_name, origin_index, des_index);
	} catch (KGraphKeyNotFoundException& a1) {
		throw WorldLocationNameCannotBeFound();
	} catch (KGraphEdgeOutOfRange& a2) {
		throw WorldLocationIsOutOfRange();
	} catch (KGraphNodesAlreadyConnected& a3) {
		throw WorldLocationsAlreadyConnected();
	} catch (KGraphEdgeAlreadyInUse& a4) {
		throw WorldIsConnectedToItsLimit();
	}
}
// Connects a node to itself via a self loop.
//
// @param key the key of the node.
// @param i the index of the self loop.
// @throw WorldLocationNameCannotBeFound if the given keys cannot be found in the
//        graph.
// @throw WorldLocationIsOutOfRange if i is not in the range [0,k-1]
// @throw WorldLocationsAlreadyConnected if the node is already self connected.
// @throw WorldIsConnectedToItsLimit if the index of the self loop is already in
//        use.
void mtm::pokemongo::World::Connect(std::string const& name, int i) {
	try {
		this->world->Connect(name, i);
	} catch (KGraphKeyNotFoundException& a1) {
		throw WorldLocationNameCannotBeFound();
	} catch (KGraphEdgeOutOfRange& a2) {
		throw WorldLocationIsOutOfRange();
	} catch (KGraphNodesAlreadyConnected& a3) {
		throw WorldLocationsAlreadyConnected();
	} catch (KGraphEdgeAlreadyInUse& a4) {
		throw WorldIsConnectedToItsLimit();
	}
}
// Disconnects two connected loations.
//
// @param name_u the name of the first location.
// @param name_v the name of the second location.
// @throw KGraphKeyNotFoundException if at least one of the given keys cannot
//        be found in the graph.
// @throw kGraphNodesAreNotConnected if the two nodes are not connected.
void mtm::pokemongo::World::Disconnect(std::string const& name_u,
		std::string const& name_v) {
	try {
		this->world->Disconnect(name_u, name_v);
	} catch (KGraphKeyNotFoundException& a1) {
		throw WorldLocationNameCannotBeFound();
	} catch (kGraphNodesAreNotConnected& a2) {
		throw WorldLocationAreNotConnected();
	}
}

std::string mtm::pokemongo::World::getLocationNameByEdgeI(
		std::string loc_curr_name, Direction& dir) const {
	std::string str;
	KGraph<std::string, Location*, 4>::iterator it(
			this->world->BeginAt(loc_curr_name));
	try {
		it = it.Move(dir);
		if (it == this->world->End()) {
			throw WorldIsConnectedToItsLimit();

		}
	} catch (KGraphEdgeOutOfRange&) {
		throw WorldLocationIsOutOfRange();

	}
	//it = it.Move(dir);
	return *it;
}

void mtm::pokemongo::World::split(const std::string& str,
		std::vector<std::string>& line) {

	typedef std::string::size_type size_of_string;
	size_of_string i = 0;
	while (i != str.size()) {
		while (i != str.size() && isspace(str[i]))
			++i;
		size_of_string j = i;
		while (j != str.size() && !isspace(str[j]))
			j++;
		if (i != j) {
			line.push_back(str.substr(i, j - i));
			i = j;
		}
	}

}

int mtm::pokemongo::World::charToInt(std::string str) {
	if (str[0] == '-')
		throw WorldInvalidInputLineException(); // INVALID ARGUMENTS
	int len = str.length();
	int num = 0;
	int i = 0;
	int temp;
	while (i < len) {
		num = num * 10;
		temp = str[i] - '0';
		num += temp;
		i++;
	}
	return num;
}

double mtm::pokemongo::World::charToDouble(std::string str) {
	if (str[0] == '-')
		throw WorldInvalidInputLineException(); // INVALID ARGUMENTS
	int len = str.length();
	double first = 0;
	int i = 0;

	while (str[i] != '.' && i < len) {
		i++;
	}

	int lenpart = i;
	int temp;
	i = 0;
	while (i < lenpart) {
		first = first * 10;
		temp = str[i] - '0';
		first += temp;
		i++;
	}

	lenpart = len - lenpart - 1;
	double second = 0;
	i++;
	while (i < len) {
		second = second * 10;
		temp = str[i] - '0';
		second += temp;
		i++;
	}
	int multi = 1;
	for (int i = 0; i < lenpart; i++) {
		multi = multi * 10;
	}
	second = second / multi;
	double num = first + second;

	return num;
}

