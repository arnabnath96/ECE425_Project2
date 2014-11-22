#include "graph.h"

void print_pattern_table(FILE* fres, int Tfs, char* input_vector, char* output_vector, char faulty_type[][Mlin], char faulty_output_vector[][Mpo], int* faulty_detected);
void print_detection_table(FILE* fres, int Total, int Tfs, char FaultTable[][Mlin], int DetectionTable[][Mpt+1]);
void tee(FILE* file, char* format, ...);
