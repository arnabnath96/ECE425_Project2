#include "display.h"
#include "graph.h"
#include "fault_simulation.h"
#include "stdarg.h"

char faulty_type[Mft][Mlin];
char faulty_output_vector[Mft][Mpo];
int faulty_detected[Mft];

void print_pattern_table(FILE* fres, FaultSimulationTable table)
{
    int maxlength_fault = 0; int maxlength_pattern = 0;
    int i = 0; int j = 0;

    //Determine maximum length of element in fault_type
    for(i = 0, j = 0; i < table.number_of_faults; i++)
    {
        j = strlen(table.row[0].fault_type[i]);
        maxlength_fault = (j > maxlength_fault) ? j : maxlength_fault;
    }

    //Determine the maximum length of input patterns
    maxlength_pattern = strlen(table.row[0].output_vector);

    //Ensure that maximum lengths are of a minimum value and increment
    if(maxlength_fault < 5) maxlength_fault = 5; maxlength_fault += 4;
    if(maxlength_pattern < 6) maxlength_pattern = 7; maxlength_pattern += 4;

    for(j = 0; j <table.number_of_patterns; j++)
    {
        //Print test pattern
        tee(fres, "Test Pattern: %s\n",table.row[j].input_vector);
        tee(fres, "%-*s%-*s%s\n",
            maxlength_fault, "Fault",
            maxlength_pattern, "Output", "Detected");
        tee(fres, "%-*s%-*s%s\n",
            maxlength_fault, "None",
            maxlength_pattern, table.row[j].output_vector, "NA");

        //Print table rows
        for(i = 0; i < table.number_of_faults; i++)
        {
            if(table.row[j].fault_detected[i] == 0 || table.row[j].fault_detected[i] == 1)
            {
                tee(fres, "%-*s%-*s%s\n",
                    maxlength_fault, table.row[j].fault_type[i],
                    maxlength_pattern, table.row[j].fault_output[i],
                    table.row[j].fault_detected[i]>0?"Yes":"No");
            }
            else
            {
                tee(fres, "%-*s%-*s%s\n",
                    maxlength_fault, table.row[j].fault_type[i],
                    maxlength_pattern, table.row[j].fault_output[i], "ER");
            }
        }

        tee(fres, "\n");
    }

    return;
}

void print_detection_table(FILE* fres, FaultTable fau_table, DetectionTable det_table)
{
    int maxlength_fault = 0; int maxlength_pattern = 0;
    int i = 0; int j = 0;

    //Determine maximum length of element in FaultTable
    for(i = 0, j = 0; i < fau_table.number_of_faults; i++)
    {
        j = strlen(fau_table.table_of_faults[i]);
        maxlength_fault = (j > maxlength_fault) ? j : maxlength_fault;
    }

    //Determine the maximum length of number of input patterns
    maxlength_pattern = (int)(ceil(log10(Mpt)));

    //Ensure that maximum lengths are of a minimum value and increment
    if(maxlength_fault < 5) maxlength_fault = 5; maxlength_fault += 4;
    if(maxlength_pattern < 7) maxlength_pattern = 7; maxlength_pattern += 4;

    tee(fres,"\n\nNumber of Input Patterns That Detect Each Fault\n");
    tee(fres,"%-*s%-*s%s\n",
        maxlength_fault, "Fault",
        maxlength_pattern, "Patterns", "Detected");

    for(i = 0; i < fau_table.number_of_faults; i++)
    {
        tee (fres, "%-*s%-*i%s\n",
             maxlength_fault, fau_table.table_of_faults[i],
             maxlength_pattern, det_table.row[i].number_of_detecting_patterns,
             det_table.row[i].number_of_detecting_patterns>0?"Yes":"No");
    }

    return;
}

void tee(FILE* file, char* format, ...)
{
    va_list additional_args;

    va_start(additional_args, format);
    vprintf(format, additional_args);
    va_end(additional_args);

    va_start(additional_args, format);
    vfprintf(file, format, additional_args);
    va_end(additional_args);

    return;
}
