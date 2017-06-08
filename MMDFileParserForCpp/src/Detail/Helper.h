#pragma once
#include <MMDFileParser/ParserHelper.hpp>

namespace MMDFileParser
{

  namespace
  {
    namespace detail
    {

      template<class T, std::enable_if_t<std::is_trivially_copyable<T>::value>* = nullptr>
      size_t ReadArray(IReader& reader, int numOfArrayElements, std::vector<T>& arr)
      {
        arr.resize(numOfArrayElements);
        if ( numOfArrayElements == 0 ) return 0;
        return reader.read(arr.data(), sizeof(T) * numOfArrayElements);
      }

      /// <summary>
      /// 最初にsizeof(SizeType)だけ読み込みその値の配列分読み込む
      /// </summary>
      /// <remarks>
      /// サイズ0の場合も正常終了
      /// </remarks>
      /// <returns>
      /// empty   : サイズの読み込みに失敗
      /// それ以外 : データの読み込み成否
      /// </returns>
      template<class SizeType, class Type>
      bool ReadSizeAndArray(IReader& reader, std::vector<Type>& arr)
      {
        SizeType num;
        if ( Read(reader, num) == 0 ) return true;
        return ReadArray(reader, num, arr) == num * sizeof(Type);
      }
      template<class T, std::enable_if_t<std::is_trivially_copyable<T>::value>* = nullptr>
      bool Read(IReader& r, T& t)
      {
        return r.read(std::addressof(t), sizeof(T)) == sizeof(T);
      }


      template<class SizeType, class Type>
      bool WriteSizeAndArray(IWriter& writer, const std::vector<Type>& arr)
      {
        SizeType num = static_cast<SizeType>(arr.size());
        if ( !Write(writer, num) ) return false;
        const auto arr_size = sizeof(Type) * num;
        return writer.write(reinterpret_cast<const char*>(arr.data()), arr_size);
      }

      template<class T> bool Write(IWriter& w, const T& t)
      {
        static_assert(std::is_pod<T>::value, "");
        return w.write(reinterpret_cast<const char*>(&t), sizeof(T));
      }
    }
  }
}
