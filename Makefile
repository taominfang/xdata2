CXXFLAGS =	-O2 ${DEBUG_FLAG} -Wall -fmessage-length=0

prejob:
	mkdir -p ${TARGET_DIR}
	
%.o: ../src/%.cpp ../src/%.h
	$(CXX) -c $(CXXFLAGS)  -o $@ $<
	
${TARGET_DIR}/%.exe: ${TARGET_DIR}/%.o
	$(CXX) $(LINKFLAGS)  -o $@ $< $(LIBS)

LIBS =

TARGET =${TARGET_DIR}/CommandLineParameterExample.exe

all:prejob	$(TARGET)

clean:
	rm -rf ${TARGET_DIR}
