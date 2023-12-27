#include <stdio.h>
#include <stdlib.h>

size_t day6_part1(){

    FILE *input = fopen("day6_input.txt", "r");
    if (input == NULL) {
        printf("Failed to open the input file\n");
        return EXIT_FAILURE;
    }


    int current_time;
    int current_distance;

    int p1;
    int p2;

    int solutions_count;
    size_t answer = 0;
    char c;
    while((c=fgetc(input)) != EOF){

        if(fscanf(input, "%d, %d", &current_time, &current_distance) == 2){

            p1 = 1;
            p2 = current_time - 1;
            while(p1 != p2){

                // distance = speed * time
                if(p1 * (current_time - p1) > current_distance && p2 * (current_time - p2) > current_distance){
                    break;
                }

                
                if(p1 * (current_time - p1) <= current_distance){
                    p1++;
                }

                if(p2 * (current_time - p2) <= current_distance){
                    p2--;
                }

            }

            if(answer){
                answer *= p2 - p1 + 1;
            } else {
                answer = p2 - p1 + 1;
            }
        } 

    }

    free(input);
    return answer;
}

size_t day6_part2(){

    FILE *input = fopen("day6_input2.txt", "r");
    if (input == NULL) {
        printf("Failed to open the input file\n");
        return EXIT_FAILURE;
    }


    size_t current_time;
    size_t current_distance;

    size_t p1;
    size_t p2;

    size_t solutions_count;
    size_t answer = 0;
    char c;
    while((c=fgetc(input)) != EOF){

        if(fscanf(input, "%ld, %ld", &current_time, &current_distance) == 2){

            p1 = 1;
            p2 = current_time - 1;
            while(p1 != p2){

                // distance = speed * time
                if(p1 * (current_time - p1) > current_distance && p2 * (current_time - p2) > current_distance){
                    break;
                }

                
                if(p1 * (current_time - p1) <= current_distance){
                    p1++;
                }

                if(p2 * (current_time - p2) <= current_distance){
                    p2--;
                }

            }

            if(answer){
                answer *= p2 - p1 + 1;
            } else {
                answer = p2 - p1 + 1;
            }
        } 

    }

    free(input);
    return answer;
}
