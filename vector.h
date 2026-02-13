#ifndef VECTOR_H
#define VECTOR_H



#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

//对vector结构体定义
typedef struct{
    size_t element_size;
    void* begin;
    void* end;
    void* end_of_storage;
}vector;
//



//函数头//

//初始化
void _vector_init(vector* v,size_t element_size, unsigned int size, const void* value);
//

//基本属性
unsigned int _vector_size(vector* v);
unsigned int _vector_capacity(vector* v);
//

//方法
void _vector_push_back(vector* v, const void* value);
void _vector_pop_back(vector* v);
void _vector_alter(vector* v, unsigned int index, void* value);
void* _vector_get(vector* v, unsigned int index);
void _vector_clear(vector* v);
void _vector_destroy(vector* v);
//
////



//封装后的宏//

//初始化
#define vector_init(v, type, size, value) \
    do { \
        __typeof__(value) tmp = (value); \
        _vector_init(&v, sizeof(type), size, &tmp); \
    } while(0)
//

//基本属性
#define vector_size(v) _vector_size(&v)
#define vector_capacity(v) _vector_capacity(&v)
//

//方法

#define vector_push_back(v, value)\
    do{ \
        __typeof__(value) tmp = (value); \
        _vector_push_back(&v, &tmp); \
    }while(0)

#define vector_pop_back(v) _vector_pop_back(&v)

#define vector_alter(v, index, value) \
    do{ \
        __typeof__(value) tmp = (value); \
        _vector_alter(&v, index, &tmp); \
    }while(0)

#define vector_get(v, type, index) \
    (*(type*)_vector_get(&v, index))

#define vector_clear(v) _vector_clear(&v)

#define vector_destroy(v) _vector_destroy(&v)
//

//错误处理
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
////

#endif