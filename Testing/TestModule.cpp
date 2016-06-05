// TestModule.cpp

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include "TestModule.h"

#define OPEN_FLAGS O_RDONLY | O_DIRECT | O_SYNC

// todo save errno to another variable before printing

void FuseTester::zeroErrno() {
    errno = 0;
}

void FuseTester::getattrTest() {

    std::cout << " -- getattr test -- " << std::endl;

    zeroErrno();

    struct stat *stat_buf = new struct stat;

    // test on an existing file
    std::cout << std::endl << "Test stat on an existing file" << std::endl;
    int res = stat("mount_dir", stat_buf);

    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    std::cout << std::endl << "Test stat on a non existing file" << std::endl;
    res = stat("mount_dir/no_such_file", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test logfile reference
    std::cout << std::endl << "Test stat on the logfile" << std::endl;
    res = stat("mount_dir/filesystem.log", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::accessTest() {

    std::cout << " -- access test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    int res = access("mount_dir", F_OK);
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    res = access("mount_dir/no_such_file", F_OK);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::opendirTest() {

    std::cout << " -- opendir test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    DIR *pdir;

    pdir = opendir("mount_dir");
    std::cout << "expect a large integer: " << (uint64_t) pdir << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    pdir = opendir("/no_such_dir");
    std::cout << "expect NULL: " << (uint64_t) pdir << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::readdirTest() {
    std::cout << " -- readdir test -- " << std::endl;

    zeroErrno();
    DIR *pdir;

    // test on an existing file
    pdir = opendir("mount_dir");
    struct dirent *de;
    de = readdir(pdir);
    std::cout << "de->d_name: " << de->d_name << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    pdir = opendir("mount_dir/no_such_file");
    std::cout << "de->d_name: " << de->d_name << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::openTest() {
    std::cout << " -- open test -- " << std::endl;

    zeroErrno();

    int res;

    // test on an existing file
    std::cout << "Test opening an existing file" << std::endl;
    res = open("mount_dir/a_file.txt", OPEN_FLAGS); // todo check if the returned 5 is an error
    std::cout << "expect a small integer: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test on non existing file
    std::cout << "Test opening a non existing" << std::endl;
    res = open("mount_dir/open_a_file.txt", OPEN_FLAGS);
    std::cout << "expect -1 (no permissions to create a new file): " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test logfile reference
    std::cout << "Test opening the logfile" << std::endl;
    res = open("mount_dir/filesystem.log", OPEN_FLAGS);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();
}

void FuseTester::renamrTest() {

    std::cout << " -- rename test -- " << std::endl;

    int res = 0;
    zeroErrno();

    // test logfile reference
    std::cout << std::endl << "Rename the logfile" << std::endl;
    res = rename("mount_dir/filesystem.log", "mount_dir/new_name.log");
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test renaming existing file
    std::cout << std::endl << "Rename an existing file" << std::endl;
    res = rename("mount_dir/a_file.txt", "mount_dir/new_name.txt");
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test renaming a non existing file
    std::cout << std::endl << "Rename a non existing file" << std::endl;
    res = rename("mount_dir/no_such_file", "mount_dir/no_file_new_name.txt");
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();
}

CacheManager CacheTester::initTest() {
    std::cout<<"construct a CacheManager with parameters: 60, 0.4, 0.4" <<
            std::endl;
    return CacheManager(60, 0.4, 0.4);

}


int main ()
{
//    FuseTester fuseTester;
//
//    fuseTester.accessTest();
//    fuseTester.getattrTest();
//    fuseTester.readdirTest();
//    fuseTester.opendirTest();
//    fuseTester.openTest();
//    fuseTester.renamrTest();

//    CacheTester cacheTester;
//    CacheManager m = cacheTester.initTest();
//
//    m.insertBlock(1, 1, (char *) "test1", (char *) "path");
//    char * filePath = m.retrieveFilePath(std::pair<int, int>(1, 1));
//    std::cout<<"expected filePath: path, got: "<<filePath<<std::endl;
//    const char* buff = m.retrieveBuffer(std::pair<int, int>(1, 1));
//    if (buff == NULL){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected buff: test1, got: " << buff << std::endl;
//    }
//
//    m.insertBlock(2, 1, (char *) "test2", (char *) "path");
//    filePath = m.retrieveFilePath(std::pair<int, int>(2, 1));
//    std::cout<<"expected filePath: path, got: "<<filePath<<std::endl;
//    buff = m.retrieveBuffer(std::pair<int, int>(2, 1));
//    if (buff == NULL){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected buff: test2, got: " << buff << std::endl;
//    }
//
//    m.insertBlock(3, 1, (char *) "test3", (char *) "path");
//    filePath = m.retrieveFilePath(std::pair<int, int>(3, 1));
//    std::cout<<"expected filePath: path, got: "<<filePath<<std::endl;
//    buff = m.retrieveBuffer(std::pair<int, int>(3, 1));
//    if (buff == NULL){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected buff: test3, got: " << buff << std::endl;
//    }
//
//    m.insertBlock(4, 1, (char *) "test4", (char *) "path");
//    filePath = m.retrieveFilePath(std::pair<int, int>(4, 1));
//    std::cout<<"expected filePath: path, got: "<<filePath<<std::endl;
//    buff = m.retrieveBuffer(std::pair<int, int>(4, 1));
//    if (buff == NULL){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected buff: test4, got: " << buff << std::endl;
//    }
//
//    for (int i = 5; i < 2000; i++){
////        const char * buff2 = ("test" + std::to_string(i)).c_str();
//        const char * buff1 = "test";
//        m.insertBlock(i, 1, buff1, (char *) "anotherPath");
//    }
//    filePath = m.retrieveFilePath(std::pair<int, int>(1980, 1));
//    if (filePath == nullptr){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected filePath: anotherPath, got: " << filePath <<
//        std::endl;
//    }
//    buff = m.retrieveBuffer(std::pair<int, int>(1980, 1));
//    if (buff == NULL){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected buff: test, got: " << buff << std::endl;
//    }
//
//    m.updatePaths("anot", "newp");
//    char * filePath1 = m.retrieveFilePath(std::pair<int, int>(1999, 1));
//    if (filePath1 == nullptr){
//        std::cout<<"got null"<<std::endl;
//    } else {
//        std::cout << "expected filePath: newpherPath, got: " << filePath1 <<
//        std::endl;
//    }
//    std::cout<< m.cacheToString();

    int fd1 = open("/home/omri/Documents/ex4/Debug/mount_dir/html.lst", O_RDONLY);
    int fd2 = open("/home/omri/Documents/ex4/Debug/root_dir/html.lst", O_RDONLY);

    struct stat st1, st2;
    fstat(fd1, &st1);
    fstat(fd2, &st2);

    std::cout << "size 1: " << st1.st_size << std::endl;
    std::cout << "size 2: " << st2.st_size << std::endl;

    int offset = 1158500;
    size_t size = 5082;

//    int offset = 0;
//    size_t size = 1158581;

    char buf1[size];
    char buf2[size];

    lseek(fd1, offset, 0);
    lseek(fd2, offset, 0);

    read(fd1, buf1, size);
    read(fd2, buf2, size);

    std::cout << "buf1: " << buf1 << std::endl;
    std::cout << "buf2: " << buf2 << std::endl;

    std::cout << "memcmp: " << memcmp(buf1, buf2, size) << std::endl;

    return 0;
}
