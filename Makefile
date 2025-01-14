# 编译器和编译选项
CXX = g++
CXXFLAGS = -Wall -fPIC -I/usr/share/R/include -g 

# 目标共享库
TARGET = build/liball.so
# 源文件
SRCS = $(wildcard src/*.cc src/*.cpp)

# 目标文件
OBJS = $(patsubst src/%.cc, build/%.o, $(patsubst src/%.cpp, build/%.o, $(SRCS)))

# 默认目标
all: $(TARGET)

# 生成共享库
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -shared -o $@ $^

# 编译 .cc 文件
build/%.o: src/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译 .cpp 文件
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -rf build lib

.PHONY: all clean