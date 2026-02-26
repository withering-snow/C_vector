#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//初始化
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




//方法
void _vector_push_back(vector* v, const void* restrict value){

    //内存扩充//

    if(v->end==v->end_of_storage){
        //创建新的指针用于迁移数据
        char* new_begin, * new_end, * new_end_of_storage;
        size_t new_capacity;
        size_t size = _vector_size(v);

        //对于空vector,默认分配1单位空间
        if(size==0)
            new_capacity = 1;
        //否则，分配其2倍的空间
        else
            new_capacity = size*2;

        //提前记录原长度并进行内存扩充
        size_t len = size;
        new_begin = (char*)realloc((char*)v->begin, new_capacity*v->element_size);

        //确认内存分配成功
        VECTOR_ASSERT(new_begin==NULL, "VECTOR_EXPAND_ERROR");

        new_end = new_begin + len*v->element_size;
        new_end_of_storage = new_begin + new_capacity*v->element_size;

        v->begin = new_begin;  v->end = new_end; v->end_of_storage = new_end_of_storage;
    }
    //


    //存入数据
    char* tmp = (char*)(v->end);
    memcpy(tmp, (char*)value, v->element_size);
    tmp += v->element_size;

    //更新尾指针
    v->end = tmp;
}


void _vector_pop_back(vector *v){

    //检查是否为空
    VECTOR_ASSERT(v->end==v->begin, "VECTOR_EMPTY_ERROR");

    //计算尾部新位置
    char* tmp = (char*)(v->end);
    tmp -= v->element_size;

    //更新尾指针
    v->end = tmp;
}


void* _vector_visit(const vector* v, size_t index){

    //检查索引合法性
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    VECTOR_ASSERT(index>=_vector_size(v), "VECTOR_INDEX_ERROR");

    //返回索引对应的地址
    return (char*)(v->begin) + index*v->element_size;
}


void _vector_clear(vector* v){

    //“清空”内容
    v->end = v->begin;
}


void _vector_destroy(vector* v){

    //释放空间
    if(v->begin)
        free(v->begin);

    //指针收回
    v->begin = v->end = v->end_of_storage = NULL;
    v->element_size = 0;
}