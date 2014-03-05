#pragma once

struct ID
{
	unsigned int index;
	unsigned int verif;
};

struct INDEX
{
	ID id;
	unsigned int object;
	unsigned int next;
};