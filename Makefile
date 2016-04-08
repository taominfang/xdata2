CXXFLAGS =	-O2 ${DEBUG_FLAG} -Wall -fmessage-length=0


all:_all test

prejob:
	mkdir -p ${TARGET_DIR}
	

	
${TARGET_DIR}/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS)  -o $@ $<
	
${TARGET_DIR}/%${OS_EXE_EXT}: ${TARGET_DIR}/%.o ${TARGET_DIR}/main.o 
	$(CXX) $(LINKFLAGS)  -o $@ $<  ${TARGET_DIR}/main.o $(LIBS)

LIBS = -l boost_regex


${TARGET_DIR}/utest_boost${OS_EXE_EXT}:${TARGET_DIR}/utest_boost.o
	$(CXX) $(LINKFLAGS)  -o $@ $< $(LIBS)


TARGET =${TARGET_DIR}/CommandLineParameterExample${OS_EXE_EXT} ${TARGET_DIR}/RegAwk${OS_EXE_EXT} ${TARGET_DIR}/BoostRegexTest${OS_EXE_EXT} 

UNIT_TESTS=${TARGET_DIR}/utest_boost${OS_EXE_EXT}

_all:prejob	$(TARGET)


test:prejob	$(UNIT_TESTS)

clean:
	rm -rf ${TARGET_DIR}
