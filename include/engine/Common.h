/** -----------------------------------------------------------------------------
  * FILE:    Common.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common types, classes, aliases and definitions
  * -----------------------------------------------------------------------------
  */

#pragma once

#include <mutex>

using MutexLock = std::unique_lock< std::mutex >;
using MutexLockGuard = std::lock_guard< std::mutex >;
//C++17
//using MutexScopedLock = std::scoped_lock< std::mutex >;

