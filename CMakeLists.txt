cmake_minimum_required(VERSION 3.6)
project(Concurrency)

set(CMAKE_CXX_STANDARD 11)

include_directories("src/core")

#Library Src
set(Library_Src Concurrency/CallOnce.cpp Concurrency/common.h Concurrency/FutureTest.cpp Concurrency/ManagerThread.cpp Concurrency/QuickSortTest.cpp Concurrency/SafeQueue.cpp Concurrency/scope_thread.cpp Concurrency/SwapObject.cpp Concurrency/thread_guard.cpp Concurrency/threadsafe_stack.cpp Concurrency/AtomicTest.hpp)

set(SOURCE_FILES Concurrency/main.cpp ${Library_Src} )
add_executable(Concurrency ${SOURCE_FILES})