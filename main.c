#include "graph.h"
#include "stack.h"
#include "topological_sort.h"
#include "simulation.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

/*************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
*************************************************************************************************/
int main(int argc,char **argv)
{
//file pointers used for .isc file, .vec file, .faults file and resultfile
FILE *fisc,*fvec,*ffau,*fres;

//maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults
int Max,Opt,Npi,Npo,Total,Tfs;

//structure used to store the ckt information in .isc file
NODE graph[Mnod];

//structure used to store the input vectors information in .vec file
PATTERN vector[Mpt];

//structure used to store the input vectors information in .vec file
FAULT fault[Mft];

//random variables
int a,b,c,d;

/*************************************************************************************************
 Read the .isc file and store the information in graph structure
*************************************************************************************************/

//file pointer to open .isc file
fisc=fopen(argv[1],"r");

//read .isc file and return index of last node in graph formed
Max=0; Max=ReadIsc(fisc,graph);

//close file pointer for .isc file
fclose(fisc);

//print all members of graph structure
PrintCircuit(graph,Max);

/*************************************************************************************************
 Read the .vec file and store the information in  vector structure
*************************************************************************************************/

//file pointer to open .vec file
fvec=fopen(argv[2],"r");

//read .vec file and store in vector structure and return Total number of patterns
Total=0; Total=ReadVec(fvec,vector);

//close file pointer for .vec file
fclose(fvec);

//print total number of patterns in .vec file
printf("\nTotal No. of Pattern: %d",Total);

//print all members of vector structure
printf("\n\nInput Vectors without Xval Replacement");
printf("\nIndex\tInputVector\n");
for(a=0;a<Total;a++){  printf("%d\t%s",a,vector[a].piv); }

/*************************************************************************************************
 Read the .faults file and store the information in faults structure
*************************************************************************************************/

//file pointer to open .vec file
ffau=fopen(argv[5],"r");

//read .vec file and store in vector structure and return Total number of patterns
Tfs=0; Tfs=ReadFaults(ffau,fault);

//close file pointer for .vec file
fclose(ffau);

//print total number of patterns in .vec file
printf("\nTotal No. of Faults: %d",Tfs);

//print all members of vector structure
printf("\n\nStuck-At Faults");
printf("\nNode\tStuck-At\n");
for(a=0;a<Tfs;a++){  printf("%i\t%i\n",fault[a].Snod,fault[a].Sval); }

/*************************************************************************************************
 Set XVal in vector structure
*************************************************************************************************/

//getting the option from terminal for xval
Opt=0; Opt=atoi(argv[3]);

//set xval in each of the vectors
set_vector(vector,Total,Opt);
printf("\nReplacing 'x' with %d...\n", Opt);

//print all members of vector structure
printf("\nInput Vectors with Xval Replacement");
printf("\nIndex\tInputVector\n");
for(a=0;a<Total;a++){  printf("%d\t%s",a,vector[a].piv); }

/*************************************************************************************************
 Begin logic simulation and logging
*************************************************************************************************/

//File pointer to open .out file for printing results
fres=fopen(argv[4],"w");

//Sort nodes in graph in topological order to facilitate simulation
Stack* topo_sorted = topological_sort(graph,Max);

//Create structures to hold input and output vectors
char input_vector[Mpi];
char output_vector[Mpo];
int output_vector_length = 0;

//Create structures to hold data to be printed to screen/file
char fault_string[Mft+1][Mlin];
char output_vector_string[Mft+1][Mpo];
int detected_int[Mft+1];

//Flag to store whether a fault can be sensed or not
int detected_flag;

int i = 0; int j = 0; int m = 0; Stack* temp;
for(i = 0; i < Total; i++) //Iterate over each input vector
{
    //Reset values
    memset(fault_string, 0, (Mft+1)*Mlin);
    memset(output_vector_string, 0, (Mft+1)*Mpo);
    memset(detected_int, -1, sizeof(detected_int[0])*(Mft+1));
    detected_flag = 0;

    /**************************************************************************
     Simulation
    **************************************************************************/

    for(m = 0; m < Tfs; m++) // Iterate over each fault
    {
        temp = copy_stack(topo_sorted);
        memcpy(input_vector, vector[i].piv, Mpi);

        if(m==0)
        {
            sprintf(fault_string[m], "None");
            output_vector_length = apply_vector_wofault(graph, Max, temp, input_vector, output_vector_string[m]);
            detected_int[m] = 2;
        }
        else
        {
            sprintf(fault_string[m],"%i/%i",fault[m-1].Snod,fault[m-1].Sval);
            apply_vector_wfault(graph, Max, temp, input_vector, output_vector_string[m], fault[m-1].Snod, fault[m-1].Sval);
            detected_int[m] = compare_faulty_circuit_outputs(graph, Max, fault[m-1].Snod, fault[m-1].Sval);
        }

        delete_stack(temp);
    }

    /**************************************************************************
     Log
    **************************************************************************/

    printf("\nTest Pattern: ");
    fprintf(fres, "\nTest Pattern: ");
    //Print input vector
    for(j = 0; j < Mpi; j++)
    {
	if(input_vector[j] == '\n' || input_vector[j] == '\0')
        {
            break;
        }
        else
        {
            printf("%c", input_vector[j]);
            fprintf(fres, "%c", input_vector[j]);
        }
    }

    //Print table header
    printf("\nFault\tOutput\tDetected\n");
    fprintf(fres, "\nFault\tOutput\tDetected\n");

    //Print table rows
    for(m = 0; m < Tfs; m++)
    {
        if(detected_int[m] == 2)
        {
            printf("%s\t%s\tNA\n", fault_string[m], output_vector_string[m]);
            fprintf(fres, "%s\t%s\tNA\n", fault_string[m], output_vector_string[m]);
        }
        else if(detected_int[m] == 0 || detected_int[m] == 1)
        {
            printf("%s\t%s\t%i\n", fault_string[m], output_vector_string[m], detected_int[m]);
            fprintf(fres, "%s\t%s\t%i\n", fault_string[m], output_vector_string[m], detected_int[m]);

            if(detected_int[m] == 1)
                detected_flag = 1;
        }
        else
        {
            printf("%s\t%s\tER\n", fault_string[m], output_vector_string[m]);
            fprintf(fres, "%s\t%s\tER\n", fault_string[m], output_vector_string[m]);
        }
    }

    //Print detection status
    if(!detected_flag)
    {
        printf("STATUS: FAULT NOT DETECED\n");
        fprintf(fres, "STATUS: FAULT NOT DETECED\n");
    }
    else
    {
        printf("STATUS: FAULT DETECTED\n");
        fprintf(fres, "STATUS: FAULT DETECTED\n");
    }
}

//Free stack used to hold topological sort
delete_stack(topo_sorted);

//Close file pointer for .out file
fclose(fres);

/*************************************************************************************************
 Clean up
*************************************************************************************************/

//clear memory for all members of graph
ClearCircuit(graph,Mnod);
//clear memory for all members of vector
for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }

return 0;
}//end of main
/************************************************************************************************/

