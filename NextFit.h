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
int check_frame_next(const char* frame, int memory, int free_memory, int recent_location){
    int k = recent_location;
    int location = -1;
    int count = 0;
    int find = 0;
    while (k < 256){
        if (find == 0 && frame[k] == '\0'){
            if (frame[k+memory-1] == '\0'){
                find = 1;
                count++;
                k++;
            }
            else k += memory;
        }
        else if (find == 1 && frame[k] == '\0'){
            count++;
            k++;
        }
        else if (find == 1 && frame[k] != '\0'){
            if (count >= memory) {
                location = k - count;
                break;
            }
            find = 0;
            count = 0;
            k++;
        }
        else k++;
    }
    if (k == 256){
        if (count >= memory)
            location = k - count;
    }
    if (location != -1) return location;
    
    k = recent_location-1;
    count = 0;
    find = 0;
    while (k >= 0){
        if (find == 0 && frame[k] == '\0'){
            find = 1;
            count++;
            k--;
        
        }
        else if (find == 1 && frame[k] == '\0'){
            count++;
            k--;
        }
        else if (find == 1 && frame[k] != '\0'){
            if ( count >= memory) {
                location = k+1;
                break;
            }
            find = 0;
            count = 0;
            k--;
        }
        else k--;
    }
    if (k == -1){
        if ( count >= memory)
            location = k +1;
    }
    return location;
    
}
int insert_process_next(char* frame,int location, char id, int mem){
    int k;
    for (k = location; k < location + mem; k++){
        frame[k] = id;
    }
    return location+mem;
}

void Sim_Next_Fit(const struct process* proc, const int num, FILE* output)
{
    unsigned int current_time = 0;
    unsigned int total_time = 0;
    int recent_location = 0;
    char frame[256] = "";

    printf("time %dms: Simulator started (Contiguous -- Next-Fit)\n", total_time);
    fflush(stdout);
    int stillworking = num;
    int free_memory = 256;
    
    while(stillworking){
        int i;
        /* Remove */
        for (i = 0; i < num; i++){
            if ( (proc[i].t_arrival_1 + proc[i].t_running_1) == current_time || 
                (proc[i].t_running_2 != 0 && (proc[i].t_arrival_2 + proc[i].t_running_2) == current_time )) {
                int j;
                int flag = 0;
                int update = 0;
                for (j = 0; j<256;j++){
                    if (frame[j] == proc[i].id){
                        flag = 1;
                        frame[j] = '\0';
                 
                        recent_location = j;
                        update = 1;
                        
                    }
                }
                if (update == 1) recent_location+=1;
                if (flag == 1){
                    free_memory += proc[i]._mem;
                    printf("time %dms: Process %c removed:\n",total_time,proc[i].id);
                    print_frame(frame);
                    fflush(stdout);
                }
            }
            if ( proc[i].t_running_2 != 0 && (proc[i].t_arrival_2 + proc[i].t_running_2) == current_time )
                stillworking--;
            else if (proc[i].t_running_2 == 0 && (proc[i].t_arrival_1 + proc[i].t_running_1) == current_time )
                stillworking--;
        }
        if (stillworking == 0){
            printf("time %dms: Simulator ended (Contiguous -- Next-Fit)\n\n",total_time);
            fflush(stdout);
        }
        /* Add */
        for (i = 0; i < num; i++){
            if ((proc[i].t_arrival_1 == current_time) || (proc[i].t_running_2 != 0 && proc[i].t_arrival_2 == current_time) ){
                printf("time %dms: Process %c arrived (requires %d frames)\n",total_time,proc[i].id,proc[i]._mem);
                fflush(stdout);
                if (free_memory < proc[i]._mem){
                    printf("time %dms: Cannot place process %c -- skipped!\n",total_time,proc[i].id);
                    fflush(stdout);
                    continue;
                }
                int location = check_frame_next(frame,proc[i]._mem,free_memory,recent_location);
                if (location == -1){
                    printf("time %dms: Cannot place process %c -- starting defragmentation\n",total_time,proc[i].id);
                    fflush(stdout);
                    location = defragmentation(frame,&total_time);
                    print_frame(frame);
                }
                recent_location = insert_process_next(frame,location,proc[i].id,proc[i]._mem);
                free_memory -= proc[i]._mem;
                printf("time %dms: Placed process %c:\n",total_time,proc[i].id);
                fflush(stdout);
                print_frame(frame);
            }
        }
        
        current_time++;
        total_time++;
    }

}