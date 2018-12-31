// File/File_SDL.hpp
/*********************************************************************************
MIT License

Copyright (c) 2016 Aaron McKinney

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/

#ifndef FILE_SDL_HPP
#define FILE_SDL_HPP

#include <SDL2/SDL_rwops.h>
#include "File_Struct.hpp"

class File
{
public:
  File(): f_file(NULL) {}
  ~File() { this->close(); }
  
  bool load(const char* filename, const char* mode) // loads a file mode can be any of the usual modes returns true on success
  {
    this->close();
    f_file = SDL_RWFromFile(filename, mode);
    return this->isOpen();
  }

  bool isOpen() // simply checks to see if the file is open returns true if open
  {
    if(f_file == NULL)
      return false;
    return true;
  }

  bool read(char* buffer, int size, int numread = 1) // reads from the file
  {
    if(SDL_RWread(f_file, buffer, size, numread) > 0)
      return true;
    return false;
  }

  char getC() // gets the next character
  {
    char c;
    this->read(&c, 1, 1);
    return c;
  }

  char peekC() // gets the next character without changing the position in the file
  {
    char c = this->getC();
    if(this->seek(-1, FILE_CUR))
      return c;
  }

  bool seek(size_t pos, int from) // seeks a position from the desired point
  {
    if(!this->isOpen())
      return false;
    if(SDL_RWseek(f_file, pos, from) == -1)
      return false;
    return true;
  }

  bool write(char* buffer, int size, int numwrite = 1) // writes to the file
  {
    if(SDL_RWwrite(f_file, buffer, size, numwrite) < numwrite)
    {
      return false;
    }
    return true;
  }

  void close() // closes the file if the file is open
  {
    if(this->isOpen())
    {
      SDL_RWclose(f_file);
      f_file = NULL;
    }
  }

  size_t getSize()
  {
    return SDL_RWsize(f_file);
  }

  f_File* getFilePtr() // returns a pointer to the file object
  {
    return f_file;
  }

  const char* getError() // returns a pointer to the error contents
  {
    return SDL_GetError();
  }
private:
  f_File* f_file;

};

#endif // FILE_SDL_HPP
