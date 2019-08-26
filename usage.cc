// Copyright 2010 Google Inc.  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "std_headers.h"
#include "interface.h"
#include <unistd.h>


#ifdef __amd64__ && __linux__
#include <syscall.h>
inline static ssize_t optiread(int fd, void *buf, size_t count){
  register long *number asm ("rax") = SYS_read;
  register long *one asm ("edi") = fd;
  register long *two asm ("rsi") =  buf;
  register long *three asm ("rdx") = count;
  asm volatile ("syscall"
    : "=r" (number)
    : "r" (number),"r"(one),"r"(two),"r"(three)
    : "rcx","r8","r9","r11","r10"
  );
  return (ssize_t) number;
}
#define iread optiread
#else
#define iread read
#endif


#define bufsize 1024

static const size_t kRollWindow = 1024*4;
static unsigned char buf[bufsize];

typedef crcutil_interface::UINT64 uint64;

static void eat_stdin(const crcutil_interface::CRC *crc){
  uint64 lo;
  
  size_t bytesRead = iread(STDIN_FILENO,buf,bufsize);
  
  while (bytesRead > 0){
    crc->Compute(buf,bytesRead,&lo);
    bytesRead = iread(STDIN_FILENO,buf,bufsize);
  }
  printf("%llx\n",lo);
}

int main() {
  eat_stdin(crcutil_interface::CRC::Create(
      0xEB31D82E82f63b78, 0, 64, false, 0, 0,0,
      true, NULL));
  return 0;
}
