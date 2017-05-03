

struct proc{
    char id;
    unsigned int _mem;
    int t_remain;
};


struct memory_list{
    unsigned int _size;
    unsigned int _index;
    struct proc* mlist;
};

void initialize_mlist(struct memory_list m)
{
    m._size = 0;
    m._index = 0;
    m.mlist = (struct proc*)malloc(sizeof(struct proc));
}

unsigned int checkRemain(char Memory[8][32]){
    unsigned int left = 0;
    int i;
    for (i=0; i <8;i++){
        int j;
        for (j=0; j<32;j++){
            if(Memory[i][j] == '.'){
                left++;
            }
        }
    }
    return left;
}

unsigned int RemoveProcess(char Memory[8][32], char id){
    int i;
    unsigned int num = 0;
    for (i=0; i <8;i++){
        int j;
        for (j=0; j<32;j++){
            if(Memory[i][j] == id){
                Memory[i][j] = '.';
                num +=1;
            }
        }
    }
    return num;
}



void printMemory(char Memory[8][32]){
    int i;
    for (i =0; i <32;i++){
        printf("=");
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
    for (i=0; i <8;i++){
        int j;
        for (j=0; j<32;j++){
            printf("%c", Memory[i][j]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
    for (i =0; i <32;i++){
        printf("=");
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
}


void addProcess(char Memory[8][32], int x, int y, struct process p){
    int i,j;
    int num = 0;
    for (i=x; i <8 && num < p._mem;i++){
        for (j=y; j<32 && num < p._mem;j++){
            if (Memory[i][j] != '.'){
                fprintf(stderr, "ERROR: already has memory\n");
                //fflush(stdout);
            }else{
                Memory[i][j] = p.id;
            }
            num+=1;
        }
        y = 0;
    }
    if (num != p._mem){
        fprintf(stderr, "ERROR: not all be added\n");
    }
}

int checkBestFit(char Memory[8][32], struct process p){
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int minx = 0;
    unsigned int miny = 0;
    unsigned int minplace = 32*8+1;
    unsigned int place = 0;
    int i, j;
    for (i=0; i<8; i++){
        for (j=0;j<32;j++){
            if (Memory[i][j] == '.' && place == 0){
                x = i;
                y = j;
                place +=1;
            }else if (Memory[i][j] == '.' && place > 0){
                place +=1;
            }else if (Memory[i][j] != '.' && place > 0){
                if (place >= p._mem){
                    if (place < minplace){
                        minx = x;
                        miny = y;
                        minplace = place;
                    }
                }
                place = 0;
            }
        }
    }
    #ifdef DEBUG_MODE
        printf("minplace is: %d, minx is: %d, miny is:%d\n", minplace, minx, miny);
        fflush(stdout);
    #endif
    if (place > 0 && place < minplace && place >= p._mem){
        minx = x;
        miny = y;
        minplace = place;
    }
    #ifdef DEBUG_MODE
        printf("minplace is: %d, minx is: %d, miny is:%d\n", minplace, minx, miny);
        fflush(stdout);
    #endif
    if (minplace < (32*8+1) &&  minplace >= p._mem){
        addProcess(Memory, minx, miny, p);
        return 1;
    }else{
        return 0;
    }
    
}


void Initialize_memory(char Memory[8][32]){
    int i, j;
    for (i=0; i <8;i++){
        for (j=0; j<32;j++){
            Memory[i][j] = '.';
        }
    }
}

unsigned int tryDefragmentation(char Memory[8][32]){
    int fragnum = 0;
    int newx = 0;
    int newy = 0;
    int i,j;
    for (i=0;i<8;i++){
        for(j=0;j<32;j++){
            if (Memory[i][j] != '.'){
                if (i > newx || (i <= newx && j > newy)){
                    Memory[newx][newy] = Memory[i][j];
                    Memory[i][j] = '.';
                    newy +=1;
                    fragnum +=1;
                }else{
                    newx = i;
                    newy = j+1;
                }
                if (newy ==32){
                    newx+=1;
                    newy=0;
                }
            }
        }
    }
    return fragnum;
}

unsigned int capacity(char ids[]){
    unsigned int num = 0;
    while (ids[num] !='\0')
        num+=1;
    return num;
}

unsigned int getSize(char ids[]){
    unsigned int num = 0;
    while (ids[num] !='.')
        num+=1;
    return num;
}

int CharInArray(char ids[], char id){
    unsigned int size = getSize(ids);
    int i;
    for (i=0;i<size;i++){
        if(id == ids[i])
            return 1;
    }
    return 0;
}



void UpdateId(char Memory[8][32], char ids[]){
    int a;
    for (a=0;a<capacity(ids);a++){
        ids[a] = '.';
    }
    ids[capacity(ids)] = '\0';
    int i,j;
    for (i=0;i<8;i++){
        for(j=0;j<32;j++){
            if (Memory[i][j] != '.'){
                if (CharInArray(ids, Memory[i][j]) == 0){
                    ids[getSize(ids)] = Memory[i][j];
                }
            }
        }
    }
    
}

// char[] getProcess(char memory[8][32]){
//     char procs[];
    
// }

/*best-fit algorithm*/
void Sim_Best_Fit(struct process_list* plist, FILE* output)
{
    
    char Memory[8][32];
    Initialize_memory(Memory);
    /*
    *while true
    *get process which are ready-to-leave 
    *delete them from memory
    *get process which are ready to put in
    *check if they are eligible 
    *   yes, check if they require defragmentation
    *       yes, defragment
    *       no, put in memory at best position
    *no, skip process
    *increment time
    *decrement time remain if not in defragmentation
    *
    *
    */
    
    //test add and remove memory process
    //printMemory(Memory);
    /*add memory*/
    // for (i=0; i <8;i++){
    //     for (j=0; j<32;j++){
    //         if (j < 10){
    //             Memory[i][j] = 'a';
    //         }
            
    //     }
    // }
    //printMemory(Memory);
    //RemoveProcess(Memory, 'a');
    //printMemory(Memory);
    
    

    #ifdef DEBUG_TEST
        /*test addProcess*/
        struct process p1;
        initialize(p1);
        p1.id = 'a';
        p1._mem = 16;
        
        printMemory(Memory);
        addProcess(Memory, 6, 8, p1);
        printMemory(Memory);
        
        struct process p2;
        initialize(p2);
        p2.id = 'b';
        p2._mem = 40;
        addProcess(Memory, 6,24,p2);
        printMemory(Memory);
        
        
        /*test checkBestFit*/
        struct process p3;
        initialize(p3);
        p3.id = 'c';
        p3._mem = 32*8-40-16;
        if (checkBestFit(Memory, p3) == 0){
            if (checkRemain(Memory)>p3._mem){
                //defragmentation
                printf("not supposed to be here\n");
                fflush(stdout);
            }
        }
        printMemory(Memory);
        
        /*test fragmentation*/
        unsigned int frag = tryDefragmentation(Memory);
        printMemory(Memory);
        printf("fragmentation number: %d\n", frag);
        fflush(stdout);
        struct memory_list list;
        initialize_mlist(list);
        list._size = 1;
        list._index = 0;
        list.mlist = NULL;
        
        
        char id_in_memory[plist->_size+1];
        int a;
        for (a=0;a<plist->_size;a++){
            id_in_memory[a] = '.';
        }
        id_in_memory[plist->_size] = '\0';
        
        UpdateId(Memory,id_in_memory);
        for(a=0;a<getSize(id_in_memory);a++){
            printf("%c, ", id_in_memory[a]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
        
    #endif
    
    
    
    unsigned int sim_time = 0;
    unsigned int current_time = 0;
    
    char ids_in_memory[plist->_size+1];
    int a;
    for (a=0;a<plist->_size;a++){
        ids_in_memory[a] = '.';
    }
    ids_in_memory[plist->_size] = '\0';
    
    
    printf("time %dms: Simulator started (Contiguous -- Best-Fit)\n", sim_time);
    fflush(stdout);
    unsigned int working = plist->_size;
    while (working > 0){
        int i;
        /*check for removal*/
        for (i = 0; i < plist->_size; i++){
            if (CharInArray(ids_in_memory,plist->list[i].id) == 1 && ((plist->list[i].t_arrival_1+plist->list[i].t_running_1 == current_time) || (plist->list[i].t_running_2 != 0 && plist->list[i].t_arrival_2+plist->list[i].t_running_2 == current_time))){
                int remove = RemoveProcess(Memory, plist->list[i].id); 
                if (remove != plist->list[i]._mem){
                    printf("removed %d bytes of %c from memory\n", remove, plist->list[i].id);
                    fflush(stdout);
                    fprintf(stderr, "ERROR: invalid remove\n");
                }else{
                    printf("time %dms: Process %c removed:\n", sim_time, plist->list[i].id);
                    fflush(stdout);
                    UpdateId(Memory, ids_in_memory);
                    printMemory(Memory);
                }
            }
            if ( plist->list[i].t_running_2 != 0 && (plist->list[i].t_arrival_2 + plist->list[i].t_running_2) == current_time )
                working--;
            else if (plist->list[i].t_running_2 == 0 && (plist->list[i].t_arrival_1 + plist->list[i].t_running_1) == current_time )
                working--;
        }
         for (i = 0; i < plist->_size; i++){
            if (plist->list[i].t_arrival_1 == current_time || (plist->list[i].t_running_2 != 0 &&plist->list[i].t_arrival_2 == current_time)){
                printf("time %dms: Process %c arrived (requires %d frames)\n", sim_time, plist->list[i].id, plist->list[i]._mem);
                fflush(stdout);
                if (checkBestFit(Memory, plist->list[i]) == 0){
                    if (checkRemain(Memory)>=plist->list[i]._mem){
                        //defragmentation
                        printf("time %dms: Cannot place process %c -- starting defragmentation\n",sim_time, plist->list[i].id);
                        fflush(stdout);
                        unsigned int frag = tryDefragmentation(Memory);
                        //printf("defragmentation should happen\n");
                        sim_time += frag;
                        printf("time %dms: Defragmentation complete (moved %d frames:", sim_time, frag);
                        fflush(stdout);
                        int g;
                        for(g=0;g<getSize(ids_in_memory);g++){
                            if (g== 0){
                                printf(" %c", ids_in_memory[g]);
                                fflush(stdout);
                            }
                            else{
                                printf(", %c", ids_in_memory[g]);
                                fflush(stdout);
                            }
                        }
                        printf(")\n");
                        fflush(stdout);
                        printMemory(Memory);
                        //int check = 
                        if (checkBestFit(Memory, plist->list[i]) > 0){
                            printf("time %dms: Placed process %c:\n", sim_time, plist->list[i].id);
                            fflush(stdout);
                            UpdateId(Memory, ids_in_memory);
                            printMemory(Memory);
                        }else{
                            fprintf(stderr, "ERROR: invalid add\n");
                        }
                    }else{
                        printf("time %dms: Cannot place process %c -- skipped!\n", sim_time, plist->list[i].id);
                        fflush(stdout);
                    }
                }else{
                    printf("time %dms: Placed process %c:\n", sim_time, plist->list[i].id);
                    fflush(stdout);
                    UpdateId(Memory, ids_in_memory);
                    printMemory(Memory);
                }
            }
        }
        current_time+=1;
        sim_time+=1;
    }
    sim_time = sim_time-1;
    printf("time %dms: Simulator ended (Contiguous -- Best-Fit)\n", sim_time);
    fflush(stdout);
}