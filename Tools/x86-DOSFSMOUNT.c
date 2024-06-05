//
//  x86-DOSFSMOUNT.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/21/24.
//
// DESIGNED TO BE BUILT WITH GCC OR CLANG, NOT x86-DOS CC
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
unsigned char DISK1[2097152];
int DISK1SIZE=0;
unsigned char DISK2[2097152];
int DISK2SIZE=0;
int CTRL_C_OP=0;
char * SYSpath;
char * DOSFSpath;
int e(char * arg1, char * arg2){
    int i=0;
    while(arg1[i]==arg2[i]){
        if(arg1[i]==0 && arg2[i]==0) return 1;
        i++;
    }
    return 0;
}
int CTRLCCOUNT=0;
void CATCH_CTRL_C(int SIGNAL){
    printf("CTRL+C: ");
    if(CTRL_C_OP==2){
        printf("RESET\n");
        CTRL_C_OP=1;
    } else if(CTRL_C_OP==1){
        printf("EXIT\n");
        exit(0);
    } else {
        printf("IGNORED\n");
        CTRLCCOUNT++;
        if(CTRLCCOUNT==4) CTRL_C_OP=1;
    }
}
// ERROR CODE 1: UNKNOWN OP
// ERROR CODE 0x0D: COUNT IS LESS THAN 1
// ERROR CODE 2: LBA DOES NOT EXIST
// ERROR CODE AAh: DISK NOT FOUND
// ERROR CODE 3: ATTEMPT TO READ/WRITE TO BOOT SECTOR
// ERROR CODE 255: EMERGENCY RETURN REACHED.
// RETURN 0 ON SUCCESS
// OP 2: READ
// OP 3: WRITE
int INTERUPT13h(int OP, int LBA, int COUNT, char * esbx, int disk){
    if(COUNT>0){
        if(LBA>0){
            if(disk>-1 && disk<2){
                if(OP==2){
                    
                } else if(OP==3){
                    
                } else return 1;
            } else return 0xAA;
        } else if(LBA==0){
            return 3;
        } else {
            return 2;
        }
    } else return 0x0D;
    return 255;
}
int FILE_SYSTEM(int operation, char * NAME, char * DATA, char * result){
    return 255;
}
void DOSFS(char * cmd){
    char * tokens[10];
    int pointer=0;
    int token_pointer=0;
    while(cmd[pointer]!=0){
        asprintf(&tokens[token_pointer], "%s%c", tokens[token_pointer], cmd[pointer]);
        pointer++;
        if(cmd[pointer]==' '){
            pointer++;
            token_pointer++;
        }
        if(token_pointer==10) break;
        if(e(tokens[0],"mount")){
            FILE *ptr= fopen(tokens[1],"rb");
            if(e(tokens[2],"B")){
                int pointer=0;
                char chr[2];
                while(fread(chr, 1, 1, ptr)){
                    DISK1[pointer]=chr[0];
                    pointer++;
                }
                DISK2SIZE=pointer;
            } else {
                int pointer=0;
                char chr[2];
                while(fread(chr, 1, 1, ptr)){
                    DISK1[pointer]=chr[0];
                    pointer++;
                }
                DISK1SIZE=pointer;
            }
        } else if(e(tokens[0],"unmount")){
            FILE *ptr= fopen(tokens[1],"wb");
            if(e(tokens[2],"B")){
                DISK1SIZE=0;
            } else {
                fwrite(DISK1, 1, sizeof DISK1, ptr);
                DISK1SIZE=0;
            }
        } else if(e(tokens[0], "cd")){
            if(tokens[1][0] == 'C'){
                tokens[1]++;
                strcpy(SYSpath, tokens[1]);
            } else {
                DOSFSpath=tokens[1];
            }
        } else if(e(tokens[0], "cp")){
            int x=0;
            if(e(tokens[1], "ON")){
                char tmp[1024];
                strcpy(tmp, SYSpath);
                strcat(tmp, tokens[2]);
                FILE *ptr = fopen(tmp, "rb");
                //x=FILE_SYSTEM();
            } else {
                char tmp[1024];
                strcpy(tmp, SYSpath);
                strcat(tmp, tokens[3]);
                FILE *ptr = fopen(tmp, "wb");
                //x=FILE_SYSTEM();
            }
            if(x != 0) printf("ERROR CODE: %d", x);
        } else if(e(tokens[0], "rm")){
            int x=0;//FILE_SYSTEM();
            if(x != 0) printf("ERROR CODE: %d", x);
        }
    }
}
int main(int argc, char * argv[]){
    DOSFSpath="A:/";
    SYSpath=getcwd(NULL, 0);
    signal(SIGINT, CATCH_CTRL_C);
    if(argc==1){
        char last[64];
        while(e(last,"exit")==0){
            CTRL_C_OP=2;
            char chr[2];
            printf("C:%s,%s$", SYSpath, DOSFSpath);
            fflush(stdout);
            int pointer=0;
            while(read(0, chr, 1)){
                if(chr[0]=='\n') break;
                last[pointer]=chr[0];
                pointer++;
                if(CTRL_C_OP==1) break;
            }
            if(CTRL_C_OP==1) continue;
            last[pointer]=0;
            DOSFS(last);
        }
    } else if(e(argv[1], "-c")) {
        DOSFS(argv[2]);
    } else{
        char com[64];
        while(e(com,"exit")==0){
            int pointer=0;
            char chr[2];
            while(read(0, chr, 1)){
                if(chr[0]=='\n') break;
                com[pointer]=chr[0];
                pointer++;
            }
            com[pointer]=0;
            DOSFS(com);
        }
    }
    free(SYSpath);
    return 0;
}
