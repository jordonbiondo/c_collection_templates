#ifndef CCT_TEST_COMMON_H
#define CCT_TEST_COMMON_H

#include "../collection_templates.h"

bool spoof_oom = false;

#ifdef cct_alloc
#undef cct_alloc
#define cct_alloc(type, count) ((spoof_oom) ? NULL : ((type*)(malloc(sizeof(type) * (count)))))
#endif

#define with_spoofed_oom for(spoof_oom = true; spoof_oom; spoof_oom = false)

#endif
