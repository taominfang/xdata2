CXXFLAGS =	-O2 ${DEBUG_FLAG} -Wall -fmessage-length=0


all:_all test

prejob:
	mkdir -p ${TARGET_DIR}
	

	
${TARGET_DIR}/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) -I C:/boost/boost_1_60_0  -o $@ $<
	
${TARGET_DIR}/%${OS_EXE_EXT}: ${TARGET_DIR}/%.o ${TARGET_DIR}/main.o 
	$(CXX) $(LINKFLAGS)  -o $@ $<  ${TARGET_DIR}/main.o $(LIBS)

LIBS = ${BOOST_LIBS} -l boost_regex 


TEST_OBJECTS=${TARGET_DIR}/UnitTestBase.o ${TARGET_DIR}/RegAwk.o 

${TARGET_DIR}/UnitFunctionsTest${OS_EXE_EXT}:${TARGET_DIR}/UnitFunctionsTest.o $(TEST_OBJECTS) src/UnitFunctionsTestCases.h
	$(CXX) $(LINKFLAGS)  -o $@ $< $(TEST_OBJECTS) $(LIBS) 


TARGET =${TARGET_DIR}/CommandLineParameterExample${OS_EXE_EXT} ${TARGET_DIR}/RegAwk${OS_EXE_EXT} ${TARGET_DIR}/BoostRegexTest${OS_EXE_EXT} 

UNIT_TESTS=${TARGET_DIR}/UnitFunctionsTest${OS_EXE_EXT}

_all:prejob	$(TARGET)


test:prejob	$(UNIT_TESTS)

clean:
	rm -rf ${TARGET_DIR}
