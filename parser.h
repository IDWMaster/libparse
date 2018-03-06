/*
Copyright 2018 Brian Bosak

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef LIBPARSE_ERRWATT
#define LIBPARSE_ERRWATT
#include <string.h>
#include <string>


namespace libparse {
  
  class StringRef {
  public:
    const char* ptr;
    size_t count;
    operator std::string() const {
      return std::string(ptr,count);
    }
    bool operator<(const StringRef& other) const {
      size_t c = other.count < count ? other.count : count;
      for(size_t i = 0;i<c;i++) {
	if(ptr[i]<other.ptr[i]) {
	  return true;
	}else {
	  if(ptr[i]>other.ptr[i]) {
	    return false;
	  }
	}
      }
      return count < other.count;
    }
    StringRef() {
      count = 0;
    }
    template<typename... pack>
    bool in(int& match,pack... expectations) {
      const char* values[sizeof...(pack)] = {expectations...};
      for(size_t i = 0;i<sizeof...(pack);i++) {
	const char* ptr = values[i];
	const char* compare = this->ptr;
	bool matched = true;
	for(size_t c = 0;ptr[c];c++ /*This is how C++ was invented*/) {
	  if((ptr[c] != compare[c]) || c == count) {
	    matched = false;
	    break;
	  }
	}
	
	if(matched) {
	  match = i;
	  return true;
	}
      }
      
	return false;
    }
    StringRef(const char* ptr, size_t count):ptr(ptr),count(count) {
    }
    StringRef(const char* ptr):ptr(ptr) {
      count = strlen(ptr);
    }
  };
  
  

class ParseTree {
public:
  const char* ptr;
  ParseTree(const char* code) {
    ptr = code;
  }
  
#define BEGIN out.ptr = ptr;while(*ptr != 0) {
#define END ptr++;}return false;
#define RT out.count = (size_t)(ptr-out.ptr);return true;
  
  ///@summary Expects a sequence of characters.
  ///@returns TRUE if the specified character was found, otherwise false
  ///@param expectations A list of characters to expect
  ///@param out The string
  ///@param found The character which was found
  template<typename... pack>
  bool expect(StringRef& out, char& found, pack... expectations) {
    const size_t count = sizeof...(pack);
    char value[sizeof...(pack)] = {expectations...};
    out.ptr = ptr;
    while(*ptr != 0) {
      for(size_t i = 0;i<count;i++) {
	if(*ptr == value[i]) {
	  found = *ptr;
	  RT
	}
      }
      ptr++;
      
    }
    return false;
  }
  void skipWhitespace() {
    while(ptr != 0) {
    if(isspace(*ptr)) {
      ptr++;
    }else {
      return;
    }
    }
  }
  ///@summary Expects whitespace
  ///@param out The string before the whitespace.
  ///@returns Whether or not whitespace was found.
  bool expectWhitespace(StringRef& out) {
    BEGIN
    if(isspace(*ptr)) {
	RT
      }
    END
  }
  
  ///@summary Custom expect
  ///@param functor The function to evaluate each character of a string
  ///@param out Output string
  ///@returns true if matched, false otherwise.
  template<typename F>
  bool expect(const F& functor, StringRef& out) {
    BEGIN
      if(functor(*ptr)) {
	RT
      }
    END
  }
  template<typename F>
  bool scan(const F& functor, StringRef& out) {
    BEGIN
      if(!functor(*ptr)) {
	RT
      }
    END
  }
#undef RT
#undef BEGIN
#undef END
};


}
#endif
