#include <assert.h>
#include "simulation.h"
#include "graph.h"
#include "stack.h"
#include "gates.h"

#define NO_DEBUG

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

int apply_vector_wfault(NODE* graph, int max, Stack* sorted,
                        char* input_vector, char* output_vector,
                        FAULT fault)
{
	if(isempty_stack(sorted))
	{
		printf("STACK: No elements to pop!\n");
		exit(1);
	}

	if(fault.Snod >= -1)
    {
        int a = 0;
        for(a=0; a <= max; a++)
        {
            graph[a].Fval = 3;
            graph[a].Mark = 0;
        }
    }

    Stack* sorted_temp = copy_stack(sorted);
    apply_circuit_inputs(graph,sorted_temp,input_vector, fault);
    delete_stack(sorted_temp);

	return read_circuit_outputs(graph,max,output_vector, fault);
}

int apply_vector_wofault(NODE* graph, int max, Stack* sorted, char* input_vector, char* output_vector)
{
    FAULT nofault = CreateNoFault();
    return apply_vector_wfault(graph, max, sorted, input_vector, output_vector, nofault);
}

void apply_circuit_inputs(NODE* graph, Stack* sorted, char* input_vector, FAULT fault)
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
		get_node_inputs(graph,id,input_vector,&input_vector_iter,node_input, fault);
        apply_node_inputs(graph,id,node_input, fault);

        // Reset variables
        memset(node_input, -1, sizeof(node_input));
    } while(!isempty_stack(sorted));

	return;
}

void get_node_inputs(NODE* graph, int id, char* input_vector, int* input_vector_iter, int* node_input, FAULT fault)
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
				    if(fault.Snod <= -1)
                    {
                        node_input[i] = graph[current_fanin->id].Cval;
                    }
					else
                    {
                         node_input[i] = graph[current_fanin->id].Fval;
                         if(graph[current_fanin->id].Mark == 1)
                         {
                             #ifdef DEBUG
                                if(graph[id].Mark == 1)
                                {
                                    printf("REDUNDANT ");
                                }
                                printf("FAULT PROPAGATED AND MARKED!\n");
                            #endif // DEBUG

                            graph[id].Mark = 1;
                         }
                    }
					current_fanin = current_fanin->next;
				}

				break;
			}
		case FROM  :
			{
			    if(fault.Snod <= -1)
                {
                    node_input[0] = graph[graph[id].Fin->id].Cval;
                }
                else
                {
                    node_input[0] = graph[graph[id].Fin->id].Fval;
                    if(graph[graph[id].Fin->id].Mark == 1)
                    {
                        #ifdef DEBUG
                            if(graph[id].Mark == 1)
                            {
                                printf("REDUNDANT ");
                            }
                            printf("FAULT PROPAGATED AND MARKED!\n");
                        #endif // DEBUG

                        graph[id].Mark = 1;
                    }
                }
				break;
			}
	}
	return;
}

void apply_node_inputs(NODE* graph, int id, int* node_input, FAULT fault)
{
    int* Mark = &(graph[id].Mark);

    int* Xval = 0;
    if(fault.Snod <= -1)  {Xval = &graph[id].Cval;}
    else            {Xval = &graph[id].Fval;}

    if(fault.Snod > -1 && fault.Snod == id)
    {
        *Xval = fault.Sval;
        *Mark = 1;
        #ifdef DEBUG
            printf("\nFAULT INJECTED AND MARKED!\n");
        #endif // DEBUG
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

int read_circuit_outputs(NODE* graph, int max, char* output_vector, FAULT fault)
{
	//Set output vector to a known string
	memset(output_vector, 0, Mpo);

	//Keeps track of bits in output vector that are already assigned
	int output_vector_iter = 0;

	//Keeps track of inputs to an individual node.
	char output_bit = 0;

    int id = 0;
	for(id = 0; id <= max; id++) //Iterate over every node in graph
	{
		if(graph[id].Po) //If node is a primary output
		{
		    if(fault.Snod <= -1)
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

int compare_faulty_circuit_outputs(NODE* graph, int max, FAULT fault)
{
    int detected = 0;

    int id = 0;
    for(id=0; id <= max; id++)
    {
        if(graph[id].Po == 1 && graph[id].Mark == 1)
        {
            if(graph[id].Cval != graph[id].Fval)
            {
                detected = 1;
                break;
            }
        }
    }
    return detected;
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
