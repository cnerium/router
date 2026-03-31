add_executable(router_example_01 01_basic.cpp)
target_link_libraries(router_example_01 PRIVATE router::router)

add_executable(router_example_02 02_dynamic_params.cpp)
target_link_libraries(router_example_02 PRIVATE router::router)

add_executable(router_example_03 03_multiple_routes.cpp)
target_link_libraries(router_example_03 PRIVATE router::router)

add_executable(router_example_04 04_methods.cpp)
target_link_libraries(router_example_04 PRIVATE router::router)

add_executable(router_example_05 05_priority.cpp)
target_link_libraries(router_example_05 PRIVATE router::router)

add_executable(router_example_06 06_full_demo.cpp)
target_link_libraries(router_example_06 PRIVATE router::router)
