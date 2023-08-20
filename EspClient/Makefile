CC = g++
CXXFLAGS = -std=c++11 -Wall -I./include -I/usr/include/websocketpp

# Get a list of all source files in src/ and websocketpp/
PROJECT_SOURCES = $(wildcard src/*.cpp)
WEBSOCKETPP_SOURCES = $(wildcard websocketpp/common/*.cpp) $(wildcard websocketpp/client/*.cpp)

# Convert source filenames to object filenames in the obj/ directory
PROJECT_OBJ = $(addprefix $(OBJDIR)/, $(notdir $(PROJECT_SOURCES:.cpp=.o)))
WEBSOCKETPP_OBJ = $(addprefix $(OBJDIR)/, $(notdir $(WEBSOCKETPP_SOURCES:.cpp=.o)))

OBJDIR = obj
APPNAME = EspClient

all: $(APPNAME)

$(APPNAME): $(PROJECT_OBJ) $(WEBSOCKETPP_OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: websocketpp/common/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: websocketpp/client/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(APPNAME)
