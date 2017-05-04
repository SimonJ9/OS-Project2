/* CSCI-4210 Operating Systems Project 2
 * 
 * Group Members: 
 * 
 * Yifan Xu: xuy11 
 * Hantian Jiang: jiangh5 
 * Chenjun Zhou: zhouc3 
 * 
 * 
 */ 

int check_frame_non(const char* frame, int memory){
    int k = 0;
    int location = 0;
    while (k < 256){
        if (frame[k] == '\0'){
            location = k;
            break;
        }
        k++;
    }
    return location;
}

void insert_process_non(char* frame,int location, char id, int mem){
    int k;
    int insert = 0;
    for (k = location; k < 256 && insert < mem; k++){
        if(frame[k] == '\0') {
            frame[k] = id;
            insert++;
        }
    }
}

/*non-contiguous algorithm*/
void Sim_Non_Con(const struct process* proc, const int num, FILE* output)
{
    unsigned int total_time = 0;
    
    char frame[256] = "";

    printf("time %dms: Simulator started (Non-contiguous)\n", total_time);
    fflush(stdout);
    
    int stillworking = num;
    int free_memory = 256;
    
    while(stillworking){
        int i;
        /* Remove */
        for (i = 0; i < num; i++){
            if ( (proc[i].t_arrival_1 + proc[i].t_running_1) == total_time || 
                (proc[i].t_running_2 != 0 && (proc[i].t_arrival_2 + proc[i].t_running_2) == total_time )) {
                int j;
                int flag = 0;
                for (j = 0; j<256;j++){
                    if (frame[j] == proc[i].id){
                        flag = 1;
                        frame[j] = '\0';
                    }
                }
                if (flag == 1){
                    free_memory += proc[i]._mem;
                    printf("time %dms: Process %c removed:\n",total_time,proc[i].id);
                    print_frame(frame);
                    fflush(stdout);
                }
            }
            if ( proc[i].t_running_2 != 0 && (proc[i].t_arrival_2 + proc[i].t_running_2) == total_time )
                stillworking--;
            else if (proc[i].t_running_2 == 0 && (proc[i].t_arrival_1 + proc[i].t_running_1) == total_time )
                stillworking--;
        }
        if (stillworking == 0){
            printf("time %dms: Simulator ended (Non-contiguous)\n",total_time);
            fflush(stdout);
        }
        /* Add */
        for (i = 0; i < num; i++){
            if ((proc[i].t_arrival_1 == total_time) || (proc[i].t_running_2 != 0 && proc[i].t_arrival_2 == total_time) ){
                printf("time %dms: Process %c arrived (requires %d frames)\n",total_time,proc[i].id,proc[i]._mem);
                fflush(stdout);
                if (free_memory < proc[i]._mem){
                    printf("time %dms: Cannot place process %c -- skipped!\n",total_time,proc[i].id);
                    fflush(stdout);
                    continue;
                }
                int location = check_frame_non(frame,proc[i]._mem);
                insert_process_non(frame,location,proc[i].id,proc[i]._mem);
                free_memory -= proc[i]._mem;
                printf("time %dms: Placed process %c:\n",total_time,proc[i].id);
                fflush(stdout);
                print_frame(frame);
            }
        }
        
        total_time++;
    }

    
}