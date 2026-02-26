#ifndef VECTOR_H
#define VECTOR_H



#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>




//这个宏负责开关断言检测，在未注释的情况下会在风险处进行内存检查
#define VECTOR_DEBUG
//如果确定不会有内存风险，可以将这个宏注释掉以加快效率




//对vector结构体定义
typedef struct{
    size_t element_size;
    void* begin;
    void* end;
    void* end_of_storage;
}vector;
#define VECTOR_NULL {0, NULL, NULL, NULL}
//



//函数头//

//初始化
void _vector_init(vector* v, size_t element_size, size_t size, const void* restrict value);
//

//基本属性
static inline size_t _vector_size(const vector* v){
    return ((char*)v->end - (char*)v->begin) / v->element_size;
}
static inline size_t _vector_capacity(const vector* v){
    return ((char*)v->end_of_storage - (char*)v->begin) / v->element_size;
}

//方法
void _vector_push_back(vector* v, const void* restrict value);
void _vector_pop_back(vector* v);
void* _vector_visit(const vector* v, size_t index);
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

#define vector_visit(v, type, index) \
    *((type*)_vector_visit(&v, index))

#define vector_clear(v) _vector_clear(&v)

#define vector_destroy(v) _vector_destroy(&v)
//


//错误处理
#ifdef VECTOR_DEBUG
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
#else
#define VECTOR_ASSERT(condition, message) \
    ((void)0)
#endif
////



#endif