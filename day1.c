// Includes
#include <stdio.h>
#include <string.h>

// Macaronis :)
#define true 1
#define false 0
#define EMPTY -1 // Represents an empty array 
#define TERMINATION_CHARACTER '?' // Chosen to end the string of characters
#define MAX_LINE_LENGTH 100

// Checks the array n. If we found the first number, write value into the second. Else, write it in the first.
#define WRITE_N(n, value) ( ( (n[0]) == EMPTY ) ? ((n[0]) = (value)) : ((n[1]) = (value)) )


// Checks the numbers found and adds them onto sum accordingly 
int sum_calculation(int sum, int *n){

    // ! Do not try to optimize the conditions. These are necessary to ensure blank lines don't change the solution value
    // If we found only one number
    if(n[0] != EMPTY && n[1] == EMPTY){
        sum += n[0]*10 + n[0]; // The number "x" will be added as "xx" 
    } 
    
    // If we found two numbers 
    else if(n[0] != EMPTY && n[1] != EMPTY){
        sum += n[0]*10 + n[1]; // The numbers "x" and "y" will be added as "xy" 
    }

    return sum;
}


/* 
   Checks a string for numerical digits. Combines the first and last digit into a number and adds them onto a sum variable.
   Once done with a line, it moves onto the next and repeats the process till the TERMINATION_CHARACTER is found. 
*/
int day1_part1(){
    char c;

    int n[2] = {EMPTY, EMPTY}; // keeps track of the digits found
    int sum = 0;

    while((c=getchar()) != TERMINATION_CHARACTER){ 
        if(c>='0' && c<='9'){
            WRITE_N(n, c-'0');
        }

        if(c=='\n'){ // If we move onto a new string
            sum = sum_calculation(sum, n); // calculate the new sum

            // Reset the array n
            n[0] = EMPTY;
            n[1] = EMPTY;
        }
    }

    return sum;
}





// Checks from the current position in the sentence if a worded digit exists
const char *list_of_possible_digits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
const int list_of_number_length[] =     {3,      3,     5,       4,      4,      3,     5,       5,      4};

int check_sentence(char *reading_position, int remaining_characters){

    // Iterates through the list of possible digits
    for(int i=0;i<9;i++){

        // Checks if the length of the possible digit fits into the sentence
        if(list_of_number_length[i] <= remaining_characters){

            // Compare the next "x" characters in the sentence with the current possible digit, 
            // where x is the length of the current digit word*/
            if(strncmp(reading_position, list_of_possible_digits[i], list_of_number_length[i]) == 0){ // Found a digit? 
                return i+1; // returns the integer value of the digit found 
            }
        }
    }

    return -1; // No digits found
}



/* 
   Handles a sentence by iteratively checking if either a numerical or a worded digit exists. 
   If a numerical digit is found, it calls WRITE_N.
   If not, it checks for a worded digit by calling check_sentence()
*/
void handle_sentence(char *reading_position, int line_length, int *n){

    // Variables
    int remaining_characters; // Remaining characters in the sentence 
    int digit_found; // keeps track of the digit found
    char *starting_point = reading_position; // Set the starting point to the 0th character
    
    while(*reading_position){ // whilst not the terminator character
        if(*reading_position>='0' && *reading_position<='9') 
            WRITE_N(n, *reading_position-'0');
        else {
            remaining_characters = (line_length - (reading_position - starting_point)); // calculates the remaining characters in the line
            digit_found = check_sentence(reading_position, remaining_characters); // checks if the remainder of the sentence contains a worded digit

            (digit_found != -1) ? WRITE_N(n, digit_found) : 0; // If we found a digit (as in check_sentence did not return -1), write it into n
        }

        reading_position++; // move onto the next reading position
    }
}   


/* 
   Checks a string for numerical AND worded digits. Combines the first and last digit into a number and adds them onto a sum variable.
   Once done with a line, it moves onto the next and repeats the process till the TERMINATION_CHARACTER is found. 
*/
int day1_part2(){

    // --Variables-- //
    int n[2] = {EMPTY, EMPTY}; // Array to keep track of the first and last numbers found 
    char current_line[MAX_LINE_LENGTH+1]; // +1 for terminator character
    char *writing_position = current_line; // we will increment the pointer
    int current_line_length = 0;
    char c;

    int sum = 0;

    while((c = getchar()) != TERMINATION_CHARACTER){
        if(c=='\n'){
            *writing_position = '\0';
            handle_sentence(current_line, current_line_length, n); // handle the current sentence
            sum = sum_calculation(sum, n); // calculate the new sum

            // Reset indicators:
            writing_position = current_line; // writing position back to the zeroeth index
            current_line_length = 0;
            n[0] = EMPTY;
            n[1] = EMPTY;
        } else {
            *writing_position = c; // write the line into current_line
            current_line_length++; // increase line length
            writing_position++; // move onto the next element
        }
    }

    return sum; 
}
