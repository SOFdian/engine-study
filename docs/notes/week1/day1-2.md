## 主要是CMake学习，CMake Tutorial中Step1的内容

Step1/
├── build/                        # 构建输出目录（存放编译生成的中间文件及 exe）
├── MathFunctions/                # 库文件夹
│   ├── CMakeLists.txt            # 子目录 CMake 配置文件
│   ├── MathFunctions.cxx         # 库源文件
│   └── MathFunctions.h           # 库头文件
├── Tutorial/                     # 主程序文件夹
│   ├── CMakeLists.txt            # 子目录 CMake 配置文件
│   └── Tutorial.cxx              # 主程序源文件
└── CMakeLists.txt                # 项目顶层 CMake 配置文件

### 项目顶层 CMake 配置文件

```
cmake_minimum_required(VERSION 3.23) #描述版本
project(Tutorial)	#项目名称，例如使用VS构建后就是Tutorial.sln
add_subdirectory(Tutorial)	#找子目录“Tutorial”下的CML
add_subdirectory(MathFunctions)	#找子目录下的CML
```

###  子目录 “Tutorial”的CMake 配置文件

```
add_executable(Tutorial) #会生成可执行文件
target_sources(Tutorial  
	PRIVATE
		Tutorial.cxx
) #这个可执行文件的源代码
target_link_libraries(Tutorial
	PRIVATE
		MathFunctions
) #这个可执行文件使用的库，库的名称是由 add_library 定义的（如下）
```

### 子目录 “MathFunctions”的CMake 配置文件

```
add_library(MathFunctions) #定义库名称
target_sources(MathFunctions
	PRIVATE
		MathFunctions.cxx #源文件

	PUBLIC
		FILE_SET HEADERS
		FILES
			MathFunctions.h #列出具体的头文件
)
```

