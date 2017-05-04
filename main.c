//@Simon: next-fit
#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "BestFit.h"
#include "NextFit.h"
#include "WorstFit.h"
#include "NonCon.h"



int main(int argc, char* argv[]) {
    
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./%s, <input_file>", argv[0]);
        return EXIT_FAILURE;
    }
    
    //Data Section
    unsigned int num_process = 0;
    struct process_list p_list;
    FILE* input_file = fopen(argv[1], "r");
    
    if(input_file == NULL)  
        error("fopen() failed");
    
    initialize_list(p_list);
    
    parse_input(input_file, &p_list, num_process);
    
    
    /*Simulation*/
    
    Sim_Next_Fit(&p_list, stdout);
    Sim_Best_Fit(&p_list, stdout);
    Sim_Worst_Fit(p_list.list,p_list._size,stdout);//change arguments for worst_fit
    Sim_Non_Con(&p_list, stdout);
    
#if 0
    print_list(&p_list);
#endif
    
    
    fclose(input_file);
    return EXIT_SUCCESS;
}