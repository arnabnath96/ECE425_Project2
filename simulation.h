#ifndef SIMULATION_H
#define SIMULATION_H

#include "graph.h"
#include "stack.h"

void set_vector(PATTERN* input_vector, int length, int xval);
int apply_vector_wfault(NODE* graph, int max, Stack* sorted, char* input_vector, char* output_vector, int Snod, int Sval);
int apply_vector_wofault(NODE* graph, int max, Stack* sorted, char* input_vector, char* output_vector);

void apply_circuit_inputs(NODE* graph, Stack* sorted, char* input_vector, int Snod, int Sval);
void get_node_inputs(NODE* graph, int id, char* input_vector, int* input_vector_iter, int* node_input, int Snod, int Sval);
void apply_node_inputs(NODE* graph, int id, int* node_input, int Snod, int Sval);
int read_circuit_outputs(NODE* graph, int max, char* output_vector, int Snod, int Sval);

int compare_faulty_circuit_outputs(NODE* graph, int max, int Snod, int Sval);

int assign_bitvalue_ctoi(char);
char assign_bitvalue_itoc(int);

#endif // SIMULATION_H
