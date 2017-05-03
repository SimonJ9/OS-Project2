void print_frame(const char* frame){
    char equalsign[32];
    memset(equalsign, '=', 32);
    equalsign[32] = '\0';
    printf("%s\n", equalsign);
    fflush(stdout);
    int k;
    for (k = 0; k < 256; k++){
        if (frame[k] == '\0'){
            printf(".");
            fflush(stdout);
        }
        else {
            printf("%c",frame[k]);
            fflush(stdout);
        }
      
        if ((k+1)%32 == 0){
            printf("\n");
            fflush(stdout);
        }
    }
   
    printf("%s\n", equalsign);
    fflush(stdout);
   
}

int check_frame(const char* frame, int memory, int free_memory){
    int k = 0;
    int location = -1;
    int empty_space = 0;
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
            if (count > empty_space && count >= memory) {
                empty_space = count;
                location = k - count;
            }
            if (free_memory-count < memory)
                break;
            find = 0;
            count = 0;
            k++;
        }
        else k++;
    }
    if (k == 256){
        if (count > empty_space && count > memory)
            location = k - count;
    }
    return location;
}


int defragmentation(char* frame, unsigned int* total_time){
    char new_frame[256] ="";
    char moved[26] = "";
    int k;
    int j = 0;
    for(k = 0; k<256;k++){
        if (frame[k] != '\0'){
            new_frame[j] = frame[k];
            j++;
        }
    }
    int mp = 0;
    int time_start = 0;
    int time_end = 0;
    for(k = 0; k<j;k++){
        if (frame[k] != new_frame[k]){
           if (time_start == 0) time_start = k-1;
           else time_end = k;
           if (new_frame[k] != moved[mp]){
              if (moved[mp] != '\0') mp++;
              moved[mp] = new_frame[k];
           }
        }
    }
    strncpy (frame,new_frame,256);
    (*total_time) += (time_end - time_start);
    printf("time %dms: Defragmentation complete (moved %d frames: ",(*total_time), time_end-time_start);
    fflush(stdout);
    for(k = 0; k<=mp;k++){
       if (k == 0) {
           printf("%c",moved[k]);
           fflush(stdout);
       }
       else {
           printf(", %c",moved[k]);
           fflush(stdout);
       }
    }
    printf(")\n");
    fflush(stdout);
    return j;
}

void insert_process(char* frame,int location, char id, int mem){
    int k;
    for (k = location; k < location + mem; k++){
        frame[k] = id;
    }
}

void Sim_Worst_Fit(const struct process* proc, const int num, FILE* output)
{
    unsigned int current_time = 0;
    unsigned int total_time = 0;
    
    char frame[256] = "";

    printf("time %dms: Simulator started (Contiguous -- Worst-Fit)\n", total_time);
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
                for (j = 0; j<256;j++){
                    if (frame[j] == proc[i].id){
                        flag = 1;
                        frame[j] = '\0';
                    }
                }
                if (flag == 1){
                    free_memory += proc[i]._mem;
                    printf("time %dms: Process %c removed:\n",total_time,proc[i].id);
                    fflush(stdout);
                    print_frame(frame);
                }
            }
            if ( proc[i].t_running_2 != 0 && (proc[i].t_arrival_2 + proc[i].t_running_2) == current_time )
                stillworking--;
            else if (proc[i].t_running_2 == 0 && (proc[i].t_arrival_1 + proc[i].t_running_1) == current_time )
                stillworking--;
        }
        if (stillworking == 0){
            printf("time %dms: Simulator ended (Contiguous -- Worst-Fit)\n\n",total_time);
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
                int location = check_frame(frame,proc[i]._mem,free_memory);
                if (location == -1){
                    printf("time %dms: Cannot place process %c -- starting defragmentation\n",total_time,proc[i].id);
                    fflush(stdout);
                    location = defragmentation(frame,&total_time);
                    print_frame(frame);
                }
                insert_process(frame,location,proc[i].id,proc[i]._mem);
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