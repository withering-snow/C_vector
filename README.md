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
vector v;

// 初始化 (变量名, 类型, 初始数量, 初值)
vector_init(v, int, 0, 0);

// 存入数据至尾部 (变量名, 新值)
vector_push_back(v, 100);
vector_push_back(v, 200);

// 访问对应索引的数据 (变量名, 类型, 索引)
int a = vector_visit(v, int, 1);
vector_visit(v, 0, 1) = 50;

// 销毁, 这一步会释放内存，为防止内存泄漏必须使用
vector_destroy(v)
```

**所有可使用的宏**
- *vector_init(v, type, size, value)*: 初始化，**必须使用**。
- *vector_size(v)*: 获取当前元素个数。
- *vector_capacity(v)*: 获取当前内存最大容量。
- *vector_push_back(v, value)*: 添加元素至末尾。
- *vector_pop_back(v)*: 删除末尾元素。
- *vector_visit(v, type, index)*: 访问对应索引的数据。
- *vector_clear(v)*: 清空数据， 不会释放内存。
- *vector_destroy(v)*: 释放内存，**必须使用**。

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
### 内联函数
对于 *vector_size(v)* 与 *vector_capacity(v)* 两个使用频率较高的函数，其函数体并未使用头文件-源文件的结构，而是使用 *static inline* 在**头文件**中直接展开定义，利用**内联函数**以提高这两个函数的效率，减少调用开销。

***
### 扩容机制
- **触发条件**: *v->end == v->end_of_storage*
- **策略**: 初始容量为 1，此后若已满则容量翻倍（*new_capacity = current_size * 2*）。
- **实现**: 使用 *realloc* 重新分配地址。扩容时会提前记录旧长度，并在内存迁移后通过偏移量重新定位 end 指针，避免野指针问题。
- ==
***
### 错误处理
- 所有涉及索引访问和内存操作的地方均挂载了 *VECTOR_ASSERT*
- 在vector.h中的*VECTOR_DEBUG* 宏用于开关调试模式，在被定义时会打开**调试模式**（默认打开）。
	-  **调试模式下**:  *VECTOR_ASSERT* 会进行对应位置的错误检测，且在进行未定义行为时会向 *stderr* 打印报错信息，触发 *abort()* 。这也会**一定程度地降低效率**。
	- **非调试模式下**:  *VECTOR_ASSERT* 将***不会做任何检测***，所有的vector的非定义行为***不会触发段错误***。效率会有所提升，但在这种模式下不良代码***可能出现内存泄漏***，***请谨慎使用***。
> [!TIP] *错误类型说明*
> - **VECTOR_ALREADY_INIT_ERROR**: 重复初始化
> - **VECTOR_MEMORY_INIT_ERROR**: 初始化内存分配失败
> - **VECTOR_EXPAND_ERROR**: 容量扩充失败
> - **VECTOR_EMPTY_ERROR**: 对空容器执行了非法操作
> - **VECTOR_INDEX_ERROR**: 访问了不合法的索引
