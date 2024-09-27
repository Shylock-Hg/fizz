/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/crypto/Crypto.h>
#include <fizz/crypto/Hasher.h>
#include <folly/io/IOBuf.h>

namespace fizz {
/**
 * An HKDF implementation conformant with
 * https://tools.ietf.org/html/rfc5869.
 *
 * outputBytes represents the number of bytes
 * required as output from the HKDF. Other
 * params are as defined by the RFC.
 */
class Hkdf {
 public:
  explicit Hkdf(const HasherFactoryWithMetadata* makeHasher)
      : makeHasher_(makeHasher) {}

  std::vector<uint8_t> extract(folly::ByteRange salt, folly::ByteRange ikm)
      const;

  std::unique_ptr<folly::IOBuf> expand(
      folly::ByteRange extractedKey,
      const folly::IOBuf& info,
      size_t outputBytes) const;

  std::unique_ptr<folly::IOBuf> hkdf(
      folly::ByteRange ikm,
      folly::ByteRange salt,
      const folly::IOBuf& info,
      size_t outputBytes) const;

  // TODO: Deprecate. Use hasher().hashLength()
  size_t hashLength() const {
    return makeHasher_->hashLength();
  }

  const HasherFactoryWithMetadata* hasher() const {
    return makeHasher_;
  }

 private:
  const HasherFactoryWithMetadata* makeHasher_;
};
} // namespace fizz
