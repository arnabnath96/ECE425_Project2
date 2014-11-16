#include "gates.h"
#include "graph.h"

static int andt[3][3] = {{0, 0, 0}, {0, 1, 2}, {0, 2, 2}};
static int nandt[3][3] = {{1, 1, 1}, {1, 0, 2}, {1, 2, 2}}; //NOT USED
static int ort[3][3] = {{0, 1, 2}, {1, 1, 1}, {2, 1, 2}};
static int nort[3][3] = {{1, 0, 2}, {0, 0, 0}, {2, 0, 2}}; //NOT USED
static int xort[3][3] = {{0, 1, 2}, {1, 0, 2}, {2, 2, 2}};
static int xnort[3][3] = {{1, 0, 2}, {0, 1, 2}, {2, 2, 2}};
static int buffert[3] = {0, 1, 2};
static int nott[3] = {1, 0, 2};
static int fromt[3] = {0, 1, 2};

int andg(int* node_input)
{
	int current_value = 1; //Conjunctive Identity
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
		current_value = andt[current_value][node_input[i]];
    }
	return current_value;
}

int nandg(int* node_input)
{
	int current_value = 0; //Disjunctive Identity
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
		current_value = ort[current_value][nott[node_input[i]]];
    }
	return current_value;
}

int org(int* node_input)
{
	int current_value = 0; //Disjunctive Identity
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
		current_value = ort[current_value][node_input[i]];
    }
	return current_value;
}

int norg(int* node_input)
{
	int current_value = 1; //Conjunctive Identity
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
		current_value = andt[current_value][nott[node_input[i]]];
    }
	return current_value;
}

int xorg(int* node_input)
{
	// 0 XOR A = A -> Compute odd-parity
	int current_value = 0;
	
    int i = 0;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
		current_value = xort[current_value][node_input[i]];
    }
	return current_value;
}

int xnorg(int* node_input)
{
	// 0 XOR A = A -> Compute odd-parity
	int current_value = 0;
	
    int i = 1;
    for(i=0; i < Min && node_input[i] != -1; i++)
    {
		current_value = xort[current_value][node_input[i]];
    }
	
	// NOT (Odd-Parity) = (Even-Parity)
	current_value = nott[current_value]; 
	
	return current_value;
}

int bufferg(int* node_input)
{
	return buffert[node_input[0]];
}

int notg(int* node_input)
{
	return nott[node_input[0]];
}

int fromg(int* node_input)
{
	return fromt[node_input[0]];
}
