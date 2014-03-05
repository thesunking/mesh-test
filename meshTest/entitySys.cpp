#include "entitySys.h"

ID EntitySys::createEntity()
{
	//create CmpIDContainer
	//set exposedID to first available
	//set cmpIDs to UINT_MAX

	CmpIDContainer cmpColl;

	for (int i = 0; i != NUMBER_OF_CMPS; ++i)
	{
		cmpColl.cmpIDs[i].index = UINT_MAX ;
		cmpColl.cmpIDs[i].verif = 0;
	}

	return aggregateComponents.add(cmpColl);
}

ID EntitySys::createEntity(ID prefab_id)
{
	ID ent_ID = createEntity();
	CmpIDContainer& entity = aggregateComponents.get(ent_ID);
	PrefabContainer& prefab = prefabs.get(prefab_id);

	//NOTE: Must add each component here
	if (prefab.cmpList[PHYSICS_CMP])
	{
		entity.cmpIDs[PHYSICS_CMP] = physicsComponents.add(prefab.phyCmp);
	}
	if (prefab.cmpList[RENDER_CMP])
	{
		entity.cmpIDs[RENDER_CMP] = renderComponents.add(prefab.rndCmp);
	}

	return ent_ID;
}

void EntitySys::deleteEntity(ID id)
{
	if (id.index == UINT_MAX )
	{
		return;
	} //exit if passed a null id

	//NOTE: Must add each component here
	remove<PhysicsCmp>(id);
	remove<RenderCmp>(id);
}

ID EntitySys::createPrefab()
{
	return prefabs.add(PrefabContainer());
}


template <>
void EntitySys::addPrefabCmp<PhysicsCmp>(ID prefab_id, PhysicsCmp& cmp)
{
	PrefabContainer& prefab = prefabs.get(prefab_id);
	prefab.phyCmp = cmp;
	prefab.cmpList.set(PHYSICS_CMP);
}

template <>
void EntitySys::addPrefabCmp<RenderCmp>(ID prefab_id, RenderCmp& cmp)
{
	PrefabContainer& prefab = prefabs.get(prefab_id);
	prefab.rndCmp = cmp;
	prefab.cmpList.set(RENDER_CMP);
}


template <>
void EntitySys::removePrefabCmp<PhysicsCmp>(ID prefab_id)
{
	PrefabContainer& prefab = prefabs.get(prefab_id);
	prefab.cmpList.reset(PHYSICS_CMP);
}

template <>
void EntitySys::removePrefabCmp<RenderCmp>(ID prefab_id)
{
	PrefabContainer& prefab = prefabs.get(prefab_id);
	prefab.cmpList.reset(RENDER_CMP);
}


template <>
ID_LOOKUP_TABLE<CmpIDContainer>& EntitySys::getTable()
{
	return aggregateComponents;
}

template <>
ID_LOOKUP_TABLE<PhysicsCmp>& EntitySys::getTable()
{
	return physicsComponents;
}

template <>
ID_LOOKUP_TABLE<RenderCmp>& EntitySys::getTable()
{
	return renderComponents;
}


template <>
ID EntitySys::getExposedID<PhysicsCmp>(ID internalID)
{
	CmpIDContainer* cmps = getAll<CmpIDContainer>();
	unsigned int size = getSize<CmpIDContainer>();

	for (unsigned int i = 0; i != size; ++i)
	{
		ID cmpID = cmps[i].cmpIDs[PHYSICS_CMP];
		if ( cmpID.index == internalID.index && cmpID.verif == internalID.verif )
		{
			return cmps[i].id;
		}
	}

	//if we get to here, the internal ID was not found -> return a null ID
	ID id;
	id.index = UINT_MAX ;
	id.verif = UINT_MAX ;
	return id;
}

template <>
ID EntitySys::getExposedID<RenderCmp>(ID internalID)
{
	CmpIDContainer* cmps = getAll<CmpIDContainer>();
	unsigned int size = getSize<CmpIDContainer>();

	for (unsigned int i = 0; i != size; ++i)
	{
		ID cmpID = cmps[i].cmpIDs[RENDER_CMP];
		if ( cmpID.index == internalID.index && cmpID.verif == internalID.verif )
		{
			return cmps[i].id;
		}
	}

	//if we get to here, the internal ID was not found -> return a null ID
	ID id;
	id.index = UINT_MAX ;
	id.verif = UINT_MAX ;
	return id;
}


template <>
ID EntitySys::getInternalID<PhysicsCmp>(ID exposedID)
{
	return aggregateComponents.get(exposedID).cmpIDs[PHYSICS_CMP];
}

template <>
ID EntitySys::getInternalID<RenderCmp>(ID exposedID)
{
	return aggregateComponents.get(exposedID).cmpIDs[RENDER_CMP];
}


template <>
void EntitySys::setInternalID<PhysicsCmp>(ID exposedID, ID internalID)
{
	aggregateComponents.get(exposedID).cmpIDs[PHYSICS_CMP] = internalID;
}

template <>
void EntitySys::setInternalID<RenderCmp>(ID exposedID, ID internalID)
{
	aggregateComponents.get(exposedID).cmpIDs[RENDER_CMP] = internalID;
}