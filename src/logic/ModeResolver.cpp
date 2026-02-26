#include "ModeResolver.h"

ResolvedMode ModeResolver::resolve(const Inputs& in) const {
  ResolvedMode m;
  m.tailEnabled = true;           // always
  m.brakeEnabled = in.brake;
  m.turnEnabled  = in.turn;
  return m;
}