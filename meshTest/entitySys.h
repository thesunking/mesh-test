#pragma once
#include "lookupTable.h"
#include "physicsCmp.h"
#include "renderCmp.h"
#include <bitset>

//LIST OF PLACES WHERE COMPONENTS MUST BE MANUALLY ADDED
//enum
//prefabContainer
//EntitySys (add lookup table)
//createEntity(ID)
//deleteEntity(ID)
//all specialized functions

enum componentEnum
{
	PHYSICS_CMP,
	RENDER_CMP,
	NUMBER_OF_CMPS
};

class CmpIDContainer
{
public:
	ID id;
	ID cmpIDs[NUMBER_OF_CMPS];
};

class PrefabContainer
{
public:
	ID id;

	std::bitset<NUMBER_OF_CMPS> cmpList;

	//NOTE: make sure to add components here
	PhysicsCmp phyCmp;
	RenderCmp rndCmp;
};

class EntitySys
{
private:
	ID_LOOKUP_TABLE<PrefabContainer> prefabs;

	ID_LOOKUP_TABLE<CmpIDContainer> aggregateComponents;
	ID_LOOKUP_TABLE<PhysicsCmp> physicsComponents;
	ID_LOOKUP_TABLE<RenderCmp> renderComponents;

public:
	ID createEntity();
	ID createEntity(ID prefab_id);
	void deleteEntity(ID id);

	ID createPrefab();
	//these 2 functions must be specialized for each component
	template <class T>
	void addPrefabCmp(ID prefab_id, T& cmp);
	template <class T>
	void removePrefabCmp(ID prefab_id);

	//these 4 functions must be specialized for each component
	template <class T>
	ID_LOOKUP_TABLE<T>& getTable();
	template <class T>
	ID getExposedID(ID internalID);
	template <class T>
	ID getInternalID(ID exposedID);
	template <class T>
	void setInternalID(ID exposedID, ID internalID);

	//defined in header file (no specialization needed)
	template <class T>
	bool has(ID exposedID);
	template <class T>
	T& get(ID exposedID);
	template <class T>
	T* getAll();
	template <class T>
	unsigned int getSize();
	template <class T>
	ID add(ID exposedID, T& cmp);
	template <class T>
	void remove(ID exposedID);
};


template <class T>
bool EntitySys::has(ID exposedID)
{
	ID id = getInternalID<T>(exposedID);
	return getTable<T>().has(id);
}

template <class T>
T& EntitySys::get(ID exposedID)
{
	ID id = getInternalID<T>(exposedID);
	return getTable<T>().get(id);
}

template <class T>
T* EntitySys::getAll()
{
	return getTable<T>().components;
}

template <class T>
unsigned int EntitySys::getSize()
{
	return getTable<T>().num_objects;
}

template <class T>
ID EntitySys::add(ID exposedID, T& cmp)
{
	ID internalID = getTable<T>().add(cmp);
	setInternalID<T>(exposedID, internalID);
	return internalID;
}

template <class T>
void EntitySys::remove(ID exposedID)
{
	ID id = getInternalID<T>(exposedID);
	getTable<T>().remove(id);

	//reset the cmpID in aggregateComponents
	id.index = UINT_MAX ;
	id.verif = UINT_MAX ;
	setInternalID<T>(exposedID, id);
}