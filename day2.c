// Includes
#include <stdio.h>
#include <string.h>

// Macaronis
#define true                  1
#define false                 0
#define MAX_GAMES             100
#define MAX_LINE_LENGTH       150
#define MAX_RED_CUBES         12
#define MAX_GREEN_CUBES       13
#define MAX_BLUE_CUBES        14
#define TERMINATION_CHARACTER '?'

// --- Parameterized Macros --- //
#define IS_GAME_POSSIBLE(r, g, b) (((r)<=MAX_RED_CUBES) && ((g)<=MAX_GREEN_CUBES) && ((b)<=MAX_BLUE_CUBES))


// Iterates through an array made up of ids. Returns the sum of its elements
int get_id_sum(int *id){
    int sum = 0;
    while(*id){
        sum += *id;
        id++;
    }
    return sum;
}


// Zeros all the values passed into ptr_array
void zero_ptrvalue(int num_ptr, int **ptr_array){
    for(int i=0;i<num_ptr;++i){
        if(ptr_array[i] != NULL){
            *(ptr_array[i]) = 0;
        }
    }
}


/*
    As we move through the characters of the string input, we first check the id and save it in game_id (will be removed later if game is unsuccessful)
    Then we move through each set and check if they do not fail our requirement of being below a certain count. Otherwise the game will be considered illegal and the id wont be saved
    If the game is legal/possible given our maximum number of cubes of each color, we will save it into game_id and will iterate through it at the end of the code to get the sum.
*/
int day2_part1(){
    int game_id[MAX_GAMES]; // stores successful games
    int *current_id = game_id; // to be iterated if a game is succesful. Otherwise, leave the same

    int current_number = 0; // Used to store the current number being read
    int is_game_possible = true; // is the current game possible?
    int in_word = false; // indicator to check if we are currently in a word (specifically, a color like red)

    // Counters for the number of cubes of each respective color
    int red_cubes = 0;
    int green_cubes = 0;
    int blue_cubes = 0;


    char c;
    while((c=getchar()) != TERMINATION_CHARACTER){

        // If we have a new line
        if(c=='\n'){

            // We check if all sets in the game have been possible so far (is_game_possible)
            // Then we check if the last set is possible
            // This is necessary because the last set in the game does not have a semi-colon at its end 
            if(is_game_possible && IS_GAME_POSSIBLE(red_cubes, green_cubes, blue_cubes)){
                current_id++; // move onto the next id element 
            } else {
                *current_id = 0; // erase the current game id as it is impossible
            }

            // Resets value
            zero_ptrvalue(5 ,(int *[]){&current_number, &red_cubes, &green_cubes, &blue_cubes, &in_word});
            is_game_possible = true;
        } 

        // If we do not have a new line, we first must check if the game is possible
        else if(is_game_possible){
            switch (c)
            {
            case ':': // this comes after the game id and acts as an indicator to save the id into the game_id array 
                *current_id = current_number; // save the game id (represented by current_number) into the game_id
                current_number = 0; // reset current_number
                break;
            
            case ';': // this comes after a set is completed and acts as an indicator to check for the possibility of the current set
                is_game_possible = IS_GAME_POSSIBLE(red_cubes, green_cubes, blue_cubes);
                
                // Reset cube count for the next set and
                // Since a semi-colon comes after a word. And since we are no longer in a word, we can reset the "in_word" indicator
                zero_ptrvalue(4, (int *[]){&red_cubes, &green_cubes, &blue_cubes, &in_word});
                break;

            case ',':
                // A comma comes after a word (like red). And since we are no longer in a word, we reset the "in_word" indicator 
                in_word = false;
                break;

            // cases for colors
            case 'r': // r for red
                // if we aren't already in a word (this is necessary as green as the letter r in it)
                if(!in_word){
                    red_cubes += current_number;
                    current_number = 0;
                    in_word = true;
                }
                break;
            case 'g': // g for green
                if(!in_word){
                    green_cubes += current_number;
                    current_number = 0;
                    in_word = true;
                }
                break;
            case 'b': // b for blue
                if(!in_word){
                    blue_cubes += current_number;
                    current_number = 0;
                    in_word = true;
                }
                break;
            
            // Not any of the above
            default:
                // If it is a digit
                if(c>='0' && c<='9'){
                    current_number *= 10; // add a zero to the right of the digit (remember we are reading from left to right)
                    current_number += (c-'0'); // add the new digit
                } 
                break;
            } 
        } 
    }

    return get_id_sum(game_id);
}



/*
    As we move through the characters of the input string, we will check for the highest value ball. This will be saved into its respective counter.
    We will then save the product of the three counters into game_id. Each index in game_id corresponds to the product produced at that game.
    At the end, we add the sum of all the products. Many of the checks and necessary steps required in part1 aren't required here :) 
*/
int day2_part2(){
    int game_id[MAX_GAMES]; // stores successful games
    int *current_id = game_id; // to be iterated if a game is succesful. Otherwise, leave the same

    int current_number = 0; // Used to store the current number being read
    int in_word = false; // indicator to check if we are currently in a word (specifically, a color like red)

    // Counters for the highest count of each color passed by
    int highest_red = 0;
    int highest_green = 0;
    int highest_blue = 0;


    char c;
    while((c=getchar()) != TERMINATION_CHARACTER){
        switch (c)
        {
            case '\n':
                *current_id = highest_red*highest_green*highest_blue; // save the product the three greatest red, green and blue count 
                current_id++; // move onto the next id

                // reset the value of these integers
                zero_ptrvalue(5 ,(int *[]){&current_number, &highest_red, &highest_green, &highest_blue, &in_word});

            case ':': // this comes after the game id and acts as an indicator to save the id into the game_id array 
                *current_id = current_number; // save the game id (represented by current_number) into the game_id
                current_number = 0; // reset current_number
                break;
            
            case ',':
            case ';':
                // A semi-colon/comma comes after a word. And since we are no longer in a word, we can reset the "in_word" indicator
                in_word = false;
                break;

            // cases for colors
            case 'r': // r for red
                // if we aren't already in a word (this is necessary as green as the letter r in it)
                if(!in_word){
                    if(current_number>highest_red) highest_red = current_number;  // update highest_red if the current number is greater
                    current_number = 0;
                    in_word = true;
                }
                break;
            case 'g': // g for green
                if(!in_word){
                    if(current_number>highest_green) highest_green = current_number; // update highest_green if the current number is greater
                    current_number = 0;
                    in_word = true;
                }
                break;
            case 'b': // b for blue
                if(!in_word){
                    if(current_number>highest_blue) highest_blue = current_number; // update highest_blue if the current number is greater
                    current_number = 0;
                    in_word = true;
                }
                break;
            
            // Not any of the above
            default:
                // If it is a digit
                if(c>='0' && c<='9'){
                    current_number *= 10; // add a zero to the right of the digit (remember we are reading from left to right)
                    current_number += (c-'0'); // add the new digit
                } 
                break;
        } 
        
    }

    return get_id_sum(game_id);
}
