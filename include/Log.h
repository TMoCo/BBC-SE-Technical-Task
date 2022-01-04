/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* Log class adapted from the imgui demo:
* https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
*/

#ifndef LOG_H
#define LOG_H 1

#include <error.h>
#include <imgui.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>

class Log
{
  Log()
  {
    clear();
  }

public:
  static Log* get()
  {
    static Log log;
    return &log;
  }

  void clear()
  {
    buffer.clear();
    offsets = { 0 };
  }

  static void add(const char* format, ...) // variadic 
  {
    Log* log = get();

    int oldSize = log->buffer.size();
    va_list args;
    __crt_va_start(args, format);
    log->buffer.appendfv(format, args);
    // _vfprintf_p(stdout, format, args);
    __crt_va_end(args);

    int newSize = log->buffer.size();
    while (oldSize < newSize)
    {
      if (log->buffer[oldSize] == '\n')
      {
        log->offsets.push_back(oldSize + 1);
      }
      oldSize++;
    }

  }

  static void draw(bool newWindow = true, const char* name = "Log")
  {
    Log* log = get();

    int size = (int)log->offsets.size();

    ImGuiListClipper clipper; // clip the buffer string
    clipper.Begin(size);
    
    const char* begin = log->buffer.begin();
    const char* end = log->buffer.end();
    
    while (clipper.Step())
    {
      for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
      {
        const char* line_start = begin + log->offsets[line_no];
        const char* line_end = (line_no + 1 < size) ? (begin + log->offsets[line_no + 1] - 1) : end;
        ImGui::TextUnformatted(line_start, line_end);
      }
    }
    clipper.End();
  }

  static void writeLog()
  {
    Log* log = get();

    std::ofstream out("log.txt");

    if (!out.is_open())
    {
      ERROR_MSG("Failed to open log file.", __FILE__, __LINE__)
      return;
    }

    out << log->buffer.c_str();

    out.close();
  }

private:
  ImGuiTextBuffer buffer;
  std::vector<uint32_t> offsets;
};

#endif // !LOG_H
