#pragma once
#include "Types.h"

class ModeResolver {
public:
  ResolvedMode resolve(const Inputs& in) const;
};