// BufferedSequentialMemoryAllocator.cpp :
//
// Made by Nicolas Distefano, 10/06/2018.
//
// Implementation details:
//   • The class is a template that can use any value_type & size_t.
//   • Other details are in comments in the BufferedSequentialMemoryAllocator class.
//

// This memory allocator is a buffered-sequential allocator using a dynamically heap-allocated buffer. The reason  I made it this way is because
// if we're planning on using any container that uses this allocator, we'd want the buffer to live across other scopes.
// You WILL need to pass whichever container using this allocator in reference (&) when said container is passed as a parameter in a function (it makes the code more robust anyways).
template <class T>
class BufferedSequentialMemoryAllocator
{
public:
  using value_type = T; // needed as part of allocator_traits

  BufferedSequentialMemoryAllocator() noexcept(true) // default constructor
    : memoryPool(new value_type[MemoryPoolSize]) // dynamically allocate memory here, only when needed
  {
  };
  BufferedSequentialMemoryAllocator(const BufferedSequentialMemoryAllocator&) noexcept(true) {};
  BufferedSequentialMemoryAllocator(BufferedSequentialMemoryAllocator&&) noexcept(true) {};
  template <class U>
  explicit BufferedSequentialMemoryAllocator(const BufferedSequentialMemoryAllocator<U> &) noexcept(true) {}
  ~BufferedSequentialMemoryAllocator() noexcept(true)
  {
    delete [] memoryPool;
    memoryPool = nullptr;
  };
  value_type* allocate(size_t n) noexcept(true) // needed as part of allocator_traits
  {
    if ((memoryPool + n * sizeof(value_type)) > (memoryPool + MemoryPoolSize))
    {
      return static_cast<value_type*>(::operator new(n)); // intended leak, should probably log it somewhere... Adds necessary memory so it doesn't crash at least.
    }
    memoryPool += n * sizeof(value_type);
    return memoryPool;
  };
  void deallocate(value_type *address, size_t n) noexcept(true) // needed as part of allocator_traits
  {
    memoryPool -= n * sizeof(value_type);
  };

private:
  static const size_t MemoryPoolSize = 512 * 1024 * 1024;
  value_type* memoryPool;
};
