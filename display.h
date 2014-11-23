#ifndef DISPLAY_H
#define DISPLAY_H

#include "graph.h"
#include "fault_simulation.h"

void print_pattern_table(FILE* fres, FaultSimulationTable table);
void print_detection_table(FILE* fres, FaultTable fau_table, DetectionTable det_table);

void tee(FILE* file, char* format, ...);

#endif // DISPLAY_H
