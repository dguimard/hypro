if(${STATICLIB_SWITCH} STREQUAL "OFF")

	################################
	# Core Unit Tests
	################################

	add_executable(runCoreTests
		EigenAdaptionsTest.cpp
		#ifdef LOGGING_HYPRO
		#LoggerTest.cpp
		#endif
		OptimizerTest.cpp
		ParserTest.cpp
		PlotterTest.cpp
		UtilityTest.cpp
	)

	add_dependencies(runCoreTests resources)
	target_include_directories(runCoreTests PRIVATE ${GTEST_INCLUDE_DIR})

	target_link_libraries(runCoreTests
							${PROJECT_NAME}-shared
							${GTEST_LIBRARIES}
							)

	cotire(runCoreTests)

	add_test( NAME core COMMAND runCoreTests )


	################################
	# Parser Unit Tests
	################################

	add_executable(runParserTests
		AntlrParserTest.cpp
		#AntlrParserCIFTest.cpp
	)
	add_dependencies(runParserTests resources antlr4cpp)
	target_include_directories(runParserTests PRIVATE ${GTEST_INCLUDE_DIR})
	target_link_libraries(runParserTests ${PROJECT_NAME}-shared ${GTEST_LIBRARIES})
	#message("Targeted link libraries: " ${PROJECT_NAME} ${GTEST_LIBRARIES} ${ANTLR4CPP_LIBS})
	add_test( NAME parser COMMAND runParserTests )

endif()
