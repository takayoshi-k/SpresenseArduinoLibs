MBEDTLS_DIR = modules
MBEDTLS_VERDIR = $(MBEDTLS_DIR)/mbedtls_v2.28

CXX = g++
CC = gcc
AR = ar
CPPFLAGS += -I. -I$(MBEDTLS_VERDIR)/include
CXXFLAGS ?= -O2 -Wall
CFLAGS ?= -O2 -Wall

TARGET := libarduino.a
BUILD_DIR := build
SOURCES := $(wildcard *.cpp)
CSOURCES := utility/URLParser/http_parser.c
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES)) $(patsubst %.c,$(BUILD_DIR)/%.o,$(CSOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET)

-include $(DEPS)
