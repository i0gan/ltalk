#pragma once
#include <string>
#include <malloc.h>
#include <string.h>
#include <iostream>
#define VESSEL_DEFAULT_SIZE 1024
#define VESSEL_DEFAULT_ALIGN 256

namespace Util {
class Vessel {
public:
    Vessel() :
        sub_(false),
        size_(0),
        capacity_(align(VESSEL_DEFAULT_SIZE)),
        data_ptr(static_cast<char*>(malloc(align(VESSEL_DEFAULT_SIZE))))
    {
        data_start_ptr = data_ptr;
    }
    ~Vessel() {
        free(data_ptr);
    };
    void operator<<(std::string data) {
        size_t size =  data.size();
        if(sub_ || size == 0) {
            std::cout << "data subed, can't append\n";
            return;
        }
        if((capacity_ - size_) < size) {
            std::cout << "realloc\n";
           size_t align_size = align(capacity_ + size);
           void* ret_ptr = realloc(data_ptr, align_size);
           if(ret_ptr == nullptr) {
               std::cout << "data realloc, can't append\n";
               return;;
           }
           data_ptr = static_cast<char *>(ret_ptr);
           data_start_ptr = data_ptr;
           capacity_ = align_size;
        }
        memcpy(data_start_ptr + size_, data.data(), size);
        size_ += size;
    }

    void operator<<(const char *data) {
        size_t size =  strlen(data);
        if(sub_ || size == 0) {
            std::cout << "data subed, can't append\n";
            return;
        }
        if((capacity_ - size_) < size) {
           size_t align_size = align(capacity_ + size);
           void* ret_ptr = realloc(data_ptr, align_size);
           if(ret_ptr == nullptr) {
               std::cout << "data realloc, can't append\n";
               return;;
           }
           data_ptr = static_cast<char *>(ret_ptr);
           data_start_ptr = data_ptr;
           capacity_ = align_size;
        }
        memcpy(data_start_ptr + size_, data, size);
        size_ += size;
    }

    void append(void *data, size_t length) {
        size_t size = length;
        if(sub_ || size == 0) {
            std::cout << "data subed, can't append\n";
            return;
        }
        if((capacity_ - size_) < size) {
           size_t align_size = align(capacity_ + size);
           void* ret_ptr = realloc(data_ptr, align_size);
           if(ret_ptr == nullptr) {
               std::cout << "data realloc, can't append\n";
               return;;
           }
           data_ptr = static_cast<char *>(ret_ptr);
           data_start_ptr = data_ptr;
           capacity_ = align_size;
        }
        memcpy(data_start_ptr + size_, data, size);
        size_ += size;
    }


    void sub(size_t size) {
        sub_ = true;
        if(size_ < size) {
            std::cout << "sub data : out of range\n";
            size_ = 0;
        }else
            size_ -= size;
        data_start_ptr += size;
    }

    void clear() {
        sub_ = false;
        size_ = 0;
        data_start_ptr = data_ptr;
    }
    char *data() {
        return data_start_ptr;
    }

    size_t size() {
        return size_;
    }
    size_t capacity() {
        return capacity_;
    }
private:
    bool sub_;
    size_t size_;
    size_t capacity_;
    char *data_ptr;
    char *data_start_ptr;

    size_t align(size_t size) {
        size_t n = size / VESSEL_DEFAULT_ALIGN;
        if(size % VESSEL_DEFAULT_ALIGN == 0)
            return n * VESSEL_DEFAULT_ALIGN;
        return  (n + 1) * VESSEL_DEFAULT_ALIGN;
    }
};
}
