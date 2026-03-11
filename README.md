# C_vector 使用说明
本库提供了一个通用的 C 语言动态数组容器。支持任意类型的存储，并内置越界检查。

***
***
## 快速使用

***
### cmake配置
#### 目录结构： 
将 vector 库整个文件夹放在主项目的 lib 或 extern 目录下。

```text
Project/
├── CMakeLists.txt
├── main.c
└── extern/
    └── vector/
```
#### 主项目的 CMakeLists.txt 配置：
```cmake
# 1. 进入子目录并运行那里的 CMakeLists.txt
add_subdirectory(extern/vector)
add_executable(Project main.c)

# 2. 链接vector库
target_link_libraries(Project PRIVATE vector)
```

***
### 基础使用方法
```c
// 推荐做法：使用 VECTOR_NULL 进行静态初始化，确保指针初始为 NULL
vector v = VECTOR_NULL;
// 初始化 (变量名, 类型, 初始数量, 初值)
vector_init(v, int, 0, 0);

// 存入数据至尾部 (变量名, 新值)
vector_push_back(v, 100);
vector_insert(v, 1, 200);

// 访问对应索引的数据 (变量名, 类型, 索引)
int a = vector_visit(v, int, 1);
vector_front(v, int) = 50;

//遍历所有元素  (遍历变量, 变量名, 类型)
vecrot_auto(it, v, int){
	printf("%d ", *it);
}

// 销毁, 这一步会释放内存，为防止内存泄漏必须使用
vector_destroy(v)
```

**所有可使用的宏**
- 生命周期：
	- *vector_init(v, type, size, value)*: 初始化，**必须使用**。
	- *vector_reserve(v, capacity)*: 扩张预留空间。
	- *vector_destroy(v)*: 释放内存，**必须使用**。
- 基本属性：
	- *vector_size(v)*: 获取当前元素个数。(返回*size_t*类型)
	- *vector_capacity(v)*: 获取当前内存最大容量。(返回*size_t*类型)
- 操作函数：
	- *vector_push_back(v, value)*: 添加元素至末尾。
	- *vector_pop_back(v)*: 删除末尾元素。
	- *vector_insert(v, index, value)*: 插入元素至指定索引。
	- *vector_erase(v, index)*: 删除索引指定元素。
	- *vector_fit*: 缩小容量至当前有效数据大小。
	- *vector_clear(v)*: 清空数据， 不会释放内存。
- 访问函数：
	- vector_front: 访问首个元素。
	- vector_back: 访问尾部元素。
	- *vector_visit(v, type, index)*: 访问对应索引的数据。
	- *vector_auto(it, v, type)*: 以变量 *it* 遍历整个容器， *it* 类型为对应的指针。


***
***
## 底层实现原理
为了方便后续维护与排查 Bug，以下是该库的底层逻辑说明：

***
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

***
### 宏封装逻辑
为了调用函数时更加便捷，头文件使用了大量宏：
- **自动类型转换**: 使用 *\_\_typeof\_\_* 创建临时变量并取地址，从而允许直接传递常量而不会引发地址报错。
- **接口隔离**: 外部调用的所有函数均为宏，宏会调用带下划线的底层函数。

***
### 性能优化
#### 内联函数
- 对于 *vector_size(v)* 与 *vector_capacity(v)* 两个使用频率较高的函数，其函数体并未使用头文件-源文件的结构，而是使用 *static inline* 在**头文件**中直接展开定义，利用**内联函数**以提高这两个函数的效率，减少调用开销。
#### restrict优化
-  对于 *vector_push_back* 等使用了 *restrict* 修饰符，这向编译器承诺：vector 的内存与传入的数据地址不会重叠，使得编译器可以大胆地进行指令优化。

***
### 扩容机制
- **触发条件**: *v->end == v->end_of_storage*
- **策略**: 初始容量为 1，此后若已满则容量翻倍（*size? size\*2: 1*）。
- **实现**: 使用 *realloc* 重新分配地址。扩容时会提前记录旧长度，并在内存迁移后通过偏移量重新定位 end 指针，避免野指针问题。
- **补充**: 扩容过程已经封装为 *static* 函数 *_vector_realloc* 
***
### 错误处理
- 所有涉及索引访问和内存操作的地方均挂载了 *VECTOR_ASSERT*
- 在你的文件中定义宏 *VECTOR_NDEBUG* 即可关闭调试模式，否则其默认为打开。
	-  **调试模式下**:  *VECTOR_ASSERT* 会进行对应位置的错误检测，且在进行未定义行为时会向 *stderr* 打印报错信息，触发 *abort()* 。这也会**一定程度地降低效率**。
	- **非调试模式下**:  *VECTOR_ASSERT* 将***不会做任何检测***，所有的vector的非定义行为***不会触发段错误***。效率会有所提升，但在这种模式下不良代码***可能出现内存泄漏***，***请谨慎使用***。

> [!TIP] 
> **错误类型说明**
> - **VECTOR_ALREADY_INIT_ERROR**: 重复初始化
> - **VECTOR_MEMORY_INIT_ERROR**: 初始化内存分配失败
> - **VECTOR_EXPAND_ERROR**: 容量扩充失败
> - **VECTOR_EMPTY_ERROR**: 对空容器执行了非法操作
> - **VECTOR_INDEX_ERROR**: 访问了不合法的索引