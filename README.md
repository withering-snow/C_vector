# C_vector 使用说明
本库提供了一个通用的 C 语言动态数组容器。支持任意类型的存储，并内置越界检查。

***
## 快速使用
基础流程
```c
vector v;
// 1. 初始化 (变量名, 类型, 初始数量, 初始值指针)
vector_init(v, int, 0, NULL);

// 2. 存入数据 (自动取地址，直接传值即可)
vector_push_back(v, 100);
vector_push_back(v, 200);

// 3. 获取数据 (变量名, 类型, 索引)
int a = vector_get(v, int, 1); 

// 4. 修改数据
vector_alter(v, 0, 888);

// 5. 销毁 (释放内存)
vector_destroy(v)
```

**常用 API**
- *vector_size(v)*: 获取当前元素个数。
- *vector_capacity(v)*: 获取当前内存最大容量。
- *vector_pop_back(v)*: 删除末尾元素。
- *vector_clear(v)*: 清空数据但保留内存，方便下次复用。

***
## 底层实现原理
为了方便后续维护与排查 Bug，以下是该库的底层逻辑说明：

### 内存模型
本库采用*指针式管理*（参考C++ STL 源码），结构体定义如下：
```c
typedef struct {
    size_t element_size;   // 单个元素的字节数
    void* begin;           // 数组首地址
    void* end;             // 逻辑结尾（最后一个元素的下一个位置）
    void* end_of_storage;  // 物理内存结尾
} vector;
```

- **Size** 计算: *(end - begin) / element_size*
- **Capacity** 计算: *(end_of_storage - begin) / element_size*

### 宏封装逻辑
为了调用函数时更加便捷，头文件使用了大量宏：
- **自动类型转换**: 使用 *\_\_typeof\_\_* 创建临时变量并取地址，从而允许直接传递常量而不会引发地址报错。
- **接口隔离**: 外部调用的所有函数均为宏，宏会调用带下划线的底层函数。
### 扩容机制
- **触发条件**: *v->end == v->end_of_storage*
- **策略**: 初始容量为 1，此后若已满则容量翻倍（*new_capacity = current_size * 2*）。
- **实现**: 使用 *realloc* 重新分配地址。扩容时会提前记录旧长度，并在内存迁移后通过偏移量重新定位 end 指针，避免野指针问题。
### 错误处理
所有涉及索引访问和内存操作的地方均挂载了 *VECTOR_ASSERT*：
如果发生错误，程序会向 *stderr* 打印详细错误报告并直接 *abort()*。
> [!tips] 错误类型
> *VECTOR_INIT_ERROR*：初始化内存分配失败
> *VECTOR_REALLOC_ERROR*：容量扩充失败
> *VECTOR_EMPTY_ERROR*：对空容器执行了非法操作
> *VECTOR_INDEX_ERROR*：访问了不合法的索引
