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

printf("\nOutput Vectors");
printf("\nIndex\tInputVector\tOutputVector\n");

fprintf(fres,"Index\tInputVector\tOutputVector\n");

int i = 0; int j = 0; Stack* temp;
for(i = 0; i < Total; i++) //Iterate over each input vector
{

    /**************************************************************************
     Simulation
    **************************************************************************/

    //Make a copy of the input vector and topographically sorted stack;
    //Then apply the input vector and get the output vector
    temp = copy_stack(topo_sorted);
    memcpy(input_vector, vector[i].piv, Mpi);
    output_vector_length = apply_vector(graph, Max, temp, input_vector, output_vector);
    delete_stack(temp);


    /**************************************************************************
     Log
    **************************************************************************/

    //Print index
    fprintf(fres,"%d\t",i);
    printf("%d\t",i);

    //Print input vector
    for(j = 0; j < Mpi; j++)
    {
	if(input_vector[j] == '\n' || input_vector[j] == '\0')
        {
            break;
        }
        else
        {
            fprintf(fres,"%c",input_vector[j]);
            printf("%c", input_vector[j]);
        }
    }

    //Print seperator
    fprintf(fres,"\t");
    printf("\t");

    //Print output vector
    for(j = 0; j < output_vector_length; j++)
    {
        fprintf(fres,"%c",output_vector[j]);
        printf("%c",output_vector[j]);
    }

    //Print newline
    fprintf(fres, "\n");
    printf("\n");
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

