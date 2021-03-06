//===--- ExponentialGrowthAppendingBinaryByteStream.h -----------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2018 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
/// Defines an \c llvm::WritableBinaryStream whose internal buffer grows
/// exponentially in size as data is written to it. It is thus more efficient
/// than llvm::AppendingBinaryByteStream if a lot of small data gets appended to
/// it.
///
//===----------------------------------------------------------------------===//

#ifndef POLARPHP_BASIC_EXPONENTIALGROWTHAPPENDINGBINARYBYTESTREAM_H
#define POLARPHP_BASIC_EXPONENTIALGROWTHAPPENDINGBINARYBYTESTREAM_H

#include "polarphp/basic/adt/ArrayRef.h"
#include "polarphp/utils/BinaryByteStream.h"

namespace polar::basic {

using polar::utils::WritableBinaryStream;
using polar::utils::BinaryStreamFlags;

/// An implementation of WritableBinaryStream which can write at its end
/// causing the underlying data to grow.  This class owns the underlying data.
class ExponentialGrowthAppendingBinaryByteStream
      : public WritableBinaryStream
{
public:
   ExponentialGrowthAppendingBinaryByteStream()
      : ExponentialGrowthAppendingBinaryByteStream(
           polar::utils::Endianness::Little)
   {}

   ExponentialGrowthAppendingBinaryByteStream(polar::utils::Endianness endian)
      : m_endian(endian)
   {}

   void reserve(size_t size);

   polar::utils::Endianness getEndian() const override
   {
      return m_endian;
   }

   polar::utils::Error readBytes(uint32_t offset, uint32_t size,
                                 ArrayRef<uint8_t> &buffer) override;

   polar::utils::Error readLongestContiguousChunk(uint32_t offset,
                                                  ArrayRef<uint8_t> &buffer) override;

   MutableArrayRef<uint8_t> data()
   {
      return m_data;
   }

   uint32_t getLength() override
   {
      return m_data.size();
   }

   polar::utils::Error writeBytes(uint32_t offset, ArrayRef<uint8_t> buffer) override;

   /// This is an optimized version of \c writeBytes that assumes we know the
   /// size of \p value at compile time (which in particular holds for integers).
   /// It does so by exposing the memcpy to the optimizer along with the size
   /// of the value being assigned; the compiler can then optimize the memcpy
   /// into a fixed set of instructions.
   /// This assumes that the endianess of this steam is the same as the native
   /// endianess on the executing machine. No endianess transformations are
   /// performed.
   template<typename T>
   polar::utils::Error writeRaw(uint32_t offset, T value)
   {
      if (auto error = checkOffsetForWrite(offset, sizeof(T))) {
         return error;
      }

      // Resize the internal buffer if needed.
      uint32_t RequiredSize = offset + sizeof(T);
      if (RequiredSize > m_data.size()) {
         m_data.resize(RequiredSize);
      }

      ::memcpy(m_data.data() + offset, &value, sizeof value);

      return polar::utils::Error::getSuccess();
   }

   polar::utils::Error commit() override
   {
      return polar::utils::Error::getSuccess();
   }

   virtual BinaryStreamFlags getFlags() const override
   {
      return BinaryStreamFlags::BSF_Write | BinaryStreamFlags::BSF_Append;
   }

private:
   /// The buffer holding the data.
   SmallVector<uint8_t, 0> m_data;
   polar::utils::Endianness m_endian;
};

} // polar::basic

#endif // POLARPHP_BASIC_EXPONENTIALGROWTHAPPENDINGBINARYBYTESTREAM_H
