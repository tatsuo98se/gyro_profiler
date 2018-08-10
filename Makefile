CXX       := g++
CXXFLAGS  := -Wall -std=c++0x -v

TARGET    := gyro_profiler
PROJ_ROOT := $(realpath .)
VPATH     := $(PROJ_ROOT)
INCLUDES  := -I$(PROJ_ROOT)/lib `pkg-config libczmq --cflags`
SRCS      := *.c
LIBS      := -lpthread \
	      `pkg-config libczmq --libs` \
              -ldl \
	      -lboost_system \
	      -lboost_program_options \
 	      -lstick_sdk \
	      -lbcm2835 \
	      -lm

OBJS      := $(SRCS:.cpp=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)
.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS) -Llib

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<
