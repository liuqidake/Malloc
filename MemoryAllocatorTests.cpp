//
// Created by Qi Liu on 2019-03-06.
//
//
#define CATCH_CONFIG_RUNNER

#include <vector>
#include "catch.h"
#include "MemoryAllocator.h"
#include <chrono>


TEST_CASE("testAllocate1") {
    MemoryAllocator mal;

    REQUIRE(mal.allocate(0) == nullptr);
    REQUIRE(mal.allocate(10000) != MAP_FAILED);
    REQUIRE(mal.allocate(2187) != MAP_FAILED);
    REQUIRE(mal.allocate(4095) != MAP_FAILED);
    REQUIRE(mal.allocate(4096) != MAP_FAILED);
    REQUIRE(mal.allocate(4097) != MAP_FAILED);
    REQUIRE(mal.allocate(24576) != MAP_FAILED);
}

TEST_CASE("testAllocate2") {
    MemoryAllocator mal;
    REQUIRE(mal.allocate(10000) != MAP_FAILED);
    REQUIRE(mal.allocate(10000) != MAP_FAILED);
    REQUIRE(mal.allocate(10000) != MAP_FAILED);
    REQUIRE(mal.allocate(10000) != MAP_FAILED);
    REQUIRE(mal.allocate(10000) != MAP_FAILED);
}

TEST_CASE("testAllocate3") {
    MemoryAllocator mal;
    for(int i = 0; i <= 368890; ++i) {
        void *ptr = mal.allocate(i);
        REQUIRE(ptr!= MAP_FAILED);

    }
}

TEST_CASE("testDeallocate1") {
    MemoryAllocator mal;
    std::vector<void*> vec;
    vec.push_back(mal.allocate(10000));
    vec.push_back(mal.allocate(2187));
    vec.push_back(mal.allocate(4095));
    vec.push_back(mal.allocate(4096));
    vec.push_back(mal.allocate(4097));

    for(void* ptr: vec) {
        mal.deallocate(ptr);
    }

    REQUIRE(mal.getSize() == 0);
}

TEST_CASE("testDeallocate2") {
    MemoryAllocator mal;
    void* ptr = mal.allocate(5);
    mal.deallocate(nullptr);
    mal.deallocate(ptr);
    mal.deallocate(nullptr);
}

TEST_CASE("testDeallocate3") {
    MemoryAllocator mal;
    for(int i = 0; i < 100; ++i) {
        mal.deallocate(mal.allocate(i * 4096));
    }
}

TEST_CASE("testMemoryNoneOverlapping") {
    MemoryAllocator mal;

    std::vector<void*> vec;
        for(int i = 1; i <= 5; ++i) {
        void* ptr = mal.allocate(i * 7);
        REQUIRE(ptr != MAP_FAILED);
        vec.push_back(ptr);
    }

}
//
TEST_CASE("testMemoryNoneOverlapping2") {
    MemoryAllocator mal;

    std::vector<void*> vec;
    for(int i = 1; i <= 10000; ++i) {
        void* ptr = mal.allocate(i * 4096);
        REQUIRE(ptr != MAP_FAILED);
        vec.push_back(ptr);
    }
}
//
int main(int argc, const char *argv[]) {
    for(int i = 0; i< 1000000; i++){};
    MemoryAllocator mal;
    std::cout<<"Self-defined malloc timing:"<<std::endl;
    for(int i = 10; i < 30; i++){
        auto start = std::chrono::high_resolution_clock::now();
            mal.allocate(i);
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout<<i<<" bytes: "<<(finish - start).count() <<std::endl;
    }

    std::cout<<"Built-in malloc timing:"<<std::endl;
    for(int i = 10; i < 30; i++){
        auto start = std::chrono::high_resolution_clock::now();
        std::malloc(i);
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout<<i<<" bytes: "<<(finish - start).count() <<std::endl;
    }

    int result = Catch::Session().run(argc, argv);
    return result;
}