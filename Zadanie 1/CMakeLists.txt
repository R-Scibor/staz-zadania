cmake_minimum_required(VERSION 3.15)
project(Ticketmat CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_library(ticketmat_core src/CashRegister.cpp src/TicketStore.cpp)
target_include_directories(ticketmat_core PUBLIC src)

enable_testing()

add_executable(cash_register_tests tests/CashRegisterTest.cpp)
target_link_libraries(cash_register_tests PRIVATE ticketmat_core GTest::gtest_main)
add_test(NAME CashRegisterTests COMMAND cash_register_tests)

add_executable(ticket_store_tests tests/TicketStoreTest.cpp)
target_link_libraries(ticket_store_tests PRIVATE ticketmat_core GTest::gtest_main)
add_test(NAME TicketStoreTests COMMAND ticket_store_tests)
