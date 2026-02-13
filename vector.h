#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


//错误处理定义宏
#define VECTOR_ASSERT(condition, message) \
do { \
if (condition) { \
fprintf(stderr, "\n\033[1;31m[Vector Runtime Error]\033[0m\n"); \
fprintf(stderr, "Condition: %s\n", #condition); \
fprintf(stderr, "Message:   %s\n", message); \
fprintf(stderr, "Location:  %s (%s:%d)\n", __func__, __FILE__, __LINE__); \
abort(); \
} \
} while(0)
//


//对vector结构体定义
typedef struct{
    size_t element_size;
    void* begin;
    void* end;
    void* end_of_storage;
}vector;
//


//初始化
void _vector_init(vector* v,size_t element_size, unsigned int size, const void* value);
#define vector_init(v, type, size, value) _vector_init(&v, sizeof(type), size, (void*)value)
//


//基本属性
unsigned int vector_size(vector* v);
unsigned int vector_capacity(vector* v);
//


//方法
void vector_push_back(vector* v, const void* value);
void vector_pop_back(vector* v);
void vector_alter(vector* v, unsigned int index, void* value);
void* vector_get(vector* v, unsigned int index);
void vector_clear(vector* v);
void vector_destroy(vector* v);
//

#endif