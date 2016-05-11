#ifndef ARRAY_H
#define ARRAY_H

#include <cstring>
#include <stdexcept>

#include "types.hpp"

namespace fds
{
  // Array container implemantetion based on byte array as a storage.
  template <typename T>
  class array
  {
  public:
    array(size_t capacity = 0);
    array(const array<T>& other); 
    array(array<T>&& other); 

    virtual ~array();

    array<T>& operator=(const array<T>& rhs);
    array<T>& operator=(array<T>&& rhs);

    void push_back(T& value);
    void push_back(const T& value);

    void pop_back();

    T& operator[](size_t pos);
    //const T& operator[](int pos);

    size_t size() const { return m_size; }

    // Iterator
    class iterator
    {
    public:
      iterator(byte_t* current) : m_current(current){}
      T& operator*() { return *reinterpret_cast<T*>(m_current); }
      T& operator++() { m_current += sizeof(T); return *reinterpret_cast<T*>(m_current); }
      bool operator!=(const iterator& rhs) { return m_current != rhs.m_current; }
      byte_t* current_pointer() { return m_current; }
    private:
      byte_t* m_current;
    };

    iterator begin() { return iterator(m_data); }
    iterator end() { return iterator(m_data+m_size*sizeof(T)); }

  private:
    size_t m_capacity;
    size_t m_size;
    byte_t* m_data;
  };


  template <typename T>
  array<T>::array(size_t capacity) : m_capacity(capacity), m_size(0), m_data(0)
  {
    m_data = capacity > 0 ? new byte_t[capacity*sizeof(T)] : nullptr;  
  }

  template <typename T>
  array<T>::array(const array<T>& other) 
  {
    if(this != &other) {
      m_size = other.m_size;
      m_capacity = other.m_capacity;
      m_data = m_capacity ? new byte_t[m_capacity*sizeof(T)] : nullptr;  
      if (m_data) 
	for (size_t i = 0; i < m_size*sizeof(T); i += sizeof(T)) 
	  *(new (m_data + i) T()) = *(reinterpret_cast<T*>(other.m_data + i));
	
    }
  }

  template <typename T>
  array<T>::array(array<T>&& other)
  {
    if(this != &other) {
      m_data = other.m_data;
      m_size = other.m_size;
      m_capacity = other.m_capacity;
      other.m_data = nullptr;
      other.m_size = 0;
      other.m_capacity = 0;
    }
  }
	
  template <typename T>
  array<T>::~array()
  {
    if (m_data) {
      for (size_t i = 0; i < m_size*sizeof(T); i += sizeof(T)) {
	T* obj = reinterpret_cast<T*>(m_data + i);
	obj->~T();
      }
      delete [] m_data;
    }
  }

  template <typename T>
  array<T>& array<T>::operator=(const array<T>& rhs)
  {
    if(this != &rhs) {
      if(m_data) {
	for (size_t i = 0; i < m_size*sizeof(T); i += sizeof(T)) {
	  T* obj = reinterpret_cast<T*>(m_data + i);
	  obj->~T();
	}
	delete [] m_data;
      }
      m_size = rhs.m_size;
      m_capacity = rhs.m_capacity;
      m_data = m_capacity > 0 ? new byte_t[m_capacity*sizeof(T)] : nullptr;  
      
      if (m_data) 
	for (size_t i = 0; i < m_size*sizeof(T); i += sizeof(T)) 
	  *(new (m_data + i) T()) = *(reinterpret_cast<T*>(rhs.m_data + i));
    }
    return *this;
  }

  template <typename T>
  array<T>& array<T>::operator=(array<T>&& rhs)
  {
    if(this != &rhs) {
      if(m_data) {
	for (size_t i = 0; i < m_size*sizeof(T); i += sizeof(T)) {
	  T* obj = reinterpret_cast<T*>(m_data + i);
	  obj->~T();
	}
	delete [] m_data;
      }
      m_data = rhs.m_data;
      m_size = rhs.m_size;
      m_capacity = rhs.m_capacity;
      rhs.m_data = nullptr;
      rhs.m_size = 0;
      rhs.m_capacity = 0;
    }
    return *this;
  }

  template <typename T>
  void array<T>::push_back(T& value)
  {
    if (m_size == m_capacity)
      throw std::range_error("size reached capacity");
    new(m_data + m_size*sizeof(T)) T(value); 
    m_size++;
  }

  template <typename T>
  void array<T>::push_back(const T& value)
  {
    push_back(const_cast<T&>(value));
  }

  template <typename T>
  void array<T>::pop_back()
  {
    if (m_size == 0)
      throw std::range_error("size is 0");

    T *obj = reinterpret_cast<T*>(m_data + (m_size-1)*sizeof(T));
    obj->~T();

    m_size--;
  }

  template <typename T>
  T& array<T>::operator[](size_t pos)
  {
    if (pos < 0 || pos >= m_size)
      throw std::range_error("wrong position");
      
    T *obj = reinterpret_cast<T*>(m_data + pos*sizeof(T));
    return *obj;
  }

  #if 0
  template <typename T>
  const T& array<T>::operator[](int pos)
  {
    if (pos < 0 || pos >= m_size)
      thtow std::range_error();
      
    T *obj = reinterpret_cast<T*>(m_data + pos*sizeof(T));
    return *obj;
  }
  #endif
}

#endif
