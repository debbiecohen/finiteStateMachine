#include <stdio.h>

int CountLines(char *filename){
    /* returns the number of lines in the fsm definition,
     * this will allow us to know the sizes of the arrays
     * that will hold the data. If couldn't open the file
     * returns -1*/
    FILE *fp = fopen(filename, "r");
    int ch;
    int count = 0;
    if (fp == NULL){
        printf("ERROR: Could not open file %s\n",filename);
        return -1;
    }
    count++;
    while ((ch = fgetc(fp)) != EOF){
        if (ch=='\n')
            count++;
    }
    fclose(fp);
    return count;
}


int LoadDataFromFSMDef(char* filename, int currentStates[], char inputs[], int nextStates[]){
    /* Will fill out 3 arrays with the data provided by the
     * fsm definition. Returns 0 if success and 1 if error.*/
    //variables to read through file...
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("ERROR: Could not open file %s\n",filename);
        return 1;
    }

    int isZeroThere = 0;                                        //Checks if  there is an initial state 0
    int index = 0;
    int cur;                                                    //Holds value of current state in the line.
    char input;                                                 //Holds value of input in the line. character from a-z
    int next;                                                   //Holds value of next in the line.
    int match;                                                  //Checks if the line matches the expected format
    while ((match = fscanf(fp, "%d:%c>%d", &cur, &input,&next)) != EOF){
        if (match && input>='a' && input<='z'){                 //check also if the input is a char between a-z
            currentStates[index]=cur;
            if (currentStates[index]==0)
                isZeroThere=1;
            inputs[index]=input;
            nextStates[index]=next;
            index++;
        }
        else{
            printf("ERROR: the format of fsm definition has an error, should be cur:input>next, "
                   "or invalid character used as input\n");
            return 1;
        }
    }
    fclose(fp); //close the .fsm file

    if (isZeroThere == 0) {                  //If there is no initial state, ERROR
        printf("ERROR: there isn't a state 0 in your FSM\n");
        return 1;
    }
    printf("processing FSM definition file %s\n", filename);
    printf("FSM has %d transitions\n", CountLines(filename));
    return 0;
}

int ExecuteSteps(char * filename, int count, int currentStates[], char inputs[], int nextStates[]) {
    /* Will print to stdout all the transitions that were made
     * depending of the inputs provided by the input file */
    FILE *fp = fopen(filename, "r");
    int step = 0;                                 //This will count the number of transitions
    int cur = 0;                                  //The initial state will always be 0
    int next;                                     //Will hold the next state to go
    int haveNext;                                 //Since we might get an input that does nothing on the current state

    if (fp == NULL) {
        printf("ERROR: Could not open file %s\n", filename);
        return 1;
    }

    printf("processing FSM inputs file %s\n", filename);
    char input;
    int match;
    while ((match = fscanf(fp, "%c\n", &input)) != EOF) {    //Loop while we haven't reach end of file
        if (match) {                                         //Check if line matches expected format (single char)
            haveNext = 0;
            for (int i = 0; i < count; i++) {                //Check if in the current state, the input is valid
                if (currentStates[i] == cur && inputs[i] == input) {
                    next = nextStates[i];
                    haveNext = 1;
                    break;
                }
            }

            if (haveNext == 0) {                             //If the input doesnt lead to anything, ERROR
                printf("ERROR: Invalid input: %c at step %d\n", input, step);
                return 1;
            }

            printf("\tat step %d input %c transitions FSM from state %d to state %d\n", step, input, cur, next);
            cur = next;
            step += 1;
        } else {
            printf("ERROR: Invalid input, it is not a single char");
            return 1;
        }
    }
    printf("after %d steps, state machine finished successfully at state %d\n", step, cur);
    fclose(fp);
    return 0; //if it was a success, return 0
}
