// --Macaronis-- //
// Refers to the right and left elements of the array n
#define RIGHT                  1
#define LEFT                   0

#define MAX_ROWS              140
#define MAX_COLS              140
#define TERMINATION_CHARACTER '?'

// Is the character n a digit?
#define IS_DIGIT(n)     (((n) >= '0') && ((n) <= '9'))


// -- Actual code starts here -- //
// Resets all the elements of array a with n elements to 0
void reset_array(int *a, int n){
    for(int i=0;i<n;++i){
        a[i] = 0;
    }
}


// Returns 10 to the power of "exp"
int pow_10(int exp){
    int result = 1;
    while(exp>0){
        result *= 10;
        exp--;}
    return result;
}


// Write the user input into the schematic array
void write_schematic(char schematic[MAX_ROWS][MAX_COLS+1]){
    char *ptr = schematic[0];
    char c;
    while((c=getchar()) != TERMINATION_CHARACTER){
        if(c!='\n'){
            *ptr = c;
            ptr++;
        } else {
            *ptr = '\0';
            ptr++;
        }            
    }
}


/* 
    Checks every character to the right of position [row][col] till it finds either a period or the end of the array 
    The function writes its answer into n[RIGHT]                                                                     
*/
void right_check(char schematic[MAX_ROWS][MAX_COLS+1], int row, int col, int n[2]){

    char current_character;
    
    // Look right. i=1 because we want to iterate from our position exclusive
    for(int i=1;i<MAX_COLS;++i){
        if((col+i) <= (MAX_COLS-1)){ // check if the shift is in-range
            current_character = schematic[row][col+i];
            if(IS_DIGIT(current_character)){
                n[RIGHT] *= 10; // add a zero to the right of the digit (remember we are reading from left to right)
                n[RIGHT] += (current_character-'0'); // add the new digit
            } else {
                break;
            }
        } else {
            break;
        }
    }

}


/* 
    Checks every character to the left of position [row][col] till it finds either a period or the end of the array 
    The function writes its answer into n[LEFT]                                                                     
*/
void left_check(char schematic[MAX_ROWS][MAX_COLS+1], int row, int col, int n[2]){
    char current_character;

    // Look left. i=1 since we want to iterate from our position exclusive
    for(int i=1;i<MAX_COLS;++i){
        if((col-i) >= 0){ // check if the shift is in-range
            current_character = schematic[row][col-i];
            if(IS_DIGIT(current_character)){

                // This technique takes advantage of the properties of powers.
                // F.e: As we are reading from right to left, the number 245 will appear 5, 4, 2
                // 5 * 10^0 = 5
                // 4 * 10^1 = 40
                // 2 * 10^2 = 200
                n[LEFT] += (current_character-'0') * (pow_10(i-1)); 
            } else {
                break;
            }
        } else {
            break;
        }
    }

}


/*
    This upward check is performed by first ensuring that there is a row above you.
    We keep moving left of this up row till we reach the end of the array or find a period.
    We then save this as our current character and read from that shifted position to the right.
*/
void upward_check(char schematic[MAX_ROWS][MAX_COLS+1], int row, int col, int n[2]){

    char current_character;
    int i_col = col; // initial column

    // Look up
    if((row-1) >= 0){
        current_character = schematic[row-1][col];
        
        // While we haven't reached the end of the current line or a period,
        // we will keep shifting to the left to find all the possible digits
        // of the value above us
        // F.e: 
        /*
            3 4 5 .
            . . # .
        */
       // We will keep iterating backwards through the line above till we reach 3, where we can finally read the line normally.
       // This also works with a number that is shifted to the right
       // F.e: 
        /*
            . . 3 4 5
            . . # . .
        */
        while((col-1) >= 0 && IS_DIGIT(current_character)){
            col -= 1;
            current_character = schematic[row-1][col];
        }


        // A check to the right is done no matter what to capsulate a possible right diagonal like so:
        /*
            . 3 4 5
            # . . .
        */
        // Or to read from the current shifted position performed by the previous while loop
        /*
            Current position -> 3 4 5 .
                                . . # .
        */
        right_check(schematic, row-1, col, n);
        
        // If the initial column (vertically above the special symbol) is equal to the current column,
        // that means that we have a period above us verticallly, and we must check the left as well
        // for the chance of diagonal numbers such as "x . y" 
        if(i_col == col){ 
             left_check(schematic, row-1, col, n);
        }
        
    }
    
}


/*
    This downwards check is performed by first ensuring that there is a row below you.
    We keep moving left of this down row till we reach the end of the array or find a period.
    We then save this as our current character and read from that shifted position to the right.

    This function is identical to the upward_check, except with a different limit check and with row+1 instead of row-1.
    All other explanations from upward_check apply here.
*/
void downward_check(char schematic[MAX_ROWS][MAX_COLS+1], int row, int col, int n[2]){
    
    char current_character;
    int i_col = col;
    
    // Look down
    if((row+1) < MAX_ROWS){
        current_character = schematic[row+1][col];

        while((col-1) >= 0 && IS_DIGIT(current_character)){
            col -= 1;
            current_character = schematic[row+1][col];
        }

        right_check(schematic, row+1, col, n);
        if(i_col == col){
            left_check(schematic, row+1, col, n);
        }
    }

}


/*
    For part 1 of the challenge.
    Iterates through the schematic produced by write_schematic until it finds a special character.
    It then checks left, right, up and down for any values and adds it onto sum. Returns sum.
*/
long read_schematic1(char schematic[MAX_ROWS][MAX_COLS+1]){
    char current_character;
    long sum = 0;
    int n[2] = {0}; // saves values returned per check call

    // Iterates through the schematic
    for(int i=0;i<MAX_ROWS;++i){
        for(int j=0;j<MAX_COLS;++j){
            current_character = schematic[i][j];


            // Finds a speciail character (for example: /, #, *, etc)
            if(!IS_DIGIT(current_character) && current_character != '.'){
                
                right_check(schematic, i, j, n);
                left_check(schematic, i, j, n);
                SUM_ARRAY();
                reset_array(n, 2); // reset the array as it will be used by upward_check

                upward_check(schematic, i, j, n);
                SUM_ARRAY();
                reset_array(n, 2); // reset the array as it will be used by downward_check

                downward_check(schematic, i, j, n);
                SUM_ARRAY();
                reset_array(n, 2); // reset the array as it will be used by the right and left check next iteration
            }
        }
    }

    return sum;
}


/*
    For part 2 of the challenge.
    Iterates through the schematic produced by write_schematic until it finds a special character.
    It then checks left, right, up and down for any values and saves them into the array m.
    m is then iterated through to find exactly two non-zero characters which are multiplied and added onto the sum.
    If there are more than two or less than two non-zero characters, we should not add them onto the sum 
*/
long read_schematic2(char schematic[MAX_ROWS][MAX_COLS+1]){
    char current_character;
    long sum = 0;

    int n[2] = {0};  // saves returned values per check call
    int m[6] = {0};  // keeps track of returned values of n
    int non_zero_elements = 0; // keeps track of non zero elements in m
    long b = 0; // buffer

    // Iterates through the schematic
    for(int i=0;i<MAX_ROWS;++i){
        for(int j=0;j<MAX_COLS;++j){
            current_character = schematic[i][j];


            // Finds a speciail character (for example: /, #, *, etc)
            if(!IS_DIGIT(current_character) && current_character != '.'){
                
                right_check(schematic, i, j, n);
                left_check(schematic, i, j, n);

                m[0] = n[LEFT];
                m[1] = n[RIGHT];

                reset_array(n, 2);

                upward_check(schematic, i, j, n);
    
                m[2] = n[LEFT];
                m[3] = n[RIGHT];
                reset_array(n, 2);

                downward_check(schematic, i, j, n);
                m[4] = n[LEFT];
                m[5] = n[RIGHT];
                reset_array(n, 2);

                // Iterate through m
                for(int i=0;i<6;++i){
                    if(non_zero_elements == 0 && m[i] != 0) {
                        non_zero_elements++;
                        b = m[i];
                    } else if(non_zero_elements == 1 && m[i] != 0) {
                        b = b * m[i];
                        non_zero_elements++;
                    } else if(m[i] != 0){
                        non_zero_elements++;
                        break;
                    }
                }

                // If exactly two elements are non-zero
                if(non_zero_elements == 2){
                    sum += b;
                }

                // Reset for next iteration
                non_zero_elements = 0;
                reset_array(m, 6);
            }
        }
    }

    return sum;
}


long day3_part1(){
    char engine_schematic[MAX_ROWS][MAX_COLS+1];

    write_schematic(engine_schematic);

    return read_schematic1(engine_schematic);
}


long day3_part2(){
    char engine_schematic[MAX_ROWS][MAX_COLS+1];

    write_schematic(engine_schematic);

    return read_schematic2(engine_schematic);
}
