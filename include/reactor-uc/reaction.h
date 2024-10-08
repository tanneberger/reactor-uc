#ifndef REACTOR_UC_REACTION_H
#define REACTOR_UC_REACTION_H

#include "reactor-uc/tag.h"
#include <stdbool.h>

typedef struct Reaction Reaction;
typedef struct Reactor Reactor;
typedef struct Trigger Trigger;

typedef int (*ReactionHandler)(Reaction *);

struct Reaction {
  Reactor *parent;
  ReactionHandler body;
  int level; // Negative level means it is invalid.
  size_t index;
  Trigger **effects;
  size_t effects_size;
  size_t effects_registered;
  void (*register_effect)(Reaction *self, Trigger *effect);
  size_t (*calculate_level)(Reaction *self);
  size_t (*get_level)(Reaction *self);
};

void Reaction_ctor(Reaction *self, Reactor *parent, ReactionHandler body, Trigger **effects, size_t effects_size,
                   size_t index);

#endif
