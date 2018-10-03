#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include "k_graph.h"
//#include "location.h"
#include <vector>
#include <string>
#include "location_new.h"
namespace mtm {
namespace pokemongo {

typedef int Direction;

static const int NORTH = 0;
static const int SOUTH = 1;
static const int EAST = 2;
static const int WEST = 3;

class World {
	mtm::KGraph<std::string, Location*, 4>* world;
	std::vector<std::string> Gym_loc_names;
public:
	// Constructs a new empty world.
	World();

	// A destructor.
	~World();
	//insert a new location to the world with its name as a string
	// @param key the name to be assigned to the new location.
	// @param value the value to be assigned to the new location.
	// @throw WorldLocationNameAlreadyUsed when trying to insert a location name with a
	//        location name that already exists in the graph.
	void Insert(std::string key, Location* loc);
	// Removes the location with the given name from the graph.
	//
	// @param name is the name of the location to be removed.
	// @throw KGraphKeyNotFoundException when trying to remove a key that cannot
	//        be found in the graph.
	void Remove(std::string const& name);
	// The subscript operator. Returns a reference to the location_ptr assigned to
	// the given name in the world. If the name does not exist, inserts a new location
	// to the graph with the given name and the default location, then returns a
	// refernce to its location.
	//
	// @param name the name to return its location_ptr.
	// @return the location_ptr assigned to the given name.
	Location*& operator[](std::string const& name);
	// A const version of the subscript operator. Returns the location_ptr assigned to
	// the given name in the world. If the name does not exist, throws an exception.
	//
	// @param name the name to return its location_ptr.
	// @return the location_ptr assigned to the given name.
	// @throw WorldLocationNameCannotBeFound if the given name cannot be found in the
	//        world.
	Location* const & operator[](std::string const& name) const;
	//connects to locations in the world graph
	// @param origin_name the name of the first location.
	// @param des_name the key of the second location.
	// @param origin_index the index of the new edge at the first location.
	// @param des_index the index of the new edge at the second location.
	void Connect(std::string origin_name, std::string des_name,
			int origin_index, int des_index);
	// Checks whether the graph contains the given key.
	//
	// @param key
	// @return true if the graph contains the given key.
	bool Contains(std::string const& name) const;
	// Connects a node to itself via a self loop.
	//
	// @param key the key of the node.
	// @param i the index of the self loop.
	// @throw KGraphKeyNotFoundException if the given keys cannot be found in the
	//        graph.
	// @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
	// @throw KGraphNodesAlreadyConnected if the node is already self connected.
	// @throw KGraphEdgeAlreadyInUse if the index of the self loop is already in
	//        use.
	void Connect(std::string const& name, int i);
	// Disconnects two connected loations.
	//
	// @param name_u the name of the first location.
	// @param name_v the name of the second location.
	// @throw KGraphKeyNotFoundException if at least one of the given keys cannot
	//        be found in the graph.
	// @throw kGraphNodesAreNotConnected if the two nodes are not connected.

	void Disconnect(std::string const& name_u, std::string const& name_v);

private:
	void split(const std::string& str, std::vector<std::string>& line);

	int charToInt(std::string str);
	double charToDouble(std::string str);

public:
	// Input iterator. Scans a single line from the input stream. The line can be
	// one of the following three options:
	//
	// (1) "GYM <gym_name>"
	//     e.g. "GYM taub"
	//     Adds a gym with the given name to the world.
	// (2) "POKESTOP <pokestop_name> <item_type1> <item_level1> <item_type2> <item_level2> ..."
	//     e.g. "POKESTOP mikhlol POTION 10 CANDY 20 CANDY 13"
	//     Creates a Pokestop with the given name that contains the given items by
	//     order, and adds it to the world.
	// (3) "STARBUCKS <starbucks_name> <pokemon_species1> <cp1> <level1> <pokemon_species2> <cp2> <level2> ..."
	//     e.g. "STARBUCKS shani pikachu 2.5 3 pikachu 1 2 charmander 3.45 4"
	//     Creates a starbucks with the given name that contains Pokemons with the
	//     given parameters by order, and adds it to the world. The Pokemons will
	//     have the default types.
	// You can assume that none of the pieces of information (location name,
	// Pokemon species, etc.) contains a space.
	//
	// @param input the input stream.
	// @param world the world to which to add the locations.
	// @return the input stream.
	// @throw WorldInvalidInputLineException if the input line is not one of the
	//        three options, or one of the parameters is invalid (for example,
	//        negative CP value, etc.).
	// @throw WorldLocationNameAlreadyUsed if there already exists a location with
	//        the given name in the world.
	friend std::istream& operator>>(std::istream& input, World& world) {

		std::vector<std::string> line;
		std::string str;
		std::getline(input, str);
		world.split(str, line);
		int len = line.size();
		if (line[0] == "GYM") {
			if (world.world->Contains(line[1]))
				throw WorldLocationNameAlreadyUsed();

			world.Insert(line[1], new Gym()); // need to put Gym here
			world.getVectorLoc().push_back(line[1]);
		} else if (line[0] == "POKESTOP") {
			if (world.world->Contains(line[1]))
				throw WorldLocationNameAlreadyUsed();

			Pokestop* pokestop = new Pokestop();
			for (int i = 2; i < len; i++) {
				int j = i;
				i++;
				try {
					world.charToInt(line[i]);
				} catch (WorldInvalidInputLineException& a) {
					throw WorldInvalidInputLineException();
				}
				pokestop->insertItems(line[j], world.charToInt(line[i]));

			}
			world.Insert(line[1], pokestop);
			world.getVectorLoc().push_back(line[1]); // by Toommy checking
		} else if (line[0] == "STARBUCKS") {
			if (world.world->Contains(line[1]))
				throw WorldLocationNameAlreadyUsed();

			Starbucks* starbucks = new Starbucks();
			for (int i = 2; i < len; i++) {
				int name = i++; // ++???
				int cp = i++;
				int level = i;
				try {
					world.charToInt(line[i]);
				} catch (WorldInvalidInputLineException& a) {
					throw WorldInvalidInputLineException();
				}
				starbucks->insertPokemons(line[name],
						world.charToDouble(line[cp]),
						world.charToInt(line[level]));
			}
			world.Insert(line[1], starbucks);
			world.getVectorLoc().push_back(line[1]); // by Toommy checking
		} else
			throw WorldInvalidInputLineException();
		return input;
	}

	// Disable copy constructor.
	World(const World& world) = delete;

	std::string getLocationNameByEdgeI(std::string loc_curr_name,
			Direction& dir) const;

	std::vector<std::string>& getVectorLoc();
	const std::vector<std::string>& getVectorLoc() const;

};

std::istream& operator>>(std::istream& input, World& world);

}
// namespace pokemongo
}// namespace mtm

#endif  // WORLD_H
