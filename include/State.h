#ifndef STATE_H
#define STATE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum State
{
    StateCreated,
    StateStarting,
    StateStarted,
    StateStopping,
    StateStopped,
    StateInterrupted
} State;

#ifdef __cplusplus
}
#endif

#endif // STATE_H
