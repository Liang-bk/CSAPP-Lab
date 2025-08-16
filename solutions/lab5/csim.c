#define _GNU_SOURCE
#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// hashtable start
// 哈希表节点，用于存储键值对
typedef struct Node {
    int key;
    int value;
    struct Node* next; // 指向链表中的下一个节点，用于解决哈希冲突
} Node;

// 哈希表结构
typedef struct {
    int capacity; // 哈希表容量（桶的数量）
    int size;     // 哈希表现有元素数量
    Node** table; // 指向节点指针数组的指针（即桶数组）
} HashMap;

/**
 * @brief 创建一个指定容量的哈希表
 * @param capacity 哈希表的初始容量
 * @return 指向新创建的哈希表的指针，如果失败则返回 NULL
 */
HashMap* create_hash_map(int capacity);

/**
 * @brief 释放哈希表及其所有节点占用的内存
 * @param map 要释放的哈希表
 */
void free_hash_map(HashMap* map);

/**
 * @brief 向哈希表中插入或更新一个键值对
 * 如果键已存在，则更新其值；否则，插入新的键值对。
 * @param map 目标哈希表
 * @param key 键
 * @param value 值
 */
void hash_map_put(HashMap* map, int key, int value);

/**
 * @brief 从哈希表中获取指定键的值
 * @param map 目标哈希表
 * @param key 要查找的键
 * @param value_ptr 用于存储查找到的值的指针
 * @return 如果找到键，则返回 true；否则返回 false
 */
bool hash_map_get(HashMap* map, int key, int* value_ptr);

/**
 * @brief 从哈希表中移除一个键值对
 * @param map 目标哈希表
 * @param key 要移除的键
 */
void hash_map_remove(HashMap* map, int key);

// 一个简单的哈希函数，将键映射到表索引
static unsigned int hash_function(int key, int capacity) {
    // 使用 abs() 确保结果非负，然后取模
    return (unsigned int)abs(key) % capacity;
}

// 创建哈希表
HashMap* create_hash_map(int capacity) {
    if (capacity <= 0) return NULL;

    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) return NULL;

    map->capacity = capacity;
    map->size = 0;
    // 为桶数组分配内存并初始化为 NULL
    map->table = (Node**)calloc(capacity, sizeof(Node*));
    if (!map->table) {
        free(map);
        return NULL;
    }
    return map;
}

// 释放哈希表
void free_hash_map(HashMap* map) {
    if (!map) return;

    for (int i = 0; i < map->capacity; ++i) {
        Node* current = map->table[i];
        // 遍历并释放每个桶中的链表
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

// 插入或更新键值对
void hash_map_put(HashMap* map, int key, int value) {
    if (!map) return;

    unsigned int index = hash_function(key, map->capacity);
    Node* current = map->table[index];

    // 遍历链表，检查键是否已存在
    while (current) {
        if (current->key == key) {
            // 键已存在，更新值并返回
            current->value = value;
            return;
        }
        current = current->next;
    }

    // 键不存在，创建新节点并插入到链表头部
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return; // 内存分配失败

    new_node->key = key;
    new_node->value = value;
    new_node->next = map->table[index];
    map->table[index] = new_node;
    map->size++;
}

// 获取键对应的值
bool hash_map_get(HashMap* map, int key, int* value_ptr) {
    if (!map || !value_ptr) return false;

    unsigned int index = hash_function(key, map->capacity);
    Node* current = map->table[index];

    // 遍历链表查找键
    while (current) {
        if (current->key == key) {
            *value_ptr = current->value;
            return true;
        }
        current = current->next;
    }

    return false; // 未找到键
}

// 删除键值对
void hash_map_remove(HashMap* map, int key) {
    if (!map) return;

    unsigned int index = hash_function(key, map->capacity);
    Node* current = map->table[index];
    Node* prev = NULL;

    // 遍历链表查找要删除的节点
    while (current) {
        if (current->key == key) {
            if (prev) {
                // 要删除的节点不是头节点
                prev->next = current->next;
            } else {
                // 要删除的节点是头节点
                map->table[index] = current->next;
            }
            free(current);
            map->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// hashtable end

// static list start
typedef struct {
    int value;
    int prev;
    int next;
} ListNode;

typedef struct {
    int size;
    int capacity;
    ListNode* nodes;
} List;
// the capacity will be fixed when list was created
List* create_list(int capacity) {
    if (capacity <= 0) return NULL;
    // head, tail, elem...
    ListNode* nodes = (ListNode*)malloc((capacity + 2) * sizeof(ListNode));
    if (!nodes) return NULL;
    
    int head = 0;
    int tail = 1;

    nodes[head].value = -1; 
    nodes[head].prev = -1;
    nodes[head].next = tail;

    nodes[tail].value = -1;
    nodes[tail].prev = head;
    nodes[tail].next = -1;
    for (int i = 2; i < capacity + 2; i++) {
        nodes[i].value = nodes->prev = nodes->next = -1;
    }
    List* lst = (List*)malloc(sizeof(List));
    lst->size = 2;
    lst->capacity = capacity + 2;
    lst->nodes = nodes;
    return lst;
}
// 从双链表中把指定的index节点删去
int list_remove_index(List* list, int index) {
    if (!list || index < 2 || index >= list->size) return -1;

    ListNode* nodes = list->nodes;
    ListNode* node = &nodes[index];
    int n_prev = node->prev;
    int n_next = node->next;

    nodes[n_prev].next = n_next;
    nodes[n_next].prev = n_prev;
    list->size--;
    return index;
}
// 
void list_add_to_tail(List* list, int index, int value) {
    if (!list || index < 2 || index >= list->capacity) return;
    ListNode* node = &list->nodes[index];
    
    ListNode* nodes = list->nodes;
    int tail = 1;
    int t_prev = nodes[tail].prev;
    node->prev = t_prev;
    node->next = tail;
    node->value = value;
    nodes[t_prev].next = index;
    nodes[tail].prev = index;
    list->size++;
}
int list_get_head_index(List* list) {
    int head = 0;
    if (list->nodes[head].next != 1) {
        return list->nodes[head].next;
    }
    return -1;
}
void list_free(List* list) {
    if (!list) return;
    free(list->nodes);
    free(list);
}
// static list end
typedef struct {
    HashMap *lines;
    List *lru_list;
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
        cache[i].lines = create_hash_map(E);
        // every cache line has a corresponding LRU double linked static list
        cache[i].lru_list = create_list(E);
        if (cache[i].lines == NULL || cache[i].lru_list == NULL) {
            fprintf(stderr, "Memory allocation failed for cache lines.\n");
            fclose(fp);
            return 1;
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
        } else {
            // what if size > block_size, then it must solve many times
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
            loadOperation(s_index, tag, cache, E, &res);
            if (identifier == 'M') {
                loadOperation(s_index, tag, cache, E, &res);
            }
        }
        hits += res.hit;
        misses += res.miss;
        evictions += res.evication;
        if (v_flag == 1) {
            printTraceInfo(identifier, address, size, &res);
        }
    }  
    
    printSummary(hits, misses, evictions);
    // free everything
    fclose(fp);
    for (int i = 0; i < s_pow; i++) {
        free_hash_map(cache[i].lines);
        list_free(cache[i].lru_list);
    }
    free(cache);
    return 0;
}


void loadOperation(int s_index, unsigned tag, CacheSet *cache, int E, OpResult *res) {
    // hit?
    bool hit = false;
    int index = -1;
    hit = hash_map_get(cache[s_index].lines, tag, &index);
    if (hit) {
        res->hit += 1;
        List *lru_list = cache[s_index].lru_list;
        list_remove_index(lru_list, index);
        list_add_to_tail(lru_list, index, tag);
        return;
    }
    // must miss
    res->miss = 1;
    List *lru_list = cache[s_index].lru_list;
    if (lru_list->size != E + 2) {
        // put
        index = lru_list->size;
        hash_map_put(cache[s_index].lines, tag, index);
        list_add_to_tail(lru_list, index, tag);
        return;
    }
    // must evict
    res->evication += 1;
    index = list_get_head_index(lru_list);
    int old_tag = lru_list->nodes[index].value;
    hash_map_remove(cache[s_index].lines, old_tag);
    list_remove_index(lru_list, index);
    hash_map_put(cache[s_index].lines, tag, index);
    list_add_to_tail(lru_list, index, tag);
    return;
}