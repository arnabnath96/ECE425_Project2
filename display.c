#include "display.h"
#include "graph.h"
#include "stdarg.h"

char faulty_type[Mft][Mlin];
char faulty_output_vector[Mft][Mpo];
int faulty_detected[Mft];

void print_pattern_table(FILE* fres, int Tfs, char* input_vector, char* output_vector, char faulty_type[][Mlin], char faulty_output_vector[][Mpo], int* faulty_detected)
{
    int maxlength_fault = 0; int maxlength_pattern = 0;
    int i = 0; int j = 0;

    //Determine maximum length of element in fault_type
    for(i = 0, j = 0; i < Tfs; i++)
    {
        j = strlen(faulty_type[i]);
        maxlength_fault = (j > maxlength_fault) ? j : maxlength_fault;
    }

    //Determine the maximum length of input patterns
    maxlength_pattern = strlen(faulty_output_vector[0]);

    //Ensure that maximum lengths are of a minimum value and increment
    if(maxlength_fault < 5) maxlength_fault = 5; maxlength_fault += 4;
    if(maxlength_pattern < 6) maxlength_pattern = 7; maxlength_pattern += 4;

    //Print test pattern
    tee(fres, "Test Pattern: %s\n",input_vector);
    tee(fres, "%-*s%-*s%s\n", maxlength_fault, "Fault", maxlength_pattern, "Output", "Detected");
    tee(fres, "%-*s%-*s%s\n", maxlength_fault, "None", maxlength_pattern, output_vector, "NA");

    //Print table rows
    for(i = 0; i < Tfs; i++)
    {
        if(faulty_detected[i] == 0 || faulty_detected[i] == 1)
        {
            tee(fres, "%-*s%-*s%s\n",  maxlength_fault, faulty_type[i], maxlength_pattern, faulty_output_vector[i], faulty_detected[i]>0?"Yes":"No");
        }
        else
        {
            tee(fres, "%-*s%-*s%s\n",  maxlength_fault, faulty_type[i], maxlength_pattern, faulty_output_vector[i], "ER");
        }
    }

    tee(fres, "\n");

    return;
}


void print_detection_table(FILE* fres, int Tvec, int Tfs, char FaultTable[][Mlin], int DetectionTable[][Mpt+1])
{
    int maxlength_fault = 0; int maxlength_pattern = 0;
    int i = 0; int j = 0;

    //Determine maximum length of element in FaultTable
    for(i = 0, j = 0; i < Tfs; i++)
    {
        j = strlen(FaultTable[i]);
        maxlength_fault = (j > maxlength_fault) ? j : maxlength_fault;
    }

    //Determine the maximum length of number of input patterns
    maxlength_pattern = (int)(ceil(log10(Mpt)));

    //Ensure that maximum lengths are of a minimum value and increment
    if(maxlength_fault < 5) maxlength_fault = 5; maxlength_fault += 4;
    if(maxlength_pattern < 7) maxlength_pattern = 7; maxlength_pattern += 4;

    tee(fres,"\n\nNumber of Input Patterns That Detect Each Fault\n");
    tee(fres,"%-*s%-*s%s\n", maxlength_fault, "Fault", maxlength_pattern, "Patterns", "Detected");
    for(i = 0; i < Tfs; i++)
    {
        tee (   fres,   "%-*s%-*i%s\n",
                maxlength_fault, FaultTable[i],
                maxlength_pattern, DetectionTable[i][0],
                DetectionTable[i][0]>0?"Yes":"No"
            );
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
