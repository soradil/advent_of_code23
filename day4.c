#include <stdio.h>


#define bool                    int
#define true                    1
#define false                   0
#define MAX_WINNING_NUMBERS     10
#define MAX_SCRATCHCARD_NUMBERS 25
#define MAX_NUM_GAMES           203

#define TERMINATION_CHARACTER   '?'

#define IS_DIGIT(n)             (((n) >= '0') && ((n) <= '9'))


void reset_array(int *a, int n){
    for(int i=0;i<n;++i){
        a[i] = 0;
    }
}


int nth_doubling(int n){
    if(n<1){
        return 0;
    }

    return 1 << (n-1); // shift the bits 000000001 n-1 times
}


int check_current_card(int *winning_numbers, int *scratchcard_numbers){
    int matching = 0;

    for(int i=0;i<MAX_SCRATCHCARD_NUMBERS;++i){
        for(int j=0;j<MAX_WINNING_NUMBERS;++j){
            if(scratchcard_numbers[i] == winning_numbers[j]) {
                matching++;
            }    
        }
    }

    return matching;
}

int day4_part1(){
    
    int winning_array[MAX_WINNING_NUMBERS];
    int *winning_ptr = winning_array;
    int scratchcard_array[MAX_SCRATCHCARD_NUMBERS];
    int *scratchcard_ptr = scratchcard_array;

    int sum = 0;
    int current_number_read = 0;    
    
    bool has_seen_id = false; // used to check if the Card ID has been passed by
    bool has_seen_bar = false; // used to check if the vertical bar "|" has been passed by


    char c;
    while((c=getchar()) != TERMINATION_CHARACTER){
        if(c=='\n'){
            *scratchcard_ptr = current_number_read;
            current_number_read = 0;
            
            
            sum += nth_doubling(check_current_card(winning_array, scratchcard_array));
            
            reset_array(winning_array, MAX_WINNING_NUMBERS);
            reset_array(scratchcard_array, MAX_SCRATCHCARD_NUMBERS);

            winning_ptr = winning_array;
            scratchcard_ptr = scratchcard_array;
            has_seen_bar = false; // used to check if the vertical bar "|" has been passed by
            has_seen_id = false; // used to check if the Card ID has been passed by


        } else{
            
            if(c==':'){
                has_seen_id = true;
            } else if(has_seen_id){
                if(IS_DIGIT(c)){
                    current_number_read *= 10; // add a zero to the right of the digit (remember we are reading from left to right)
                    current_number_read += (c-'0'); // add the new digit
                } 
                
                // If c is an empty space
                // The current_number_read != 0 check is necessarry 
                // as otherwise it would result in registering the empty space afterwards "Card ID: "
                else if(c==' ' && current_number_read != 0){
                    if (has_seen_bar == false) { // save into winning_number array
                        *winning_ptr = current_number_read; 
                        winning_ptr++;
                    } else { // save into scratchard array
                        *scratchcard_ptr = current_number_read;
                        scratchcard_ptr++;
                    }

                current_number_read = 0;  // Reset current_number_read after both branches
                } else if(c=='|'){
                    has_seen_bar = true;
                } 
            }

        }
    }

    return sum;
}


/*
    This function counts the number of cards we have at the end given an array of matches
    For example [4, 2, 2, 1, 0] would result in at the start 5 cards
    Then the first card will make 4 more. We now have 9 cards
    Then the second card will make 2 more. We now have 11 cards
    And the second card (copy which we made with the first card) will make 2 more. We now have 13 cards.
    And so on...
*/
int copies_count(int *match_array){
    int sum = 0;

    // Make an array to store all the scratchards we have so far
    int copies[MAX_NUM_GAMES];

    // We start with at least 1 of each card
    for(int i=0;i<MAX_NUM_GAMES;++i){
        copies[i] = 1;
    }

    // Iterate through all the scratchards
    for(int i=0;i<MAX_NUM_GAMES;++i){

        // Starting from the next card, we will keep jumping until the number of jump count has been completed or we reach the end of the array
        for(int j=i+1; j<(i+1+match_array[i]) && j<MAX_NUM_GAMES; ++j){
            // Add copies[i] (jumped from) onto copies[j] (jumped to) 
            copies[j] += copies[i];
        }
    }

    // Sum the array
    for(int i=0;i<MAX_NUM_GAMES;++i){
        sum += copies[i];
    }

    return sum;
}


/*
    Similar to part1 except we calculate matches in an array and call onto copies_count() at the end  
*/
int day4_part2(){
    int winning_array[MAX_WINNING_NUMBERS];
    int *winning_ptr = winning_array;
    int scratchcard_array[MAX_SCRATCHCARD_NUMBERS];
    int *scratchcard_ptr = scratchcard_array;

    int match_array[MAX_NUM_GAMES]; // array to store the matches of games
    int *match_ptr = match_array;

    int current_number_read = 0;    
    
    bool has_seen_id = false; // used to check if the Card ID has been passed by
    bool has_seen_bar = false; // used to check if the vertical bar "|" has been passed by


    char c;
    while((c=getchar()) != TERMINATION_CHARACTER){
        if(c=='\n'){
            *scratchcard_ptr = current_number_read;
            current_number_read = 0;

            *match_ptr = check_current_card(winning_array, scratchcard_array);
            match_ptr++; 
                        
            reset_array(winning_array, MAX_WINNING_NUMBERS);
            reset_array(scratchcard_array, MAX_SCRATCHCARD_NUMBERS);

            winning_ptr = winning_array;
            scratchcard_ptr = scratchcard_array;
            has_seen_bar = false; // used to check if the vertical bar "|" has been passed by
            has_seen_id = false; // used to check if the Card ID has been passed by


        } else{
            
            if(c==':'){
                has_seen_id = true;
            } else if(has_seen_id){
                if(IS_DIGIT(c)){
                    current_number_read *= 10; // add a zero to the right of the digit (remember we are reading from left to right)
                    current_number_read += (c-'0'); // add the new digit
                } 
                
                // If c is an empty space
                // The current_number_read != 0 check is necessarry 
                // as otherwise it would result in registering the empty space afterwards "Card ID: "
                else if(c==' ' && current_number_read != 0){
                    if (has_seen_bar == false) { // save into winning_number array
                        *winning_ptr = current_number_read; 
                        winning_ptr++;
                    } else { // save into the scratchard array
                        *scratchcard_ptr = current_number_read;
                        scratchcard_ptr++;
                    }

                current_number_read = 0;  // Reset current_number_read after both branches
                } else if(c=='|'){
                    has_seen_bar = true;
                } 
            }

        }
    }


    return copies_count(match_array);
}
