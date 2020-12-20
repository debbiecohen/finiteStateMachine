#include <stdio.h>
#include "helperFunctions.c"

void testCountLines(char *filename, int expected){
    //test if is counting the number of lines in the file right
    printf("===========================testCountLines=========================\n");
    int actual = CountLines(filename);
    printf("%s: Name of file: %s, expected=%d, actual=%d\n",
           (expected == actual ? "PASSED" : "FAILED"),
           filename, expected, actual);

}

void testLoadDataFromFSMDef(){
    //check if data is being loaded in the correct way
    printf("=======================testLoadDataFromFSMDef=====================\n");
    int currentStates[3];                //Array of current states (1st elem in fsm definition). 1-50
    char inputs[3];                      //Array of inputs (2nd elem in fsm definition). 'a'-'z'
    int nextStates[3];                   //Array of next states (3rd elem in fsm definition). 1-50
    LoadDataFromFSMDef("testLoadData.fsm", currentStates, inputs, nextStates);
    int curLoaded = 1;
    int inputsLoaded = 1;
    int nextLoaded = 1;
    int expectedCur[3] = {0,1,2};
    char expectedInputs[3] = {'a','b','c'};
    int expectedNext[3] = {1,2,3};
    for (int i=0; i<3; i++) {
        if (currentStates[i] != expectedCur[i])
            curLoaded = 0;
        if (inputs[i] != expectedInputs[i])
            inputsLoaded = 0;
        if (nextStates[i] != expectedNext[i])
            nextLoaded = 0;
    }
    printf("%s: Name of file: testLoadData.fsm, the expected data was loaded in the 3 arrays\n",
           (curLoaded && inputsLoaded && nextLoaded ? "PASSED" : "FAILED"));
}

void testLoadDataFromFSMDefRetValue(char *filename, int expected) {
    //check if returns right the 0 or 1
    printf("==================testLoadDataFromFSMDefRetValue==================\n");
    int currentStates[3];                //Array of current states (1st elem in fsm definition). 1-50
    char inputs[3];                      //Array of inputs (2nd elem in fsm definition). 'a'-'z'
    int nextStates[3];                   //Array of next states (3rd elem in fsm definition). 1-50
    int actual = LoadDataFromFSMDef(filename, currentStates, inputs, nextStates);
    printf("%s: Name of file: %s, expected=%d, actual=%d\n",
           (expected == actual ? "PASSED" : "FAILED"),
           filename, expected, actual);
}

void testExecuteSteps(char *filename, int expected){
    //check return value 0 if should succeed and 1 if there is an error
    printf("===========================testExecuteSteps=======================\n");
    int currentStates[16] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3};
    char inputs[16] = {'a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d'};
    int nextStates[16] = {1,1,2,3,0,2,2,3,0,1,2,3,0,1,0,0};
    int actual = ExecuteSteps(filename, 16, currentStates, inputs, nextStates);
    printf("%s: Name of file: %s, expected=%d, actual=%d\n",
           (expected == actual ? "PASSED" : "FAILED"),
           filename, expected, actual);

}


void testAll(){
    testCountLines("test1.fsm", 16);  //file has 16 lines
    testCountLines("noExistsFile.txt", -1); //error because the file doesnt exist
    testLoadDataFromFSMDef();  //is comparing the data being loaded in the arrays and the hardcoded data
    testLoadDataFromFSMDefRetValue("noExistsFile.fsm", 1); //error because file doesnt exist
    testLoadDataFromFSMDefRetValue("testLoadData.fsm", 0); //if the loading succeeds should return 0
    testLoadDataFromFSMDefRetValue("testLoadDataFormatError.fsm", 1); //Should fail because the file has an error in the format
    testLoadDataFromFSMDefRetValue("testLoadDataNo0State.fsm", 1); //no initial state
    testExecuteSteps("noExistsFile.inputs", 1); //error because file doesnt exist
    testExecuteSteps("test1Wrong.inputs", 1); //input a z that doesnt take to any state
    testExecuteSteps("test1WrongNoValidChar.inputs", 1); //error because input is not a char from a-z
    testExecuteSteps("test1.inputs", 0); //it should execute all the steps and succeed
}

int main() {
  printf("----------------------Testing Helper Functions----------------------\n");
  testAll();
}
