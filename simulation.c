#include <assert.h>
#include "simulation.h"
#include "graph.h"
#include "stack.h"
#include "gates.h"

//Destructively changes all of the unknown values ('x') in a set of input vectors to a given xval.
void set_vector(PATTERN* input_vector, int length, int xval)
{
    int i = 0; int j = 0;
    for(i = 0; i < length; i++) //Iterate over each vector
    {
        for(j = 0; j < Mpi; j++) //Iterate through each vector
        {
            if(input_vector[i].piv[j] == 'x')
            {
                if(xval == 1)
                    input_vector[i].piv[j] = '1';
                else if(xval == 0)
                    input_vector[i].piv[j] = '0';
                else
                    input_vector[i].piv[j] = 'x';
            }
        }
    }
	return;
}

//Computes the output vector for a given input vector applied to a graph representing a
//combinational netlist; returns the length of the output vector. Note that the user must provide
//a topological sort of the nodes in the graph.
int apply_vector_wfault(NODE* graph, int max, Stack* sorted, char* input_vector, char* output_vector, int Snod, int Sval)
{
	if(isempty_stack(sorted))
	{
		printf("STACK: No elements to pop!\n");
		exit(1);
	}

    apply_circuit_inputs(graph,sorted,input_vector, Snod, Sval);
	return read_circuit_outputs(graph,max,output_vector, Snod, Sval);
}

int apply_vector_wofault(NODE* graph, int max, Stack* sorted, char* input_vector, char* output_vector)
{
    return apply_vector_wfault(graph, max, sorted, input_vector, output_vector, -1, -1);
}

//Applies the input vector to the primary inputs of a graph representing a combinational netlist
//and propagates those inputs through the circuit.
void apply_circuit_inputs(NODE* graph, Stack* sorted, char* input_vector, int Snod, int Sval)
{
	//Keeps track of bits in input vector that are already applied
    int input_vector_iter = 0;

	//Keeps track of inputs to an individual node. NOTE: MEMSET CAN ONLY ASSIGN '0' AND '-1' TO
	//AN INTEGER ARRAY DUE TO INTERNAL REPRESENTATION OF INTEGERS. '0' IS USED AS A BOOLEAN VALUE
	//SO '-1' MUST BE USED AS A DEFAULT VALUE.
    int node_input[Min]; memset(node_input, -1, sizeof(node_input));

    int id = 0;
    do //Do while there are unprocessed nodes in the graph
	{
		id = pop_stack(sorted);
		get_node_inputs(graph,id,input_vector,&input_vector_iter,node_input, Snod, Sval);
        apply_node_inputs(graph,id,node_input, Snod, Sval);

        // Reset variables
        memset(node_input, -1, sizeof(node_input));
    } while(!isempty_stack(sorted));

	return;
}

//Given a graph representing a combinational circuit and a node in that graph, determines the inputs
//to that node -- whether those inputs are from a previous node or from an input vector.
void get_node_inputs(NODE* graph, int id, char* input_vector, int* input_vector_iter, int* node_input, int Snod, int Sval)
{
	switch (graph[id].Type)
	{
		case INPT  :
			{
				node_input[0] = assign_bitvalue_ctoi(input_vector[*input_vector_iter]);
				(*input_vector_iter)++;
				break;
			}
		case AND   :
		case NAND  :
		case OR    :
		case NOR   :
		case XOR   :
		case XNOR  :
		case BUFF  :
		case NOT   :
			{
				LIST* current_fanin = graph[id].Fin;
				int number_of_fanin = graph[id].Nfi;

				int i = 0;
				for(i = 0; i <= (number_of_fanin-1); i++) //Iterate over fanin nodes
				{
				    if(Snod <= -1)
                    {
                        node_input[i] = graph[current_fanin->id].Cval;
                    }
					else
                    {
                         node_input[i] = graph[current_fanin->id].Fval;
                    }
					current_fanin = current_fanin->next;
				}

				break;
			}
		case FROM  :
			{
			    if(Snod <= -1)
                {
                    node_input[0] = graph[graph[id].Fin->id].Cval;
                }
                else
                {
                    node_input[0] = graph[graph[id].Fin->id].Fval;
                }
				break;
			}
	}
	return;
}

//Given a graph representing a combinational circuit, a node in that graph, and the inputs to that
//node, determines the proper output and assigns that value to the node.
void apply_node_inputs(NODE* graph, int id, int* node_input, int Snod, int Sval)
{
    int* Xval = 0;
    if(Snod <= -1)  {Xval = &graph[id].Cval;}
    else            {Xval = &graph[id].Fval;}

    if(Snod > -1 && Snod == id)
    {
        *Xval = Sval;
    }
    else
    {
        // Apply Node Inputs
        switch (graph[id].Type)
        {
            case INPT  :
                *Xval = node_input[0];
                break;
            case AND   :
                *Xval = andg(node_input);
                break;
            case NAND  :
                *Xval = nandg(node_input);
                break;
            case OR    :
                *Xval = org(node_input);
                break;
            case NOR   :
                *Xval = norg(node_input);
                break;
            case XOR   :
                *Xval = xorg(node_input);
                break;
            case XNOR  :
                *Xval = xnorg(node_input);
                break;
            case BUFF  :
                *Xval = bufferg(node_input);
                break;
            case NOT   :
                *Xval = notg(node_input);
                break;
            case FROM  :
                *Xval = fromg(node_input);
                break;
        }
	}
	return;
}

//Reads the primary outputs from a graph representing a combinational circuit into an output vector
int read_circuit_outputs(NODE* graph, int max, char* output_vector, int Snod, int Sval)
{
	//Set output vector to a known string
	memset(output_vector, 'z', Mpo);

	//Keeps track of bits in output vector that are already assigned
	int output_vector_iter = 0;

	//Keeps track of inputs to an individual node.
	char output_bit = 0;

    int id = 0;
	for(id = 0; id <= max; id++) //Iterate over every node in graph
	{
		if(graph[id].Po) //If node is a primary output
		{
		    if(Snod <= -1)
            {
                output_bit = assign_bitvalue_itoc(graph[id].Cval);
            }
		    else
            {
                output_bit = assign_bitvalue_itoc(graph[id].Fval);
            }
		    output_vector[output_vector_iter] = output_bit;
			output_vector_iter++;
        }
	}
	return output_vector_iter;
}

//Helper function: Convert from character to integer representation of bit
int assign_bitvalue_ctoi(char test_vector_bit)
{
    switch(test_vector_bit)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case 'x': //Unknown value
            return 2;
        default: //Error value
            return -1;
    }
}

//Helper function: Convert from integer to character representation of bit
char assign_bitvalue_itoc(int test_vector_bit)
{
    switch(test_vector_bit)
    {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2: //Unknown value
            return 'x';
        default: //Error value
            return 'E';
    }
}
