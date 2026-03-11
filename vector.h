#ifndef VECTOR_H
#define VECTOR_H




#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif
//VECTOR_NDEBUG负责关闭断言检测，在不定义的情况下会在风险处进行内存检查
//如果确定不会有内存风险，可以在程序中 #define VECTOR_NDEBUG 以加快效率




//对vector结构体定义
typedef struct{
    size_t element_size;
    void* begin;
    void* end;
    void* end_of_storage;
}vector;
#define VECTOR_NULL {0, NULL, NULL, NULL}




//函数声明//

//生命周期
void _vector_init(vector* v, size_t element_size, size_t size, const void* restrict value);
void _vector_reserve(vector* v, size_t capacity);
void _vector_destroy(vector* v);

//基本属性
static inline size_t _vector_size(const vector* v){
    return ((char*)v->end - (char*)v->begin) / v->element_size;
}
static inline size_t _vector_capacity(const vector* v){
    return ((char*)v->end_of_storage - (char*)v->begin) / v->element_size;
}

//操作函数
void _vector_push_back(vector* v, const void* restrict value);
void _vector_pop_back(vector* v);
void _vector_insert(vector* v, size_t index, const void* restrict value);
void _vector_erase(vector* v, size_t index);
void _vector_fit(vector* v);
void _vector_clear(vector* v);

//访问函数
void* _vector_front(vector* v);
void* _vector_back(vector* v);
void* _vector_visit(const vector* v, size_t index);




//封装宏//

#define vector_init(v, type, size, value) \
    do { \
        __typeof__(value) tmp = (value); \
        _vector_init(&v, sizeof(type), size, &tmp); \
    } while(0)
#define vector_reserve(v, capacity) _vector_reserve(&v, capacity)
#define vector_destroy(v) _vector_destroy(&v)

#define vector_size(v) _vector_size(&v)
#define vector_capacity(v) _vector_capacity(&v)

#define vector_push_back(v, value)\
    do{ \
        __typeof__(value) tmp = (value); \
        _vector_push_back(&v, &tmp); \
    }while(0)
#define vector_pop_back(v) _vector_pop_back(&v)
#define vector_insert(v, index, value) \
    do{ \
        __typeof__(value) tmp = (value); \
        _vector_insert(&v, index, &tmp); \
    }while(0)
#define vector_erase(v, index) _vector_erase(&v, index)
#define vector_fit(v) _vector_fit(&v)
#define vector_clear(v) _vector_clear(&v)

#define vector_front(v, type) \
    *((type*)_vector_front(&v))
#define vector_back(v, type) \
    *((type*)_vector_back(&v)-1)
#define vector_visit(v, type, index) \
    *((type*)_vector_visit(&v, index))
#define vector_auto(it, v, type) \
    for (type* it = (type*)(v).begin; \
    it != (type*)(v).end; \
    it++)




//断言宏//
#ifndef VECTOR_NDEBUG
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


#ifdef __cplusplus
}
#endif

#endif