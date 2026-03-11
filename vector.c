#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>




//static函数区
static void _vector_realloc(vector* v, size_t new_capacity){
    //创建新的指针用于迁移数据
    char* new_begin, * new_end, * new_end_of_storage;
    size_t size = _vector_size(v);

    //提前记录原长度并进行内存扩充
    size_t len = size;
    new_begin = (char*)realloc((char*)v->begin, new_capacity*v->element_size);

    //确认内存分配成功
    VECTOR_ASSERT(new_begin==NULL, "VECTOR_EXPAND_ERROR");

    new_end = new_begin + len*v->element_size;
    new_end_of_storage = new_begin + new_capacity*v->element_size;

    v->begin = new_begin;  v->end = new_end; v->end_of_storage = new_end_of_storage;
}




//初始化//
void _vector_init(vector* v, size_t element_size, size_t size, const void* restrict value){

    //对重复定义的检测
    VECTOR_ASSERT(v->begin!=NULL, "VECTOR_ALREADY_INIT_ERROR");

    //设置其元素大小
    v->element_size = element_size;

    //对空vector的处理
    if (size == 0) {
        v->begin = v->end = v->end_of_storage = NULL;
        return;
    }

    //分配内存
    v->begin = malloc(v->element_size * size);
    //确认内存分配成功
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_MEMORY_INIT_ERROR");

    //拷贝初值
    char* tmp = (char*)(v->begin);
    for (size_t i = 0; i < size; i++) {
        memcpy(tmp, (char*)value, v->element_size);
        tmp += v->element_size;
    }

    //处理结尾指针
    v->end = tmp;
    v->end_of_storage = tmp;
}

//空间预留//
void _vector_reserve(vector* v, size_t capacity){

    //对预留空间过小的检测
    VECTOR_ASSERT(_vector_size(v)>capacity, "VECTOR_RESERVE_ERROR");

    //对足够的空间直接返回
    if(_vector_size(v)>=capacity)
        return;

    //预留足够空间
    _vector_realloc(v, capacity);
}

//销毁//
void _vector_destroy(vector* v){

    //释放空间
    if(v->begin)
        free(v->begin);

    //指针收回
    v->begin = v->end = v->end_of_storage = NULL;
    v->element_size = 0;
}




//尾插元素//
void _vector_push_back(vector* v, const void* restrict value){

    //内存扩充//
    if(v->end==v->end_of_storage){
        size_t size = _vector_size(v);

        //分配新空间
        _vector_realloc(v, size? size*2: 1);
    }

    //存入数据
    char* tmp = (char*)(v->end);
    memcpy(tmp, (char*)value, v->element_size);
    tmp += v->element_size;

    //更新尾指针
    v->end = tmp;
}

//尾删元素//
void _vector_pop_back(vector *v){

    //检查是否为空
    VECTOR_ASSERT(v->end==v->begin, "VECTOR_EMPTY_ERROR");

    //计算尾部新位置
    char* tmp = (char*)(v->end);
    tmp -= v->element_size;

    //更新尾指针
    v->end = tmp;
}

//插入元素
void _vector_insert(vector* v, size_t index, const void* restrict value){

    //索引合法性检测
    size_t size = _vector_size(v);
    VECTOR_ASSERT(index>size, "VECTOR_INDEX_ERROR");

    //检测容量大小
    if(v->end==v->end_of_storage){
        //容量不足扩容
        _vector_realloc(v, size? size*2: 1);
    }

    //后移元素
    char* target = (char*)v->begin + index*v->element_size;
    memmove(target+v->element_size, target, (size-index)*v->element_size);

    //插入元素
    memcpy(target, value, v->element_size);

    //移动指针
    char* tmp = (char*)v->end + v->element_size;
    v->end = tmp;
}

//删除元素
void _vector_erase(vector* v, size_t index){

    //索引合法性检测
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    size_t size = _vector_size(v);
    VECTOR_ASSERT(index>=size, "VECTOR_INDEX_ERROR");

    //前移元素
    char* target = (char*)v->begin + index*v->element_size;
    memmove(target, target+v->element_size, (size-index-1)*v->element_size);

    //移动指针
    char* tmp = (char*)v->end - v->element_size;
    v->end = tmp;
}

//缩容至实际大小//
void _vector_fit(vector* v){
    //直接移动指针，重新分配内存
    size_t size = _vector_size(v);
    _vector_realloc(v, size);
}

//清除元素，但保留内存//
void _vector_clear(vector* v){
    //直接移动指针
    v->end = v->begin;
}




//访问首元素//
void* _vector_front(vector* v){
    //检查索引合法性
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    //返回索引对应的地址
    return (char*)v->begin;
}
//访问尾元素//
void* _vector_back(vector* v){
    //检查索引合法性
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    //返回索引对应的地址
    return (char*)v->end;
}
//访问任意元素//
void* _vector_visit(const vector* v, size_t index){
    //检查索引合法性
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    VECTOR_ASSERT(index>=_vector_size(v), "VECTOR_INDEX_ERROR");
    //返回索引对应的地址
    return (char*)(v->begin) + index*v->element_size;
}