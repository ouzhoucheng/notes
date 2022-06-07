# 编译

```C++
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
  cout << "Hello SLAM!" << endl;
  return 0;
}

```

## g++编译

```shell
sudo apt-get install g++ # 安装g++
g++ helloSLAM.cpp # g++编译
./a.out # 运行
```

## cmake编译

新建一个文件`CMakeLists.txt`

```txt
# 声明要求的 cmake 最低版本
cmake_minimum_required(VERSION 2.8)
# 声明一个 cmake 工程
project(HelloSLAM)
# 设置编译模式
set(CMAKE_BUILD_TYPE "Debug")
# 添加一个可执行程序
# 语法：add_executable( 程序名 源代码文件 ）
add_executable(helloSLAM helloSLAM.cpp)
# 添加hello库 .a
add_library(hello libHelloSLAM.cpp)
# 共享库 .so 只有一个副本,更省空间
add_library(hello_shared SHARED libHelloSLAM.cpp)
# 添加可执行程序调用hello库中函数
add_executable(useHello useHello.cpp)
# 将库文件链接到可执行程序上
target_link_libraries(useHello hello_shared)
```

```shell
mkdir build # 将编译文件都放进里面
cd build
cmake .. # 调用cmake对工程进行cmake编译, 处理了工程文件之间关系
make # 对工程编译(g++)
./helloSLAM # 执行
```

## 调用库

库文件

```C++
#include <iostream>
using namespace std;
void printHello() {
  cout << "Hello SLAM" << endl;
}
```

在`CMakeLists.txt`加上

```txt
# 添加hello库 静态库 .a
add_library(hello libHelloSLAM.cpp)
# 共享库 .so 只有一个副本,更省空间
add_library(hello_shared SHARED libHelloSLAM.cpp)
```

编译

提供头文件`libHelloSLAM.h`后可以调用该库

```C++
#ifndef LIBHELLOSLAM_H_
#define LIBHELLOSLAM_H_// 上面的宏定义是为了防止重复引用这个头文件而引起的重定义错误
void printHello();// 打印一句hello的函数
#endif
```

调用这个库`useHello.cpp`

```C++
#include "libHelloSLAM.h"
int main(int argc, char **argv) {
  printHello(); // 使用 libHelloSLAM.h 中的 printHello() 函数
  return 0;
}
```

再在`CMakeLists.txt`加

```txt
# 添加可执行程序调用hello库中函数
add_executable(useHello useHello.cpp)
# 将库文件链接到可执行程序上
target_link_libraries(useHello hello_shared)
```

## IDE

```shell
sudo apt-get install kdevelop
```
