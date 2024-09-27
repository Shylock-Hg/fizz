/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/crypto/Hasher.h>
#include <fizz/record/Types.h>

namespace fizz {

/**
 * Keeps track of the handshake transcript and provides access to the
 * handshake context.
 */
class HandshakeContext {
 public:
  virtual ~HandshakeContext() = default;

  /**
   * Appends transcript to the current handshake transcript.
   */
  virtual void appendToTranscript(const Buf& transcript) = 0;

  /**
   * Returns the handshake context for the current transcript.
   */
  virtual Buf getHandshakeContext() const = 0;

  /**
   * Returns the finished verify_data from the current handshake context and
   * baseKey.
   */
  virtual Buf getFinishedData(folly::ByteRange baseKey) const = 0;

  /**
   * Returns the handshake context for an empty transcript.
   */
  virtual folly::ByteRange getBlankContext() const = 0;

  virtual std::unique_ptr<HandshakeContext> clone() const = 0;
};

class HandshakeContextImpl : public HandshakeContext {
 public:
  explicit HandshakeContextImpl(const HasherFactoryWithMetadata* makeHasher)
      : HandshakeContextImpl(makeHasher, makeHasher->make()) {}

  HandshakeContextImpl(
      const HasherFactoryWithMetadata* makeHasher,
      std::unique_ptr<Hasher> hashState)
      : makeHasher_(makeHasher), hashState_(std::move(hashState)) {}

  void appendToTranscript(const Buf& data) override;

  Buf getHandshakeContext() const override;

  Buf getFinishedData(folly::ByteRange baseKey) const override;

  folly::ByteRange getBlankContext() const override {
    return makeHasher_->blankHash();
  }

  virtual std::unique_ptr<HandshakeContext> clone() const override {
    return std::make_unique<HandshakeContextImpl>(
        makeHasher_, hashState_->clone());
  }

 private:
  const HasherFactoryWithMetadata* makeHasher_;
  std::unique_ptr<Hasher> hashState_;
};
} // namespace fizz
