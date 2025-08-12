#define _GNU_SOURCE
#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct  {
    unsigned long tag;
    bool valid;
    int lru; // for eviction policy
} CacheLine;

typedef struct {
    CacheLine *lines;
} CacheSet;

typedef struct {
    int hit;
    int miss;
    int evication;
} OpResult;

void loadOperation(int s_index, unsigned tag, CacheSet *cache, int E, OpResult *res);

void printTraceInfo(char identifier, unsigned address, int size, OpResult *res) {
    printf("%c %x,%d ", identifier, address, size);
    if (res->miss > 0) {
        for (int i = 0; i < res->miss; i++) {
            printf("miss ");
        }
    }
    if (res->evication > 0) {
        for (int i = 0; i < res->evication; i++) {
            printf("eviction ");
        }
    }
    if (res->hit > 0) {
        for (int i = 0; i < res->hit; i++) {
            printf("hit ");
        }
    }
    printf("\n");
}

void helperFunc(char *program_name, bool args_error) {
    if (args_error) {
        printf("%s: Missing required command line argument\n", program_name);
    }
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", program_name);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", program_name);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", program_name);
}

int main(int argc, char *argv[])
{
    // solve args
    int opt;
    int v_flag = -1;
    int s = -1, E = -1, b = -1;
    char *trace_file = NULL;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 'h':
                helperFunc(argv[0], false);
                return 0;
            case 'v':
                v_flag = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            default:
                helperFunc(argv[0], true);
                return 1;
        }
    }
    if (s == -1 || E == -1 || b == -1 || trace_file == NULL) {
        helperFunc(argv[0], true);
        return 1;
    }
    // 2^s, 2^b, open trace_file
    int s_pow = 1 << s;
    // int b_pow = 1 << b;
    FILE *fp = fopen(trace_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening trace file: %s\n", trace_file);
        return 1;
    }
    // malloc S * E
    CacheSet *cache = malloc(s_pow * sizeof(CacheSet));
    if (cache == NULL) {
        fprintf(stderr, "Memory allocation failed for cache.\n");
        fclose(fp);
        return 1;
    }
    for (int i = 0; i < s_pow; i++) {
        cache[i].lines = malloc(E * sizeof(CacheLine));
        if (cache[i].lines == NULL) {
            fprintf(stderr, "Memory allocation failed for cache lines.\n");
            fclose(fp);
            return 1;
        }
        for (int j = 0; j < E; j++) {
            cache[i].lines[j].valid = false;
            cache[i].lines[j].tag = -1; // Initialize tag
            cache[i].lines[j].lru = 0; // Initialize LRU counter
        }
    }
    // read one line at a time from trace file
    int hits = 0, misses = 0, evictions = 0;  
    char identifier;
    unsigned address;
    int size;
    while (fscanf(fp, " %c %x,%d", &identifier, &address, &size) > 0) {
        // printf("Processing: %c %x,%d\n", identifier, address, size);
        // analyze every line, find in the cache, and record hits/misses/evications
        // [space]operation address,size
        unsigned tag = address >> (s + b);
        int s_index = (address >> b) & (s_pow - 1);
        // int b_index = address & (b - 1);
        OpResult res;
        res.hit = res.miss = res.evication = 0;
        // printf("tag: %x, s_index: %d\n", tag, s_index);
        if (identifier == 'I') {
            continue;
        } else if (identifier == 'L') {
            // load
            loadOperation(s_index, tag, cache, E, &res);
            // int retrive_size = b - b_index + 1;
            // while (size > 0) {
            //     size -= retrive_size;
            //     loadOperation(s_index, tag, cache, E, &res);
            //     address += retrive_size;
            //     tag = address >> (s + b);
            //     s_index = (address >> b) & (s - 1);
            //     b_index = address & (b - 1);
            //     retrive_size = b - b_index + 1;
            // }
            
        } else if (identifier == 'S') {
            // store
            loadOperation(s_index, tag, cache, E, &res);
        } else if (identifier == 'M') {
            // M is L, then S
            loadOperation(s_index, tag, cache, E, &res);
            loadOperation(s_index, tag, cache, E, &res);
        }
        hits += res.hit;
        misses += res.miss;
        evictions += res.evication;
        if (v_flag == 1) {
            printTraceInfo(identifier, address, size, &res);
        }
    }  
    
    printSummary(hits, misses, evictions);
    fclose(fp);
    for (int i = 0; i < s_pow; i++) {
        free(cache[i].lines);
    }
    free(cache);
    return 0;
}


void loadOperation(int s_index, unsigned tag, CacheSet *cache, int E, OpResult *res) {
    
    // res.hit = res.miss = res.evication = 0;
    // hit?
    bool hit = false;
    for (int i = 0; i < E; i++) {
        if (cache[s_index].lines[i].tag == tag && cache[s_index].lines[i].valid && hit == false) {
            // hit
            res->hit += 1;
            cache[s_index].lines[i].lru = 0; // reset LRU counter
            hit = true;
        } else if (cache[s_index].lines[i].valid) {
            cache[s_index].lines[i].lru++;
        }
    }
    if (hit) {
        return;
    }
    // must miss
    bool miss = false;
    res->miss = 1;
    for (int i = 0; i < E; i++) {
        if (cache[s_index].lines[i].valid == false && miss == false) {
            cache[s_index].lines[i].valid = true;
            cache[s_index].lines[i].tag = tag;
            cache[s_index].lines[i].lru = 0; // reset LRU counter
            miss = true;
        } else if (cache[s_index].lines[i].valid) {
            cache[s_index].lines[i].lru++;
        }
    }
    if (miss) {
        return;
    }
    // must evict
    res->evication += 1;
    int max_lru = 0, replace_index = 0;
    for (int i = 0; i < E; i++) {
        if (cache[s_index].lines[i].lru > max_lru) {
            max_lru = cache[s_index].lines[i].lru;
            replace_index = i;
        }
        cache[s_index].lines[i].lru++;
    }
    cache[s_index].lines[replace_index].tag = tag;
    cache[s_index].lines[replace_index].lru = 0; // reset
    return;
}