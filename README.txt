==============================================================================
MAKING THE PROJECT
==============================================================================

The project can be compiled and linked by running 

'make'

from this folder. The makefile that is invoked has been modified from the project default to ensure compilation of every file in the project.

==============================================================================
RUNNING THE PROJECT
==============================================================================

The project can be run by executing the command 

'project netlist_filename input_filename xval output_filename fault_filename'

where netlist_filename should be replaced with the name of the file containing the circuit description, input_filename should be replaced with the name of the file containing the input vectors, xval should be replaced by the value used in place of unknown values in the input vectors, output_filename should be replaced with the name of the file that the output should be written to, and fault_filename should be replaced with the name of the file containing the stuck-at faults.

==============================================================================
BREAKDOWN OF PROJECT
==============================================================================

For ease of development, the code for this project has been split across a number of source files. Briefly, these files include:

main.c - Main executable unit. Contains code that reads program parameters from the command line and calls functions that read in a circuit description, read in a set of input vectors, read in a set of faults, applies the input vectors to the circuit, and logs resulting output vectors.

graph.h, graph.c - Contains definitions of a linked list, graph node, input vector, and fault and functions that interact with the structures. Additionally contains a number of constants and helper functions.

stack.h, stack.c - Contains definitions of a simple stack node and a simple stack capable of storing integers. The stack is capable of push, pop, print, construction, copy, and destruction operations and also includes an emptiness check. The stack is used to store a topographical sort of the nodes in the netlist.

topological_sort.h, topological_sort.c - Contains functions used to sort a graph representing a combinational circuit in topological order. The output of the topological sort operation is a stack of identifiers for nodes in the graph ordered from first nodes in the topological sort (top of stack; inputs of circuit) to last nodes in the topological sort (bottom of stack; outputs of circuit).

gates.h, gates.c - Contains function that represent the possible gates in a graph representing a combinational circuit. Given inputs to any one of these gates, these functions will compute the output of the gate.

display.h, display.c - Contains functions used to print formatted simulation results to console and to file.

simulation.h, simulation.c - Contains functions used to (1) manipulate an input vector used during simulation and (2) apply an input vector used for simulation. Manipulation of input vectors primarily consists of replacing unknown values in the input vectors with known values. Application of input vectors may be completed on a faulty or fault-free circuit to generate an output vector read from the primary outputs of the circuit.

fault_simulation.h, fault_simulation.c - Contains functions used to control simulation of faulty circuits and to cast results of simulations into standardized forms. These functions largely act as a middle-man between the code in 'main.c' and the code in 'simulation.c' and provide data in a format that is acceptable by the functions in 'display.c'.
