#include <assert.h>

#include "graph.h"
#include "stack.h"
#include "simulation.h"
#include "fault_simulation.h"
#include "topological_sort.h"

/*************************************************************************************************
 Fault Simulation
*************************************************************************************************/

FaultSimulationTable RunSimulation(NODE* graph, int max_node_id,
                                   PATTERN* input_vectors, int total_vectors,
                                   FAULT* fault, int total_faults)
{
    if(total_vectors <= 0)
        goto zero_total_vectors;
    if(total_faults <= 0)
        goto zero_total_faults;
    if(max_node_id <= 0)
        goto zero_max_node_id;

    int j = 0;
    for(j = 0; j < total_faults; j++)
    {
        if(fault[j].Snod <= 0)
            goto wrong_Snod;
        if(fault[j].Sval != 0 && fault[j].Sval != 1)
            goto wrong_Sval;
    }

    FaultSimulationTable table = CreateFaultSimulationTable();

    int i = 0;
    for(i = 0; i < total_vectors; i++)
    {
        table.row[i] = RunSimulationRow(graph, max_node_id, input_vectors[i], fault, total_faults);
    }

    table.number_of_patterns = total_vectors;
    table.number_of_faults = total_faults;
    table.number_of_inputs = strlen(table.row[0].input_vector);
    table.number_of_outputs = strlen(table.row[0].output_vector);

    if(table.number_of_inputs <= 0 || table.number_of_outputs <= 0)
        goto zero_io_length;

    return table;

zero_total_vectors:
    printf("ERROR: In RunSimulation(...): No vectors passed to function!\n");
    exit(1);

zero_total_faults:
    printf("ERROR: In RunSimulation(...): No faults passed to function!\n");
    exit(1);

zero_max_node_id:
    printf("ERROR: In RunSimulation(...): max_node_id is less than or equal to zero!\n");
    exit(1);

zero_io_length:
    printf("ERROR: In RunSimulation(...): input_vector or output_vector have length zero!\n");
    exit(1);

wrong_Snod:
    printf("ERROR: In RunSimulationRow(...): FAULT Snod is less than or equal to zero!\n");
    exit(1);

wrong_Sval:
    printf("ERROR: In RunSimulationRow(...): FAULT Sval is neither zero nor one!\n");
    exit(1);
}

FaultSimulationTableRow RunSimulationRow(NODE* graph, int max_node_id,
                                         PATTERN input_vector,
                                         FAULT* fault, int total_faults)
{
    assert(max_node_id > 0);
    assert(total_faults > 0);

    FaultSimulationTableRow row = CreateFaultSimulationTableRow();
    strncpy(row.input_vector, input_vector.piv, Mpi);

    Stack* topo_sorted = topological_sort(graph,max_node_id);
    assert(!isempty_stack(topo_sorted));

    int j = 0;
    for(j = 0; j < total_faults; j++)
    {
        assert(fault[j].Snod > 0);
        assert(fault[j].Sval == 0 || fault[j].Sval == 1);

        if(j == 0)
        {
            apply_vector_wofault(graph, max_node_id, topo_sorted,
                                 row.input_vector, row.output_vector);
        }

        row.fault_node[j] = fault[j].Snod; row.fault_value[j] = fault[j].Sval;
        sprintf(row.fault_type[j], "%i/%i", row.fault_node[j], row.fault_value[j]);

        apply_vector_wfault(graph, max_node_id, topo_sorted,
                            row.input_vector, row.fault_output[j], fault[j]);
        row.fault_detected[j] = compare_faulty_circuit_outputs(graph, max_node_id, fault[j]);
    }

    delete_stack(topo_sorted);

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
        assert(det_table.row[i].number_of_detecting_patterns == 0);

        for(j = 0; j < sim_table.number_of_patterns; j++)
        {
            int temp_detected = sim_table.row[j].fault_detected[i];
            assert(temp_detected == 0 || temp_detected == 1);

            det_table.row[i].table_of_detecting_patterns[j] = temp_detected;
            det_table.row[i].number_of_detecting_patterns += temp_detected;
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
