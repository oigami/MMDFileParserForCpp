#pragma once
#include <vector>
#include <fstream>
#include <memory>

namespace MMDFileParser
{
  namespace math
  {
    template<class T> struct Vector2D
    {
      union
      {
        struct
        {
          T x, y;
        };
        T arr[2];
      };

      T& operator[](int i) { return arr[i]; }
      const T& operator[](int i) const { return arr[i]; }
    };

    template<class T> struct Vector3D
    {
      union
      {
        struct
        {
          T x, y, z;
        };
        T arr[3];
      };

      T& operator[](int i) { return arr[i]; }
      const T& operator[](int i) const { return arr[i]; }
    };

    template<class T> struct Vector4D
    {
      union
      {
        struct
        {
          T x, y, z, w;
        };
        T arr[4];
      };

      T& operator[](int i) { return arr[i]; }
      const T& operator[](int i) const { return arr[i]; }
    };



    using Float2 = Vector2D<float>;
    static_assert(sizeof(Float2) == sizeof(float) * 2, "");

    using Float3 = Vector3D<float>;
    static_assert(sizeof(Float3) == sizeof(float) * 3, "");

    using Float4 = Vector4D<float>;
    static_assert(sizeof(Float4) == sizeof(float) * 4, "");

  }

  class IReader
  {
  public:

    virtual size_t read(void* buf, int size) = 0;
    virtual size_t getPos() const = 0;

  protected:

    ~IReader() = default;
  };

  class IWriter
  {
  public:

    virtual bool write(const void* buf, int size) = 0;
    virtual size_t getPos() const = 0;

  protected:
    virtual ~IWriter() = default;
  };

  using String = std::string;
  using FilePath = String;

  class BinaryReader : public IReader
  {
    std::shared_ptr<std::istream> stream_;
  public:

    virtual ~BinaryReader() = default;

    explicit BinaryReader(const FilePath& path);

    template<class Stream, std::enable_if_t<std::is_base_of<std::istream, Stream>::value>* = nullptr>
    explicit BinaryReader(Stream&& stream) : stream_(std::make_shared<Stream>(std::move(stream))) {}

    size_t read(void* buf, int size) override;

    size_t getPos() const override;
  };

  class BinaryWriter : public IWriter
  {
    std::shared_ptr<std::ostream> stream_;

  public:
    template<class Stream, std::enable_if_t<std::is_base_of<std::ostream, Stream>::value>* = nullptr>
    explicit BinaryWriter(Stream&& stream) : stream_(std::make_shared<Stream>(std::move(stream))) {}

    explicit BinaryWriter(const FilePath& path);

    bool write(const void* buf, int size) override;
    size_t getPos() const override;
  };


  template<class...T> using Array = std::vector<T...>;


}
