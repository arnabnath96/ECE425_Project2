#ifndef FAULTSIM_H
#define FAULTSIM_H

#include "graph.h"

/*************************************************************************************************
 Fault Simulation Table Structures
*************************************************************************************************/

typedef struct FaultSimulationTableRow
{
    char input_vector[Mpi];
    char output_vector[Mpo];

    int fault_node[Mft];
    int fault_value[Mft];

    char fault_type[Mft][Mlin];
    char fault_output[Mft][Mpo];
    int fault_detected[Mft];
} FaultSimulationTableRow;

typedef struct FaultSimulationTable
{
    FaultSimulationTableRow row[Mpt];

    int number_of_patterns;
    int number_of_faults;
    int number_of_inputs;
    int number_of_outputs;
} FaultSimulationTable;

/*************************************************************************************************
 Detection Table Structures
*************************************************************************************************/

typedef struct DetectionTableRow
{
    int number_of_detecting_patterns;
    int table_of_detecting_patterns[Mpt];
} DetectionTableRow;

typedef struct DetectionTable
{
    DetectionTableRow row[Mft];

    int number_of_patterns;
    int number_of_faults;
} DetectionTable;

/*************************************************************************************************
 Fault Table Structures
*************************************************************************************************/

typedef struct FaultTable
{
    char table_of_faults[Mft][Mlin];

    int number_of_faults;
} FaultTable;

/*************************************************************************************************
  Functions
*************************************************************************************************/

FaultSimulationTable RunSimulation(NODE* graph, int max_node_id,
                                   PATTERN* input_vectors, int total_vectors,
                                   FAULT* fault, int total_faults);

FaultSimulationTableRow RunSimulationRow(NODE* graph, int max_node_id,
                                         PATTERN input_vector,
                                         FAULT* fault, int total_faults);

DetectionTable ConvertFaultSimulationTableToDetectionTable(FaultSimulationTable sim_table);
FaultTable ConvertFaultSimulationTableToFaultTable(FaultSimulationTable sim_table);

FaultSimulationTable CreateFaultSimulationTable();
FaultSimulationTableRow CreateFaultSimulationTableRow();

DetectionTable CreateDetectionTable();
DetectionTableRow CreateDetectionTableRow();

FaultTable CreateFaultTable();

/************************************************************************************************/

#endif //FAULTSIM_H
