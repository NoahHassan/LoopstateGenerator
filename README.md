# LoopstateGenerator
A code for generating random loop-state images

The algorithm works by generating a field of cells which each have bottom-, top-, left- and right link variables (0 or 1).
The cells each can be toggled, which changes the link-variables each cell posseses from 0 -> 1 and from 1 -> 0.
By randomly toggling cells in the field, we never violate the loop-condition thus creating random loopstates.

The data is saved in a .txt file and plotted using python.
