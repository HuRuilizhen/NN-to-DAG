CC := g++
STD_FLAGS := -std=c++11
BASE_FLAGS := -O2

ifeq ($(OS),Windows_NT)
    OS_SPECIFIC_SRC := GraphKit__windows.cpp
    OS_SPECIFIC_FLAGS := -lpsapi
else
    OS_SPECIFIC_SRC := GraphKit__linux.cpp
	OS_SPECIFIC_FLAGS := -lpthread
endif

SRCS := main.cpp Graph.cpp GraphKit.cpp GraphKit__dfs.cpp GraphKit__idfs.cpp GraphKit__greedy.cpp GraphKit__dp.cpp $(OS_SPECIFIC_SRC)

main: $(SRCS)
	$(CC) $(STD_FLAGS) $(BASE_FLAGS) -o main $^ $(OS_SPECIFIC_FLAGS)

clean:
	rm -f main
