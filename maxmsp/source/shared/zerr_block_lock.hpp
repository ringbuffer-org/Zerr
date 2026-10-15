/**
 * @file    zerr_block_lock.hpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   One-block mutual exclusion between perform() and prepare() in the Max wrappers
 * @date    2026-10-13
 *
 * @copyright  Copyright (c) 2023-2026 Zeyu Yang
 * @license    MIT license
 *
 * Max can compile a new DSP chain while the old one keeps running, so an object's dsp64
 * (main thread) may be called while its perform (audio thread) is inside a block. Anything
 * prepare() resizes or swaps must therefore be kept away from a running perform(). The
 * lock is a single atomic flag: perform() tries once and outputs silence if prepare() holds
 * it; prepare() waits for the current block to finish, which takes microseconds.
 */
#pragma once

#include <atomic>
#include <thread>

namespace zerr_max {

class BlockLock {
  public:
    /** @brief Take the lock if free; never waits (audio thread) */
    bool tryAcquire() noexcept { return !held.exchange(true, std::memory_order_acquire); }

    /** @brief Wait for the lock (main thread); the holder is at most one block away */
    void acquire() noexcept
    {
        while (held.exchange(true, std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void release() noexcept { held.store(false, std::memory_order_release); }

  private:
    std::atomic<bool> held{false};
};

/** @brief Scoped tryAcquire(); `ok` says whether the block may be processed */
class TryBlockGuard {
  public:
    explicit TryBlockGuard(BlockLock& lock) noexcept : lock{lock}, ok{lock.tryAcquire()} {}
    ~TryBlockGuard()
    {
        if (ok) {
            lock.release();
        }
    }
    TryBlockGuard(const TryBlockGuard&)            = delete;
    TryBlockGuard& operator=(const TryBlockGuard&) = delete;

    const bool ok;

  private:
    BlockLock& lock;
};

/** @brief Scoped acquire(); for the resize section of prepare() */
class BlockGuard {
  public:
    explicit BlockGuard(BlockLock& lock) noexcept : lock{lock} { lock.acquire(); }
    ~BlockGuard() { lock.release(); }
    BlockGuard(const BlockGuard&)            = delete;
    BlockGuard& operator=(const BlockGuard&) = delete;

  private:
    BlockLock& lock;
};

} // namespace zerr_max
