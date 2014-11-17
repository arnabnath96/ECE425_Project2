#include "gates.h"
#include "graph.h"

int andg(int* node_input)
{
	int unknown_flag = 0;
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
        if(node_input[i] == 0)
        {
            return 0;
        }
        if(node_input[i] == 2)
        {
            unknown_flag = 1;
        }
    }
	
	if(unknown_flag)
		return 2;
	else
		return 1;
}

int nandg(int* node_input)
{
	int and_val = andg(node_input);
	
    if (and_val == 2)
    {
        return 2;
    }
    return !and_val;
}

int org(int* node_input)
{
	int unknown_flag = 0;
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
        if(node_input[i] == 1)
        {
            return 1;
        }
        if(node_input[i] == 2)
        {
            unknown_flag = 1;
        }
    }
	if(unknown_flag)
		return 2;
	else
		return 0;
}

int norg(int* node_input)
{
	int or_val = org(node_input);
	
    if (or_val == 2)
    {
        return 2;
    }
    return !or_val;
}

int xorg(int* node_input)
{
    int i = 0; int j = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
        if(node_input[i] == 1)
        {
            j++;
        }

        if(node_input[i] == 2)
        {
            return 2;
        }
    }

    return j % 2;
}

int xnorg(int* node_input)
{
	int xor_val = xorg(node_input);
	
    if (xor_val == 2)
    {
        return 2;
    }
    return !xor_val;
}

int bufferg(int* node_input)
{
	if(node_input[0] == 2)
		return 2;
	else
		return !!node_input[0];
}

int notg(int* node_input)
{
	if(node_input[0] == 2)
		return 2;
	else
		return !node_input[0];
}

int fromg(int* node_input)
{
	if(node_input[0] == 2)
		return 2;
	else
		return !!node_input[0];
}
