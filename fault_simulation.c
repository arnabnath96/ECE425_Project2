#include "graph.h"
#include "stack.h"
#include "simulation.h"
#include "fault_simulation.h"
#include "topological_sort.h"

/*************************************************************************************************
 Fault Simulation
*************************************************************************************************/

FaultSimulationTable RunSimulation(NODE* graph, int Max, PATTERN* input_vectors, int Total, FAULT* fault, int Tfs)
{
    FaultSimulationTable table = CreateFaultSimulationTable();

    int i = 0;
    for(i = 0; i < Total; i++)
    {
        table.row[i] = RunSimulationRow(graph, Max, input_vectors[i], fault, Tfs);
    }

    table.number_of_patterns = Total;
    table.number_of_faults = Tfs;
    table.number_of_inputs = (Total > 0) ? strlen(table.row[0].input_vector) : 0;
    table.number_of_outputs = (Total > 0) ? strlen(table.row[0].output_vector) : 0;

    return table;
}

FaultSimulationTableRow RunSimulationRow(NODE* graph, int Max, PATTERN input_vector, FAULT* fault, int Tfs)
{
    FaultSimulationTableRow row = CreateFaultSimulationTableRow();
    strncpy(row.input_vector, input_vector.piv, Mpi);

    int j = 0; Stack* topo_sorted = topological_sort(graph,Max);
    for(j = 0; j < Tfs; j++)
    {
        if(j == 0)
        {
            apply_vector_wofault(graph, Max, topo_sorted, row.input_vector, row.output_vector);
        }

        row.fault_node[j] = fault[j].Snod; row.fault_value[j] = fault[j].Sval;
        sprintf(row.fault_type[j], "%i/%i", row.fault_node[j], row.fault_value[j]);

        apply_vector_wfault(graph, Max, topo_sorted, row.input_vector, row.fault_output[j], fault[j]);
        row.fault_detected[j] = compare_faulty_circuit_outputs(graph, Max, fault[j]);
    } delete_stack(topo_sorted);

    return row;
}

/*************************************************************************************************
 Fault Simulation Table
*************************************************************************************************/

FaultSimulationTable CreateFaultSimulationTable()
{
    FaultSimulationTable table;

    int i = 0;
    for(i=0; i < Mpt; i++)
    {
        table.row[i] = CreateFaultSimulationTableRow();
    }

    table.number_of_patterns = 0;
    table.number_of_faults = 0;
    table.number_of_inputs = 0;
    table.number_of_outputs = 0;

    return table;
}

FaultSimulationTableRow CreateFaultSimulationTableRow()
{
    FaultSimulationTableRow row;

    memset(row.input_vector,0,Mpi);
    memset(row.output_vector,0,Mpo);
    memset(row.fault_node,0,Mft*sizeof(int));
    memset(row.fault_value,0,Mft*sizeof(int));
    memset(row.fault_type,0,Mft*Mlin);
    memset(row.fault_output,0,Mft*Mpo);
    memset(row.fault_detected,0,Mft*sizeof(int));

    return row;
}

/*************************************************************************************************
 Detection Table
*************************************************************************************************/

DetectionTable CreateDetectionTable()
{
    DetectionTable table;

    int i = 0;
    for(i=0; i < Mft; i++)
    {
        table.row[i] = CreateDetectionTableRow();
    }

    table.number_of_patterns = 0;
    table.number_of_faults = 0;

    return table;
}

DetectionTableRow CreateDetectionTableRow()
{
    DetectionTableRow row;

    row.number_of_detecting_patterns = 0;
    memset(row.table_of_detecting_patterns,0,Mpt*sizeof(int));

    return row;
}

DetectionTable ConvertFaultSimulationTableToDetectionTable(FaultSimulationTable sim_table)
{
    DetectionTable det_table = CreateDetectionTable();

    int i = 0; int j = 0;
    for(i = 0; i < sim_table.number_of_faults; i++)
    {
        for(j = 0; j < sim_table.number_of_patterns; j++)
        {
            det_table.row[i].table_of_detecting_patterns[j] = sim_table.row[j].fault_detected[i];
            det_table.row[i].number_of_detecting_patterns += det_table.row[i].table_of_detecting_patterns[j];
        }
    }

    det_table.number_of_patterns = sim_table.number_of_patterns;
    det_table.number_of_faults = sim_table.number_of_faults;

    return det_table;
}

/*************************************************************************************************
 Fault Table
*************************************************************************************************/

FaultTable CreateFaultTable()
{
    FaultTable table;

    memset(table.table_of_faults,0,Mft*Mlin);
    table.number_of_faults = 0;

    return table;
}

FaultTable ConvertFaultSimulationTableToFaultTable(FaultSimulationTable sim_table)
{
    FaultTable fau_table = CreateFaultTable();

    int i = 0;
    for(i = 0; i < sim_table.number_of_faults; i++)
    {
        strncpy(fau_table.table_of_faults[i], sim_table.row[0].fault_type[i],Mlin);
    }

    fau_table.number_of_faults = sim_table.number_of_faults;

    return fau_table;
}

/************************************************************************************************/
