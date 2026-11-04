MBEDTLS_DIR = /home/taka/work/study/google_drive/repos/spresense_apps/hostcode/google_drive
MBEDTLS_VERDIR = $(MBEDTLS_DIR)/mbedtls_v2.28

ARDUINO_DIR = ../../libarduino/

CXX = g++
AR = ar
CPPFLAGS += -I. -I$(ARDUINO_DIR) -I$(MBEDTLS_VERDIR)/include
CXXFLAGS ?= -O2 -Wall

LDFLAGS = -L$(ARDUINO_DIR)  -L$(MBEDTLS_VERDIR)/lib
LDLIBS  = -larduino
LDLIBS += -lmbedx509
LDLIBS += -lmbedtls
LDLIBS += -lmbedcrypto

TARGET := https
BUILD_DIR := build
SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
DEPLIB := $(ARDUINO_DIR)/libarduino.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) $(DEPLIB)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET)

$(DEPLIB):
	$(MAKE) -C $(ARDUINO_DIR)

-include $(DEPS)
