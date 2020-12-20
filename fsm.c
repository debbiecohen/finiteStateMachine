#include <stdio.h>
#include "helperFunctions.c"


int main(int argc, char *argv[]) {
    int count = CountLines(argv[1]); //Count lines
    if (count<1)                              //If couldn't open the file return 1
        return 1;
    char* filename = argv[1];                //The first command line argument will be the fsm definition
    int currentStates[count];                //Array of current states (1st elem in fsm definition). 1-50
    char inputs[count];                      //Array of inputs (2nd elem in fsm definition). 'a'-'z'
    int nextStates[count];                   //Array of next states (3rd elem in fsm definition). 1-50
    int dataLoaded= LoadDataFromFSMDef(filename, currentStates, inputs, nextStates);
    if (dataLoaded == 1)                     //If error loading data, return 1
        return 1;

    filename = argv[2];                       //The second command line argument will be the inputs file
    int success = ExecuteSteps(filename, count, currentStates, inputs, nextStates);
    return success;                           //If finished without errors will be 0, if there was an error 1
}

