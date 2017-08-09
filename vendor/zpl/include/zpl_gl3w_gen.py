#!/usr/bin/env python

# This software is dual-licensed to the public domain and under the following
# license: you are granted a perpetual, irrevocable license to copy, modify,
# publish, and distribute this file as you see fit.
#
# No warranty is implied, use at your own risk.
#

# gl3w generator module

# Allow Python 2.6+ to use the print() function
from __future__ import print_function

import re
import os

# Try to import Python 3 library urllib.request
# and if it fails, fall back to Python 2 urllib2
try:
    import urllib.request as urllib2
except ImportError:
    import urllib2

# UNLICENSE copyright header
UNLICENSE = br'''/*

ZPL - GL3W module

License:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.
    
Warranty:
    No warranty is implied, use at your own risk.

Usage:
    #define ZPL_GL3W_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

    #define ZPL_GL3W_IMPLEMENTATION
    #include"zpl_gl3w.h"

    Other files include just the header file.

Credits:
    Dominik Madarasz (GitHub: zaklaus)
    Ginger Bill (GitHub: gingerBill)
    Slavomir Kaslev (GitHub: skaslev)
    
Version History:
    1.00 - Initial version
*/

'''


# Download glcorearb.h
if not os.path.exists('glcorearb.h'):
    print('Downloading glcorearb.h...')
    web = urllib2.urlopen('https://www.opengl.org/registry/api/GL/glcorearb.h')
    with open('glcorearb.h', 'wb') as f:
        f.writelines(web.readlines())
else:
    print('Reusing glcorearb.h...')

# Parse function names from glcorearb.h
print('Parsing glcorearb.h header...')
procs = []
p = re.compile(r'GLAPI.*APIENTRY\s+(\w+)')
with open('glcorearb.h', 'r') as f:
    for line in f:
        m = p.match(line)
        if m:
            procs.append(m.group(1))
procs.sort()

def proc_t(proc):
    return { 'p': proc,
             'p_s': 'gl3w' + proc[2:],
             'p_t': 'PFN' + proc.upper() + 'PROC' }

# Generate gl3w.h
print('Generating zpl_gl3w.h...')
with open('zpl_gl3w.h', 'wb') as f:
    f.write(UNLICENSE)
    f.write(br'''#ifndef ZPL_INCLUDE_ZPL_GL3W_H
#define ZPL_INCLUDE_ZPL_GL3W_H

#include "glcorearb.h"

#ifndef __gl_h_
#define __gl_h_
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*zpl_gl3WglProc)(void);

/* gl3w api */
int zpl_gl3w_init(void);
int zpl_gl3w_is_supported(int major, int minor);
zpl_gl3WglProc zpl_gl3w_get_proc_address(char const *proc);

/* OpenGL functions */
''')
    for proc in procs:
        f.write('extern {0[p_t]: <52} {0[p_s]};\n'.format(proc_t(proc)).encode("utf-8"))
    f.write(b'\n')
    for proc in procs:
        f.write('#define {0[p]: <45} {0[p_s]}\n'.format(proc_t(proc)).encode("utf-8"))
    f.write(br'''
#ifdef __cplusplus
}
#endif

#endif

#if defined(ZPL_GL3W_IMPLEMENTATION) && !defined(ZPL_GL3W_IMPLEMENTATION_DONE)
#define ZPL_GL3W_IMPLEMENTATION_DONE

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN 1
#define WIN32_MEAN_AND_LEAN 1
#include <windows.h>

static HMODULE zpl__gl3w_libgl;

static void zpl__gl3w_open_libgl (void) { zpl__gl3w_libgl = LoadLibraryA("opengl32.dll"); }
static void zpl__gl3w_close_libgl(void) { FreeLibrary(zpl__gl3w_libgl); }

static zpl_gl3WglProc zpl__gl3w_get_proc(char const *proc)
{
	zpl_gl3WglProc res;

	res = (zpl_gl3WglProc) wglGetProcAddress(proc);
	if (!res)
		res = (zpl_gl3WglProc) GetProcAddress(zpl__gl3w_libgl, proc);
	return res;
}

#elif defined(__APPLE__) || defined(__APPLE_CC__)

#include <Carbon/Carbon.h>

CFBundleRef zpl__gl3w_bundle;
CFURLRef zpl__gl3w_bundleURL;

static void zpl__gl3w_open_libgl(void)
{
	zpl__gl3w_bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
		CFSTR("/System/Library/Frameworks/OpenGL.framework"),
		kCFURLPOSIXPathStyle, true);

	zpl__gl3w_bundle = CFBundleCreate(kCFAllocatorDefault, zpl__gl3w_bundleURL);
	assert(zpl__gl3w_bundle != NULL);
}

static void zpl__gl3w_close_libgl(void)
{
	CFRelease(zpl__gl3w_bundle);
	CFRelease(zpl__gl3w_bundleURL);
}

static GL3WglProc zpl__gl3w_get_proc(char const *proc)
{
	GL3WglProc res;

	CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, proc,
		kCFStringEncodingASCII);
	res = (GL3WglProc) CFBundleGetFunctionPointerForName(zpl__gl3w_bundle, procname);
	CFRelease(procname);
	return res;
}

#else

#include <dlfcn.h>
#include <GL/glx.h>

static void *zpl__gl3w_libgl;
static PFNGLXGETPROCADDRESSPROC zpl__gl3w_glx_get_proc_address;

static void zpl__gl3w_open_libgl(void)
{
	zpl__gl3w_libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
	zpl__gl3w_glx_get_proc_address = (PFNGLXGETPROCADDRESSPROC) dlsym(zpl__gl3w_libgl, "glXGetProcAddressARB");
}

static void zpl__gl3w_close_libgl(void) { dlclose(zpl__gl3w_libgl); }

static zpl_gl3WglProc zpl__gl3w_get_proc(char const *proc)
{
	zpl_gl3WglProc res;

	res = (zpl_gl3WglProc) zpl__gl3w_glx_get_proc_address((const GLubyte *) proc);
	if (!res)
		res = (zpl_gl3WglProc) dlsym(zpl__gl3w_libgl, proc);
	return res;
}

#endif

static struct {
	int major, minor;
} zpl__gl3w_version;

static int zpl__gl3w_parse_version(void)
{
	if (!glGetIntegerv)
		return -1;

	glGetIntegerv(GL_MAJOR_VERSION, &zpl__gl3w_version.major);
	glGetIntegerv(GL_MINOR_VERSION, &zpl__gl3w_version.minor);

	if (zpl__gl3w_version.major < 3)
		return -1;
	return 0;
}

static void zpl__gl3w_load_procs(void);

int zpl_gl3w_init(void)
{
	zpl__gl3w_open_libgl();
	zpl__gl3w_load_procs();
	zpl__gl3w_close_libgl();
	return zpl__gl3w_parse_version();
}

int zpl_gl3w_is_supported(int major, int minor)
{
	if (major < 3)
		return 0;
	if (zpl__gl3w_version.major == major)
		return zpl__gl3w_version.minor >= minor;
	return zpl__gl3w_version.major >= major;
}

zpl_gl3WglProc zpl_gl3w_get_proc_address(char const *proc)
{
	return zpl__gl3w_get_proc(proc);
}

''')
    for proc in procs:
        f.write('{0[p_t]: <52} {0[p_s]};\n'.format(proc_t(proc)).encode("utf-8"))
    f.write(br'''
static void zpl__gl3w_load_procs(void)
{
''')
    for proc in procs:
        f.write('\t{0[p_s]} = ({0[p_t]}) zpl__gl3w_get_proc("{0[p]}");\n'.format(proc_t(proc)).encode("utf-8"))
    f.write(b'''}


#endif /* ZPL_GL3W_IMPLEMENTATION */
''')
