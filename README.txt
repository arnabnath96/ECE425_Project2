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

'project netlist_filename input_filename xval output_filename'

where netlist_filename should be replaced with the name of the file containing the circuit description, input_filename should be replaced with the name of the file containing the input vectors, xval should be repalced by the value used in place of unkown values in the input vectors, and output_filename should be replaced with the name of the file that the output vectors should be written to.

==============================================================================
BREAKDOWN OF PROJECT
==============================================================================

For ease of development, the code for this project has been split across a number of source files. Briefly, these files include:

main.c - Main exectuable unit. Contains code that reads program parameters from the command line and calls functions that read in a circuit description, read in a set of input vectors, applies the input vectors to the circuit, and logs resulting output vectors.

graph.h, graph.c - Contains definitions of a linked list, graph node, and input vector and functions that interact with the structures. Additionally containts a number of constants. 

stack.h, stack.c - Contains definitions of a simple stack node and a simple stack capable of storing integers. The stack is capable of push, pop, print, construction, copy, and destruction operations and also includes an emptiness check.

topological_sort.h, topological_sort.c - Contains functions used to sort a graph representing a combinational circuit in topological order. The output of the topological sort operation is a stack of indetifiers for nodes in the graph ordered from first nodes in the topological sort (top of stack; inputs of circuit) to last nodes in the topological sort (bottom of stack; outputs of circuit).

gates.h, gates.c - Contains function that represent the possible gates in a graph representing a combinational circuit. Given inputs to any one of these gates, these functions will compute the output of the gate.

simulation.h, simulation.c - Contains functions used to (1) manipulate an input vector used during simulation and (2) apply an input vector used for simulation. Manipulation of input vectors primarily consists of replacing unknown values in the input vectors with known values. Application of input vectors generates an output vector read from the primary outputs of the circuit.

==============================================================================
NOTES ON GATES.C
==============================================================================

Two different implementations of the gates.c source code have been provided in this project that provide equivalent outputs but utilize different computations for each function contained. These two implementations are:

gates_table.c - Implements each of the multi-input gates in terms of a set of look-up tables for a set of two-input gates.

gates_func.c - Implements each of the multi-input gates in terms of a behavioral description of the gate.

A particular implementation can be chosen by copying the file and renaming the copied file as graph.c.

==============================================================================
NOTES ON GATES.C (XOR AND XNOR)
==============================================================================

There is no common definition for multiple input XOR gates and XNOR gates in the literature, so the following definition was chosen for this project:

For multiple inputs, XOR gates and XNOR gates serve as odd-parity generators and even-parity generators, respectively.

==============================================================================
NOTES ON TOPOLOGICAL_SORT.C
==============================================================================

Since the netlists for the test circuits are provided in topological order, the topological sort provided by topological_sort.c is not necessary for simulation. This code was written by accident but was not removed since it would require significant refactor time and would limit the overall robustness of the final solution.
