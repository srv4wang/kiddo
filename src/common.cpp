#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"

std::string get_msg_id() {
    static unsigned int id = 0;
    id++;
    std::stringstream ss;
    ss<<time(NULL)<<id;
    return ss.str();
}

int get_cpu_core_num() {
    char filename[] = "/proc/cpuinfo";
    const int MAX_CPU_NUM = 200;
    int cpu[MAX_CPU_NUM] = {0};
    FILE *fp;
    if ((fp = fopen(filename, "r")) == 0) {
        return -1;
    }

    char s[1024];  
    char *p, *x;
    while ((fgets(s, 1024, fp)) != NULL) {  
        if (strncmp(s, "physical id", strlen("physical id")) != 0) {
            continue;
        }
        p = s;
        p = strchr(p, ':');
        if (p) {
            p+=2;
        } 
        int id = atoi(p);
        // cpu id may be same
        cpu[id] = 1;
                
    }  
    int core_num = 0;
    for (int i = 0; i < MAX_CPU_NUM; ++i) {
        if (cpu[i]) {
            core_num++;
        }
    }
    return core_num;
}
