#pragma once
#include "lookupID.h"
#include <array>

//NOTES:
//
//Object must store a copy of its ID

template <class T>
class ID_LOOKUP_TABLE {
	
public:

	ID_LOOKUP_TABLE();
	
	static const unsigned int MAX_NO_OBJECTS = 64;
	unsigned int next_index;
	unsigned int num_objects;
	
	INDEX indices[MAX_NO_OBJECTS];
	T  components[MAX_NO_OBJECTS];
	
	bool has(ID id);
	T&   get(ID id);
	ID   add(T& cmp);
	void remove(ID id);
	
};


//template implementation

template <class T>
ID_LOOKUP_TABLE<T>::ID_LOOKUP_TABLE() : next_index(0), num_objects(0) {
	
	for (unsigned int i = 0; i != MAX_NO_OBJECTS; ++i) {
		indices[i].id.index = i;
		indices[i].id.verif = 0;
		indices[i].next= i+1;
	}
	
}
	
template <class T>
bool ID_LOOKUP_TABLE<T>::has(ID id) {

	if (id.index == UINT_MAX) {
		return false;
	} else {
		return ((indices[id.index]).id.verif == id.verif);
	}
	
}

template <class T>
T& ID_LOOKUP_TABLE<T>::get(ID id) {
	
	return components[ indices[id.index].object ];
	
}

template <class T>
ID ID_LOOKUP_TABLE<T>::add(T& cmp) {
	
	INDEX index;
	
	if (next_index == MAX_NO_OBJECTS) {
		printf("Array is full.");
		
		index.id.index = UINT_MAX;
		index.id.verif = UINT_MAX;
		
	} else {

		//update the table's next_index
		index.id.index = next_index;
		index.id.verif = indices[next_index].id.verif;
		index.object   = num_objects;

		next_index =  indices[next_index].next;
		index.next = UINT_MAX;

		indices[ index.id.index ] = index;

		//tell the object the ID it was given
		cmp.id = index.id;

		//add the object to the end the objects array
		components[num_objects] = cmp;
		++num_objects;

	}
	
	//return the ID
	return index.id;
	
}

template <class T>
void ID_LOOKUP_TABLE<T>::remove(ID id) {
	
	if (id.index == UINT_MAX) {return;} //exit if passed a null id

	//overwrite the deleted object with the object at the end
	INDEX d_index = indices[id.index];
	//delete objects[ d_index.object ];
	
	T& o = components[ --num_objects ];
	components[ d_index.object ] = o;
	
	//update the moved objects' indices entries
	(indices[ o.id.index ]).object = d_index.object;
	d_index.object = UINT_MAX;
	
	//delete the end object
	//delete objects[ num_objects ];
	//NOTE: the end object isn't actually deleted, so only ever loop to (num_objects)
	
	//update the table's next_index
	d_index.next = next_index;
	next_index = id.index;
	
	//increment the verification number
	//if it would overflow, just reset it to zero
	d_index.id.verif = (d_index.id.verif + 1 != UINT_MAX) ? d_index.id.verif + 1 : 0;
	
}