// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.rand;

import std.core;

export namespace util {

void set_rand_seed(uint64_t seed);
void reset_rng();
uint8_t get_random_byte();
uint32_t Random();
uint64_t Random64();

} // namespace util
