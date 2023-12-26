#include <stdio.h>
#include <stdlib.h>


// Macronis
#define IS_DIGIT(N)             (((N)>='0') && ((N)<='9'))
#define true                    1
#define false                   0
#define RESET_CHARACTER         'r'

typedef struct {
    size_t n; // size of vec
    size_t *p; 
    size_t element_size;
} vec;


void clear_array(vec *a, size_t n){
    for(int i=0;i<n;++i){
        (*a).p[i] = false;
    }
}


void make_vec(vec *v, size_t element_size){
    (*v).p =  malloc(element_size);
    if((*v).p == NULL){
        printf("Failed to allocate memory in make_array\n");
        exit(EXIT_FAILURE);
    }

    (*v).element_size = element_size;
    (*v).n = 0; // no elements currently in the vector
}

void extend_vec(vec *v, size_t i){ 
    if((*v).n == 0){ // if the vector is only initialized
        (*v).p[0] = i;
        (*v).n += 1; // 1 element in the vector
    } else { // else if the vector already has pre-existing elements
        (*v).n += 1;
        (*v).p = realloc((*v).p, ((*v).n)*(*v).element_size);
        if((*v).p==NULL){
            printf("Failed to allocate memory in extend_array\n");
            exit(EXIT_FAILURE);
        }

        (*v).p[(*v).n - 1] = i;
    }
}


size_t day5_part1(){

    // Check if file opened correctly    
    FILE *seed = fopen("day5_seed.txt", "r");
    if (seed == NULL) {
        printf("Failed to open the seed file\n");
        return EXIT_FAILURE;
    }

    vec seed_vec;
    make_vec(&seed_vec, sizeof(size_t));


    // Read the digits
    size_t read_digit;
    while (fscanf(seed, "%ld", &read_digit) == 1) {
        extend_vec(&seed_vec, read_digit);
    }

    FILE *map = fopen("day5_map.txt", "r");
    if(map == NULL){
        printf("Failed to open map file\n");
        return EXIT_FAILURE;
    }

    // This keeps track of whether or not we altered this current number in a "set" of ranges 
    // So that we do not change the same range of values twice in a corresponding map conversion 
    vec alt;
    make_vec(&alt, sizeof(size_t));

    for(int i=0;i<seed_vec.n;++i){
        extend_vec(&alt, false);
    }
 
    
    char c;
    size_t destination, source, range; 
    while((c=fgetc(map)) != EOF){
        if(IS_DIGIT(c)){ // if fgetc consumed the first character and that character is a digit, send it back to the stream
            ungetc(c, map);
        }

        if(c==RESET_CHARACTER){
            clear_array(&alt, seed_vec.n);
        }


        // fscanf "consumes" the characters from the file stream
        fscanf(map, "%ld %ld %ld", &destination, &source, &range);
        
        
        size_t source_range_diff;
        for(int i=0;i<seed_vec.n;++i){

            // if current seed has not been altered and it is between the destination range
            if(alt.p[i] == false && seed_vec.p[i] >= source && seed_vec.p[i] < source+range){
                alt.p[i] = true; // this point has been altered already and should not be altered again 
                source_range_diff = seed_vec.p[i] - source;
                seed_vec.p[i] = destination + source_range_diff;
            } 
        }


    }

    
    size_t smallest = seed_vec.p[0];
    for(int i=1;i<seed_vec.n;++i){
        if(seed_vec.p[i] < smallest){
            smallest = seed_vec.p[i];
        }
    
    }


    free(seed_vec.p);   
    free(alt.p);

    fclose(seed);
    fclose(map);

    return smallest;
}



size_t day5_part2(){

    // Check if file opened correctly    
    FILE *seed = fopen("day5_seed.txt", "r");
    if (seed == NULL) {
        printf("Failed to open the seed file\n");
        return EXIT_FAILURE;
    }

    vec seed_vec;
    vec seed_range;
    make_vec(&seed_vec, sizeof(size_t));
    make_vec(&seed_range, sizeof(size_t));


    // Read the seeds
    size_t read_seed_position, read_seed_range;
    while (fscanf(seed, "%ld %ld", &read_seed_position, &read_seed_range) == 2) {
        extend_vec(&seed_vec, read_seed_position);
        extend_vec(&seed_range, read_seed_range);
    }

    // Open the map
    FILE *map = fopen("day5_map.txt", "r");
    if(map == NULL){
        printf("Failed to open map file\n");
        return EXIT_FAILURE;
    }


    // Used to store alterations
    vec alt;
    make_vec(&alt, sizeof(size_t));

    for(int i=0;i<seed_vec.n;++i){
        extend_vec(&alt, false);
    }
    

    char c;
    size_t destination, source, range; 
    while((c=fgetc(map)) != EOF){
        if(IS_DIGIT(c)){ // if fgetc consumed the first character and that character is a digit, send it back to the stream
            ungetc(c, map);
        }

        if(c==RESET_CHARACTER){
            clear_array(&alt, alt.n);
        }


        // fscanf "consumes" the characters from the file stream
        fscanf(map, "%ld %ld %ld", &destination, &source, &range);
        
        
        int i = 0;
        size_t splice_pos;
        size_t source_range_diff;

        // Iterate through each seed
        while(i<seed_vec.n){

            /*
                The following if statements describe the three different cases in which we must split our seed range

                The first if statement:

                The second if statement:
                If our splitting list covers the seeds partially, excluding the first seed
                F.e: seed list [1,2,3,4]
                     splitting list [3, 4, 5] -> Outcome: [1, 2] [3, 4]
                    
                F.e: seed list [1,2,3,4]
                     splitting list [3, 4] -> Outcome: [1, 2] [3, 4]

                F.e: seed list [1, 2, 3, 4]
                     splitting list [2, 3] -> Outcome: [1] [2, 3, 4] (second list will be passed to the second if statement and be cut down further)
                    
                If our splitting list covers the seeds partially, including the first seed
                F.e: seed list [1,2,3,4]
                     splitting list [0, 1, 2] -> Outcome: [1, 2] and [3, 4]

                If the splitting list covers the seed list completely, it is not detected in any of the statements below.
            */
            // if current seed has not been altered and it is between the destination range
            if(alt.p[i] == false){
                
                 if(source <= seed_vec.p[i] + seed_range.p[i] - 1 && source > seed_vec.p[i]){

                    // extend the seed vector with the element at the splitting position
                    extend_vec(&seed_vec, source);

                    splice_pos = source - seed_vec.p[i];

                    // extend the range vector with the remaining of the range from the splitting position
                    extend_vec(&seed_range, seed_range.p[i] - splice_pos);

                    // change the old range
                    seed_range.p[i] = splice_pos;

                    // add an alt register for this new vector
                    extend_vec(&alt, false);

                } 
                if(source + range - 1 >= seed_vec.p[i] && source + range < seed_vec.p[i] + seed_range.p[i] && source <= seed_vec.p[i]){

                    // Extend the seed vector with the element after the splitting position
                    extend_vec(&seed_vec, source + range);

                    // splice position = 0 (1st element)
                    // splice position = 1 (2nd element)
                    splice_pos = (source + range) - seed_vec.p[i];


                    // Extend the range vector with the remaining of the range from the splitting position
                    extend_vec(&seed_range, seed_range.p[i] - splice_pos);

                    // Change the old range
                    seed_range.p[i] = splice_pos;

                    // Add an alt register for this new vector
                    extend_vec(&alt, false);
                    
                }
        



                if(seed_vec.p[i] >= source && seed_vec.p[i] < source+range){
                    alt.p[i] = true; // this point has been altered already and should not be altered again 
                    source_range_diff = seed_vec.p[i] - source;
                    seed_vec.p[i] = destination + source_range_diff;
                }
            } 

            i++;
        }
    }

    
    size_t smallest = seed_vec.p[0];

    for(int i=1;i<seed_vec.n;++i){
        if(seed_vec.p[i] < smallest){
            smallest = seed_vec.p[i];
        }
    
    }


    free(seed_vec.p);
    free(seed_range.p);
    free(alt.p);

    fclose(seed);
    fclose(map);

    return smallest;
}
