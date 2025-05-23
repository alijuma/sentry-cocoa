// Adapted from: https://github.com/kstenerud/KSCrash
//
//  SentryCrashMonitor_CPPException.c
//
//  Copyright (c) 2012 Karl Stenerud. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall remain in place
// in this source code.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "SentryCrashMonitor_CPPException.h"
#include "SentryCrashID.h"
#include "SentryCrashMachineContext.h"
#include "SentryCrashMonitorContext.h"
#include "SentryCrashStackCursor_SelfThread.h"
#include "SentryCrashThread.h"

// #define SentryCrashLogger_LocalLevel TRACE
#include "SentryCrashLogger.h"

#include <cxxabi.h>
#include <dlfcn.h>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>

#define STACKTRACE_BUFFER_LENGTH 30
#define DESCRIPTION_BUFFER_LENGTH 1000

// Compiler hints for "if" statements
#define likely_if(x) if (__builtin_expect(x, 1))
#define unlikely_if(x) if (__builtin_expect(x, 0))

// ============================================================================
#pragma mark - Globals -
// ============================================================================

/** True if this handler has been installed. */
static volatile bool g_isEnabled = false;

/** True if the handler should capture the next stack trace. */

// ============================================================================
#pragma mark - Callbacks -
// ============================================================================

// ============================================================================
#pragma mark - Public API -
// ============================================================================

static void
initialize(void)
{
    static bool isInitialized = false;
    if (!isInitialized) {
        isInitialized = true;
    }
}

static void
setEnabled(bool isEnabled)
{
    if (isEnabled != g_isEnabled) {
        g_isEnabled = isEnabled;
        if (isEnabled) {
            initialize();
        } else {
        }
    }
}

static bool
isEnabled(void)
{
    return g_isEnabled;
}

extern "C" SentryCrashMonitorAPI *
sentrycrashcm_cppexception_getAPI(void)
{
    static SentryCrashMonitorAPI api = { .setEnabled = setEnabled, .isEnabled = isEnabled };
    return &api;
}
