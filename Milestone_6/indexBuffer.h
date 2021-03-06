#pragma once

#include "pch.h"


class IndexBuffer {
private:
  unsigned int ID;
public:
  unsigned int count;
  IndexBuffer(const void* indices, unsigned int count);
  ~IndexBuffer();
  void Bind() const;
  void Unbind() const;
};