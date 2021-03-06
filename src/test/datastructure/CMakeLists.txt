if(${STATICLIB_SWITCH} STREQUAL "OFF")

	################################
	# Unit Tests
	################################

	add_executable(runDatastructureTests
		DAGTest.cpp
		EventTimingProviderTest.cpp
		FlowpipeTest.cpp
		HalfspaceTest.cpp
		HIVTest.cpp
		PointTest.cpp
		VertexTest.cpp
		VertexContainerTest.cpp
		RangeTest.cpp
		StateTest.cpp
		StateVisitorTest.cpp
		StrategyTest.cpp
		HybridAutomataTest.cpp
		#HybridAutomataOutputTest.cpp
		WorkQueueTest.cpp
		#HybridAutomataParallelCompositionTest.cpp
	)

	add_dependencies(runDatastructureTests resources)
	target_include_directories(runDatastructureTests PRIVATE ${GTEST_INCLUDE_DIR})

	target_link_libraries(runDatastructureTests
							${PROJECT_NAME}-shared
							${GTEST_LIBRARIES}
							)

	cotire(runDatastructureTests)

	add_test( NAME datastructures COMMAND runDatastructureTests )
endif()
