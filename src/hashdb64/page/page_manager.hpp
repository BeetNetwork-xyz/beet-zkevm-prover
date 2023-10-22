#ifndef PAGE_MANAGER_HPP
#define PAGE_MANAGER_HPP

#include <cstdint>
#include "zklog.hpp"
#include "exit_process.hpp"
#include "zkresult.hpp"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include "zkassert.hpp"
#include <cassert>
#include <unistd.h>

#define MULTIPLE_WRITES 0

class PageContext;
class PageManager
{
public:

    PageManager();
    ~PageManager();

    zkresult init(PageContext &ctx);
    uint64_t getFreePage();
    void releasePage(const uint64_t pageNumber);
    uint64_t editPage(const uint64_t pageNumber);
    void flushPages(PageContext &ctx);
    inline char *getPageAddress(const uint64_t pageNumber);
    inline uint64_t getNumFreePages();

    zkresult addFile();
    zkresult addPages(const uint64_t nPages_);

    inline void readLock(){ headerLock.lock_shared();}
    inline void readUnlock(){ headerLock.unlock_shared();}

private:

    bool mappedFile;
    string fileName;
    string folderName;
    uint64_t fileSize;
    uint64_t pagesPerFile;
    uint64_t nFiles;

    shared_mutex pagesLock;
    uint64_t nPages;
    vector<char *> pages;

    mutex freePagesLock;
    uint64_t firstUnusedPage;
    uint64_t numFreePages;
    vector<uint64_t> freePages;

    mutex editedPagesLock;
    unordered_map<uint64_t, uint64_t> editedPages;

    shared_mutex headerLock;

};

char* PageManager::getPageAddress(const uint64_t pageNumber)
{
    shared_lock<shared_mutex> guard_pages(pagesLock);
    zkassertpermanent(pageNumber < nPages);
    uint64_t fileId = pageNumber/pagesPerFile;
    uint64_t pageInFile = pageNumber % pagesPerFile;
    return pages[fileId] + pageInFile * (uint64_t)4096;
};

//Note: if there is a single writter thread we assume that only the writter thread will call this function! 
uint64_t PageManager::getNumFreePages(){
#if MULTIPLE_WRITES
        lock_guard<mutex> guard_freePages(freePagesLock);
        shared_lock<shared_mutex> guard_pages(pagesLock);
#endif
        return numFreePages+nPages-firstUnusedPage;
    };
#endif