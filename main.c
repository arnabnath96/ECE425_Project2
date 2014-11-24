#include "graph.h"
#include "stack.h"
#include "topological_sort.h"
#include "simulation.h"
#include "fault_simulation.h"
#include "display.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define NUMBER_OF_ARGS 5
#define NO_DEBUG

/*************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
*************************************************************************************************/
int main(int argc,char **argv)
{

if(argc != NUMBER_OF_ARGS+1)
{
    printf("ERROR: In main(...): The correct number of arguments has not been passed!\n");
    exit(1);
}

if((FileExists(argv[1]) && FileExists(argv[2]) && FileExists(argv[5])) == 0)
{
    printf("ERROR: In main(...): One of the filenames does not exist!\n");
    exit(1);
}

/*************************************************************************************************
 Declare variables
*************************************************************************************************/

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
int a;

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
for(a=0;a<Total;a++){  printf("%d\t%s\n",a,vector[a].piv); }

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
for(a=0;a<Total;a++){  printf("%d\t%s\n",a,vector[a].piv); }

printf("\n");

/*************************************************************************************************
 Begin logic simulation and logging
*************************************************************************************************/

//File pointer to open .out file for printing results
fres=fopen(argv[4],"w");

//Simulate and store in appropriate data structures
FaultSimulationTable    sim_table   = RunSimulation(graph, Max, vector, Total, fault, Tfs);
DetectionTable          det_table   = ConvertFaultSimulationTableToDetectionTable(sim_table);
FaultTable              fau_table   = ConvertFaultSimulationTableToFaultTable(sim_table);

//Print simulation results
print_pattern_table(fres, sim_table);
print_detection_table(fres, fau_table, det_table);

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

