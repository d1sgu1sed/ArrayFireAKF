TARGET1 = main_test_without_long

NVCC = nvcc
NVCC_FLAGS = -std=c++20

SRC1 = main_test_without_long.cpp

AF_PATH = /opt/arrayfire
AF_INCLUDE = $(AF_PATH)/include
AF_LIB = $(AF_PATH)/lib64
AF_FLAGS = -I$(AF_INCLUDE) -L$(AF_LIB) -laf

all: $(TARGET1)

$(TARGET1): $(SRC1)
	$(NVCC) $(NVCC_FLAGS) $(SRC1) -o $(TARGET1) $(AF_FLAGS)

clean:
	rm -f $(TARGET1)
