#ifndef RTSPRESTREAMER_H
#define RTSPRESTREAMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Error.h"
#include "Types.h"
#include "State.h"

typedef struct RtspRestreamer RtspRestreamer;

RtspRestreamer* RtspRestreamerCreate(const RtspRoute route);
void RtspRestreamerDestroy(RtspRestreamer* restreamer);

Error RtspRestreamerStart(RtspRestreamer *restreamer);
void RtspRestreamerStop(RtspRestreamer *restreamer);

State RtspRestreamerGetInputState(RtspRestreamer *restreamer);
State RtspRestreamerGetOutputState(RtspRestreamer *restreamer);

Boolean RtspRestreamerCompareRoute(RtspRestreamer *restreamer, const RtspRoute route);

#ifdef __cplusplus
}
#endif

#endif