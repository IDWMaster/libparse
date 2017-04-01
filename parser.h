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
    StringRef() {
      count = 0;
    }
    StringRef(const char* ptr, size_t count):ptr(ptr),count(count) {
    }
  };
  
  
  class Node {
public:
  StringRef position;
  Node* parent;
  Node(const StringRef& position, Node* parent):position(position),parent(parent) {
    
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
#define RT out.count = (size_t)(ptr-out.ptr);ptr++;return true;
  
  ///@summary Expects a sequence of characters.
  ///@returns TRUE if the specified character was found, otherwise false
  ///@param expectations A list of characters to expect
  ///@param out The string
  ///@param found The character which was found
  template<typename... pack>
  bool expect(pack... expectations, StringRef& out, char& found) {
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
#undef RT
#undef BEGIN
#undef END
};


}
#endif