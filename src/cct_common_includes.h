/*
 *  Copyright 2015 Jordon Biondo
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef CCT_COMMON_INCLUDES_H
#define CCT_COMMON_INCLUDES_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define cct_alloc(type, count) ((type*)(malloc(sizeof(type) * (count))))

#ifndef CCT_DUMMY_TYPE
typedef float* dummy_type; // DUMMY_TYPE_DECLARATION
#else
typedef DUMMY_TYPE dummy_type;
#endif

#endif
