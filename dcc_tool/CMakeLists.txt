cmake_minimum_required(VERSION 3.16)

# C ve C++ etkin (LLVM config C testleri yapar)
project(dcc_tool C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MinGW'de çok büyük objeler için
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")

# Derleme veritabanı (opsiyonel ama faydalı)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- MSYS2 / MinGW64 kurulumunu kullanıyoruz ---
# (Program Files\LLVM ile MSVC kullanıyorsan bu üç satırı kaldır ve
#  LLVM_DIR/Clang_DIR'i MSVC kurulum yoluna çevir)
list(APPEND CMAKE_PREFIX_PATH "C:/msys64/mingw64")
set(LLVM_DIR  "C:/msys64/mingw64/lib/cmake/llvm")
set(Clang_DIR "C:/msys64/mingw64/lib/cmake/clang")

# Dinamik tek kütüphaneye linkle (ODR/multiple definition önler)
set(LLVM_LINK_LLVM_DYLIB ON)
set(LLVM_LINK_CLANG_DYLIB ON)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "LLVM:  ${LLVM_PACKAGE_VERSION} at ${LLVM_DIR}")
message(STATUS "Clang: ${Clang_DIR}")

# Include'lar
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(dcc_tool
    main.cpp
    DCCVisitor.cpp
    DCCConsumer.cpp
)

# Dinamik tek kütüphanelerle linkle (MinGW'de en sorunsuz yol)
set(LLVM_LINK_LLVM_DYLIB ON)
set(LLVM_LINK_CLANG_DYLIB ON)

# ÖNEMLİ: clang-cpp + LLVM (gerekirse LLVMSupport da ekleriz)
target_link_libraries(dcc_tool PRIVATE
    clang-cpp
    LLVM
)

# Eğer hâlâ "undefined reference to llvm::..." görürsen ŞUNU da ekle:
# target_link_libraries(dcc_tool PRIVATE LLVMSupport)
