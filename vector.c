#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



//初始化
void _vector_init(vector* v, size_t element_size, unsigned int size, const void* value){

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
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_INIT_ERROR");

    //拷贝初值
    char* tmp = (char*)(v->begin);
    for (unsigned int i = 0; i < size; i++) {
        memcpy(tmp, (char*)value, v->element_size);
        tmp += v->element_size;
    }

    //处理结尾指针
    v->end = tmp;
    v->end_of_storage = tmp;

}



//基本属性
unsigned int _vector_size(vector* v){
    return ((char*)v->end - (char*)v->begin) / v->element_size;
}
unsigned int _vector_capacity(vector* v){
    return ((char*)v->end_of_storage - (char*)v->begin) / v->element_size;
}



//方法
void _vector_push_back(vector* v, const void* value){

    //内存扩充//

    if(v->end==v->end_of_storage){
        //创建新的指针用于迁移数据
        char* new_begin, * new_end, * new_end_of_storage;
        unsigned int new_capacity;

        //对于空vector,默认分配1单位空间
        if(_vector_size(v)==0)
            new_capacity = 1;
        //否则，分配其2倍的空间
        else
            new_capacity = _vector_size(v)*2;

        //提前记录原长度并进行内存扩充
        unsigned int len = _vector_size(v);
        new_begin = (char*)realloc((char*)v->begin, new_capacity*v->element_size);

        //确认内存分配成功
        VECTOR_ASSERT(new_begin==NULL, "VECTOR_REALLOC_ERROR");

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


void _vector_alter(vector* v, unsigned int index, void* value){

    //检查索引合法性
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    VECTOR_ASSERT(index>=_vector_size(v), "VECTOR_INDEX_ERROR");

    //取到修改地址
    char* tmp = (char*)(v->begin) + index*v->element_size;
    //拷贝数据
    memcpy(tmp, (char*)value, v->element_size);
}


void* _vector_get(vector* v, unsigned int index){

    //检查索引合法性
    VECTOR_ASSERT(v->begin==NULL, "VECTOR_EMPTY_ERROR");
    VECTOR_ASSERT(index>=_vector_size(v), "VECTOR_INDEX_ERROR");

    //取到访问地址
    char* tmp = (char*)(v->begin) + index*v->element_size;
    //返回指针
    return tmp;
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