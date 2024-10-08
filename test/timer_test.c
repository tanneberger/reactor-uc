#include "reactor-uc/reactor-uc.h"
#include "unity.h"

typedef struct {
  Timer super;
  Reaction *effects[1];
} MyTimer;

typedef struct {
  Reaction super;
} MyReaction;

struct MyReactor {
  Reactor super;
  MyReaction my_reaction;
  MyTimer timer;
  Reaction *_reactions[1];
  Trigger *_triggers[1];
};

int timer_handler(Reaction *_self) {
  struct MyReactor *self = (struct MyReactor *)_self->parent;
  printf("Hello World @ %ld\n", self->super.env->current_tag.time);
  return 0;
}

void MyReaction_ctor(MyReaction *self, Reactor *parent) {
  Reaction_ctor(&self->super, parent, timer_handler, NULL, 0, 0);
}

void MyReactor_ctor(struct MyReactor *self, Environment *env) {
  self->_reactions[0] = (Reaction *)&self->my_reaction;
  self->_triggers[0] = (Trigger *)&self->timer;
  Reactor_ctor(&self->super, "MyReactor", env, NULL, 0, self->_reactions, 1, self->_triggers, 1);
  MyReaction_ctor(&self->my_reaction, &self->super);
  Timer_ctor(&self->timer.super, &self->super, MSEC(0), MSEC(100), self->timer.effects, 1);
  self->timer.super.super.register_effect(&self->timer.super.super, &self->my_reaction.super);
}

void test_simple() {
  struct MyReactor my_reactor;
  Environment env;
  Environment_ctor(&env, (Reactor *)&my_reactor);
  env.set_stop_time(&env, SEC(1));
  MyReactor_ctor(&my_reactor, &env);
  env.assemble(&env);
  env.start(&env);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_simple);
  return UNITY_END();
}
