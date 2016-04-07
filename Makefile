CXXFLAGS =	-O2 ${DEBUG_FLAG} -Wall -fmessage-length=0

prejob:
	mkdir -p ${TARGET_DIR}
	
${TARGET_DIR}/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS)  -o $@ $<
	
${TARGET_DIR}/%${OS_EXE_EXT}: ${TARGET_DIR}/%.o
	$(CXX) $(LINKFLAGS)  -o $@ $< $(LIBS)

LIBS =

TARGET =${TARGET_DIR}/CommandLineParameterExample${OS_EXE_EXT}

all:prejob	$(TARGET)

clean:
	rm -rf ${TARGET_DIR}
