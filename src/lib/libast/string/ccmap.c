/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1985-2008 AT&T Intellectual Property          *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
*                                                                      *
*                A copy of the License is available at                 *
*            http://www.opensource.org/licenses/cpl1.0.txt             *
*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                   Phong Vo <kpv@research.att.com>                    *
*                                                                      *
***********************************************************************/
#pragma prototyped

/*
 * Glenn Fowler
 * AT&T Research
 *
 * dynamic single byte character code maps
 */

#include <ast.h>
#include <ccode.h>

#if !GENERATE

/*
 * catenated CC_ASCII to CC_* character code map table
 * order determined by CC_* index definitions
 * old definition values/indices must not change!
 * each table is followed by its inverse
 */

static const unsigned char tab[] =
{

 /* ascii => ascii */

 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,

 /* ascii => ascii */

 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,

 /* ascii => ebcdic */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x9a, 0x6d,
 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0x5f, 0x07,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
 0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xe1,
 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
 0x49, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
 0x58, 0x59, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
 0x76, 0x77, 0x78, 0x80, 0x8a, 0x8b, 0x8c, 0x8d,
 0x8e, 0x8f, 0x90, 0x6a, 0x9b, 0x9c, 0x9d, 0x9e,
 0x9f, 0xa0, 0xaa, 0xab, 0xac, 0x4a, 0xae, 0xaf,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xa1, 0xbe, 0xbf,
 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xda, 0xdb,
 0xdc, 0xdd, 0xde, 0xdf, 0xea, 0xeb, 0xec, 0xed,
 0xee, 0xef, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,

 /* ebcdic => ascii */

 0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
 0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x9d, 0x85, 0x08, 0x87,
 0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x0a, 0x17, 0x1b,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
 0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
 0x20, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xd5, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x7e,
 0x2d, 0x2f, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0xcb, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1,
 0xc2, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xc3, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
 0xca, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0x5e, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
 0xd1, 0xe5, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xd2, 0xd3, 0xd4, 0x5b, 0xd6, 0xd7,
 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0x5d, 0xe6, 0xe7,
 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed,
 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3,
 0x5c, 0x9f, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,

 /* ascii => ebcdic-i */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x5f, 0x6d,
 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
 0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xe1,
 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
 0x49, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
 0x58, 0x59, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
 0x76, 0x77, 0x78, 0x80, 0x8a, 0x8b, 0x8c, 0x8d,
 0x8e, 0x8f, 0x90, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e,
 0x9f, 0xa0, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xda, 0xdb,
 0xdc, 0xdd, 0xde, 0xdf, 0xea, 0xeb, 0xec, 0xed,
 0xee, 0xef, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,

 /* ebcdic-i => ascii */

 0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
 0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x9d, 0x85, 0x08, 0x87,
 0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x0a, 0x17, 0x1b,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
 0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
 0x20, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0x4a, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
 0x2d, 0x2f, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0x6a, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1,
 0xc2, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xc3, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
 0xca, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
 0xd1, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xd2, 0xd3, 0xd4, 0x5b, 0xd6, 0xd7,
 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0x5d, 0xe6, 0xe7,
 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed,
 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3,
 0x5c, 0x9f, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,

 /* ascii => ebcdic-o */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x15, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x5f, 0x6d,
 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x06, 0x17,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
 0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xff,
 0x41, 0xaa, 0x4a, 0xb1, 0x9f, 0xb2, 0x6a, 0xb5,
 0xbb, 0xb4, 0x9a, 0x8a, 0xb0, 0xca, 0xaf, 0xbc,
 0x90, 0x8f, 0xea, 0xfa, 0xbe, 0xa0, 0xb6, 0xb3,
 0x9d, 0xda, 0x9b, 0x8b, 0xb7, 0xb8, 0xb9, 0xab,
 0x64, 0x65, 0x62, 0x66, 0x63, 0x67, 0x9e, 0x68,
 0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77,
 0xac, 0x69, 0xed, 0xee, 0xeb, 0xef, 0xec, 0xbf,
 0x80, 0xfd, 0xfe, 0xfb, 0xfc, 0xba, 0xae, 0x59,
 0x44, 0x45, 0x42, 0x46, 0x43, 0x47, 0x9c, 0x48,
 0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57,
 0x8c, 0x49, 0xcd, 0xce, 0xcb, 0xcf, 0xcc, 0xe1,
 0x70, 0xdd, 0xde, 0xdb, 0xdc, 0x8d, 0x8e, 0xdf,

 /* ebcdic-o => ascii */

 0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
 0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x9d, 0x0a, 0x08, 0x87,
 0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x17, 0x1b,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
 0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
 0x20, 0xa0, 0xe2, 0xe4, 0xe0, 0xe1, 0xe3, 0xe5,
 0xe7, 0xf1, 0xa2, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0xe9, 0xea, 0xeb, 0xe8, 0xed, 0xee, 0xef,
 0xec, 0xdf, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
 0x2d, 0x2f, 0xc2, 0xc4, 0xc0, 0xc1, 0xc3, 0xc5,
 0xc7, 0xd1, 0xa6, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xf8, 0xc9, 0xca, 0xcb, 0xc8, 0xcd, 0xce, 0xcf,
 0xcc, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xd8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xab, 0xbb, 0xf0, 0xfd, 0xfe, 0xb1,
 0xb0, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0xaa, 0xba, 0xe6, 0xb8, 0xc6, 0xa4,
 0xb5, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xa1, 0xbf, 0xd0, 0x5b, 0xde, 0xae,
 0xac, 0xa3, 0xa5, 0xb7, 0xa9, 0xa7, 0xb6, 0xbc,
 0xbd, 0xbe, 0xdd, 0xa8, 0xaf, 0x5d, 0xb4, 0xd7,
 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xad, 0xf4, 0xf6, 0xf2, 0xf3, 0xf5,
 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xb9, 0xfb, 0xfc, 0xf9, 0xfa, 0xff,
 0x5c, 0xf7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xb2, 0xd4, 0xd6, 0xd2, 0xd3, 0xd5,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xb3, 0xdb, 0xdc, 0xd9, 0xda, 0x9f,

 /* ascii => ebcdic-s */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x15, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xbb, 0xbc, 0xbd, 0x6a, 0x6d,
 0x4a, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xfb, 0x4f, 0xfd, 0xff, 0x07,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x06, 0x17,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
 0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0x5f,
 0x41, 0xaa, 0xb0, 0xb1, 0x9f, 0xb2, 0xd0, 0xb5,
 0x79, 0xb4, 0x9a, 0x8a, 0xba, 0xca, 0xaf, 0xa1,
 0x90, 0x8f, 0xea, 0xfa, 0xbe, 0xa0, 0xb6, 0xb3,
 0x9d, 0xda, 0x9b, 0x8b, 0xb7, 0xb8, 0xb9, 0xab,
 0x64, 0x65, 0x62, 0x66, 0x63, 0x67, 0x9e, 0x68,
 0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77,
 0xac, 0x69, 0xed, 0xee, 0xeb, 0xef, 0xec, 0xbf,
 0x80, 0xe0, 0xfe, 0xdd, 0xfc, 0xad, 0xae, 0x59,
 0x44, 0x45, 0x42, 0x46, 0x43, 0x47, 0x9c, 0x48,
 0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57,
 0x8c, 0x49, 0xcd, 0xce, 0xcb, 0xcf, 0xcc, 0xe1,
 0x70, 0xc0, 0xde, 0xdb, 0xdc, 0x8d, 0x8e, 0xdf,

 /* ebcdic-s => ascii */

 0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
 0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x9d, 0x0a, 0x08, 0x87,
 0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x17, 0x1b,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
 0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
 0x20, 0xa0, 0xe2, 0xe4, 0xe0, 0xe1, 0xe3, 0xe5,
 0xe7, 0xf1, 0x60, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0xe9, 0xea, 0xeb, 0xe8, 0xed, 0xee, 0xef,
 0xec, 0xdf, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x9f,
 0x2d, 0x2f, 0xc2, 0xc4, 0xc0, 0xc1, 0xc3, 0xc5,
 0xc7, 0xd1, 0x5e, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xf8, 0xc9, 0xca, 0xcb, 0xc8, 0xcd, 0xce, 0xcf,
 0xcc, 0xa8, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xd8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xab, 0xbb, 0xf0, 0xfd, 0xfe, 0xb1,
 0xb0, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0xaa, 0xba, 0xe6, 0xb8, 0xc6, 0xa4,
 0xb5, 0xaf, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xa1, 0xbf, 0xd0, 0xdd, 0xde, 0xae,
 0xa2, 0xa3, 0xa5, 0xb7, 0xa9, 0xa7, 0xb6, 0xbc,
 0xbd, 0xbe, 0xac, 0x5b, 0x5c, 0x5d, 0xb4, 0xd7,
 0xf9, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xad, 0xf4, 0xf6, 0xf2, 0xf3, 0xf5,
 0xa6, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xb9, 0xfb, 0xfc, 0xdb, 0xfa, 0xff,
 0xd9, 0xf7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xb2, 0xd4, 0xd6, 0xd2, 0xd3, 0xd5,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xb3, 0x7b, 0xdc, 0x7d, 0xda, 0x7e,

 /* ascii => ebcdic-h */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xba, 0xe0, 0xbb, 0xb0, 0x6d,
 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
 0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xff,
 0x41, 0xaa, 0x4a, 0xb1, 0x9f, 0xb2, 0x6a, 0xb5,
 0xbd, 0xb4, 0x9a, 0x8a, 0x5f, 0xca, 0xaf, 0xbc,
 0x90, 0x8f, 0xea, 0xfa, 0xbe, 0xa0, 0xb6, 0xb3,
 0x9d, 0xda, 0x9b, 0x8b, 0xb7, 0xb8, 0xb9, 0xab,
 0x64, 0x65, 0x62, 0x66, 0x63, 0x67, 0x9e, 0x68,
 0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77,
 0xac, 0x69, 0xed, 0xee, 0xeb, 0xef, 0xec, 0xbf,
 0x80, 0xfd, 0xfe, 0xfb, 0xfc, 0xad, 0xae, 0x59,
 0x44, 0x45, 0x42, 0x46, 0x43, 0x47, 0x9c, 0x48,
 0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57,
 0x8c, 0x49, 0xcd, 0xce, 0xcb, 0xcf, 0xcc, 0xe1,
 0x70, 0xdd, 0xde, 0xdb, 0xdc, 0x8d, 0x8e, 0xdf,

 /* ebcdic-h => ascii */

 0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
 0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x9d, 0x85, 0x08, 0x87,
 0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x0a, 0x17, 0x1b,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
 0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
 0x20, 0xa0, 0xe2, 0xe4, 0xe0, 0xe1, 0xe3, 0xe5,
 0xe7, 0xf1, 0xa2, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0xe9, 0xea, 0xeb, 0xe8, 0xed, 0xee, 0xef,
 0xec, 0xdf, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0xac,
 0x2d, 0x2f, 0xc2, 0xc4, 0xc0, 0xc1, 0xc3, 0xc5,
 0xc7, 0xd1, 0xa6, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xf8, 0xc9, 0xca, 0xcb, 0xc8, 0xcd, 0xce, 0xcf,
 0xcc, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xd8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xab, 0xbb, 0xf0, 0xfd, 0xfe, 0xb1,
 0xb0, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0xaa, 0xba, 0xe6, 0xb8, 0xc6, 0xa4,
 0xb5, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xa1, 0xbf, 0xd0, 0xdd, 0xde, 0xae,
 0x5e, 0xa3, 0xa5, 0xb7, 0xa9, 0xa7, 0xb6, 0xbc,
 0xbd, 0xbe, 0x5b, 0x5d, 0xaf, 0xa8, 0xb4, 0xd7,
 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xad, 0xf4, 0xf6, 0xf2, 0xf3, 0xf5,
 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xb9, 0xfb, 0xfc, 0xf9, 0xfa, 0xff,
 0x5c, 0xf7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xb2, 0xd4, 0xd6, 0xd2, 0xd3, 0xd5,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xb3, 0xdb, 0xdc, 0xd9, 0xda, 0x9f,

 /* ascii => ebcdic-m */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x15, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x5f, 0x6d,
 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x06, 0x17,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
 0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xdf,
 0x41, 0xaa, 0x4a, 0xb1, 0x9f, 0xb2, 0x6a, 0xb5,
 0xbb, 0xb4, 0x9a, 0x8a, 0xb0, 0xca, 0xaf, 0xbc,
 0x90, 0x8f, 0xea, 0xfa, 0xbe, 0xa0, 0xb6, 0xb3,
 0x9d, 0xda, 0x9b, 0x8b, 0xb7, 0xb8, 0xb9, 0xab,
 0x64, 0x65, 0x62, 0x66, 0x63, 0x67, 0x9e, 0x68,
 0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77,
 0xac, 0x69, 0xed, 0xee, 0xeb, 0xef, 0xec, 0xbf,
 0x80, 0xfd, 0xfe, 0xfb, 0xfc, 0xba, 0xae, 0x59,
 0x44, 0x45, 0x42, 0x46, 0x43, 0x47, 0x9c, 0x48,
 0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57,
 0x8c, 0x49, 0xcd, 0xce, 0xcb, 0xcf, 0xcc, 0xe1,
 0x70, 0xdd, 0xde, 0xdb, 0xdc, 0x8d, 0x8e, 0xff,

 /* ebcdic-m => ascii */

 0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
 0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x9d, 0x0a, 0x08, 0x87,
 0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x17, 0x1b,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
 0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
 0x20, 0xa0, 0xe2, 0xe4, 0xe0, 0xe1, 0xe3, 0xe5,
 0xe7, 0xf1, 0xa2, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0xe9, 0xea, 0xeb, 0xe8, 0xed, 0xee, 0xef,
 0xec, 0xdf, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
 0x2d, 0x2f, 0xc2, 0xc4, 0xc0, 0xc1, 0xc3, 0xc5,
 0xc7, 0xd1, 0xa6, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xf8, 0xc9, 0xca, 0xcb, 0xc8, 0xcd, 0xce, 0xcf,
 0xcc, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xd8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xab, 0xbb, 0xf0, 0xfd, 0xfe, 0xb1,
 0xb0, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0xaa, 0xba, 0xe6, 0xb8, 0xc6, 0xa4,
 0xb5, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xa1, 0xbf, 0xd0, 0x5b, 0xde, 0xae,
 0xac, 0xa3, 0xa5, 0xb7, 0xa9, 0xa7, 0xb6, 0xbc,
 0xbd, 0xbe, 0xdd, 0xa8, 0xaf, 0x5d, 0xb4, 0xd7,
 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xad, 0xf4, 0xf6, 0xf2, 0xf3, 0xf5,
 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xb9, 0xfb, 0xfc, 0xf9, 0xfa, 0x9f,
 0x5c, 0xf7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xb2, 0xd4, 0xd6, 0xd2, 0xd3, 0xd5,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xb3, 0xdb, 0xdc, 0xd9, 0xda, 0xff,

 /* ascii => ebcdic-u */

 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
 0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x9f,
 0x10, 0x11, 0x12, 0x13, 0xb6, 0xb5, 0x32, 0x26,
 0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
 0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x5f, 0x6d,
 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
 0x68, 0xdc, 0x51, 0x42, 0x43, 0x44, 0x47, 0x48,
 0x52, 0x53, 0x54, 0x57, 0x56, 0x58, 0x63, 0x67,
 0x71, 0x9c, 0x9e, 0xcb, 0xcc, 0xcd, 0xdb, 0xdd,
 0xdf, 0xec, 0xfc, 0xb0, 0xb1, 0xb2, 0x3e, 0xb4,
 0x45, 0x55, 0xce, 0xde, 0x49, 0x69, 0x9a, 0x9b,
 0xab, 0x0f, 0xba, 0xb8, 0xb7, 0xaa, 0x8a, 0x8b,
 0x3c, 0x3d, 0x62, 0x6a, 0x64, 0x65, 0x66, 0x20,
 0x21, 0x22, 0x70, 0x23, 0x72, 0x73, 0x74, 0xbe,
 0x76, 0x77, 0x78, 0x80, 0x24, 0x15, 0x8c, 0x8d,
 0x8e, 0x41, 0x06, 0x17, 0x28, 0x29, 0x9d, 0x2a,
 0x2b, 0x2c, 0x09, 0x0a, 0xac, 0x4a, 0xae, 0xaf,
 0x1b, 0x30, 0x31, 0xfa, 0x1a, 0x33, 0x34, 0x35,
 0x36, 0x59, 0x08, 0x38, 0xbc, 0x39, 0xa0, 0xbf,
 0xca, 0x3a, 0xfe, 0x3b, 0x04, 0xcf, 0xda, 0x14,
 0xe1, 0x8f, 0x46, 0x75, 0xfd, 0xeb, 0xee, 0xed,
 0x90, 0xef, 0xb3, 0xfb, 0xb9, 0xea, 0xbb, 0xff,

 /* ebcdic-u => ascii */

 0x00, 0x01, 0x02, 0x03, 0xec, 0x09, 0xca, 0x7f,
 0xe2, 0xd2, 0xd3, 0x0b, 0x0c, 0x0d, 0x0e, 0xa9,
 0x10, 0x11, 0x12, 0x13, 0xef, 0xc5, 0x08, 0xcb,
 0x18, 0x19, 0xdc, 0xd8, 0x1c, 0x1d, 0x1e, 0x1f,
 0xb7, 0xb8, 0xb9, 0xbb, 0xc4, 0x0a, 0x17, 0x1b,
 0xcc, 0xcd, 0xcf, 0xd0, 0xd1, 0x05, 0x06, 0x07,
 0xd9, 0xda, 0x16, 0xdd, 0xde, 0xdf, 0xe0, 0x04,
 0xe3, 0xe5, 0xe9, 0xeb, 0xb0, 0xb1, 0x9e, 0x1a,
 0x20, 0xc9, 0x83, 0x84, 0x85, 0xa0, 0xf2, 0x86,
 0x87, 0xa4, 0xd5, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
 0x26, 0x82, 0x88, 0x89, 0x8a, 0xa1, 0x8c, 0x8b,
 0x8d, 0xe1, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
 0x2d, 0x2f, 0xb2, 0x8e, 0xb4, 0xb5, 0xb6, 0x8f,
 0x80, 0xa5, 0xb3, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
 0xba, 0x90, 0xbc, 0xbd, 0xbe, 0xf3, 0xc0, 0xc1,
 0xc2, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
 0xc3, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 0x68, 0x69, 0xae, 0xaf, 0xc6, 0xc7, 0xc8, 0xf1,
 0xf8, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
 0x71, 0x72, 0xa6, 0xa7, 0x91, 0xce, 0x92, 0x0f,
 0xe6, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 0x79, 0x7a, 0xad, 0xa8, 0xd4, 0x5b, 0xd6, 0xd7,
 0x9b, 0x9c, 0x9d, 0xfa, 0x9f, 0x15, 0x14, 0xac,
 0xab, 0xfc, 0xaa, 0xfe, 0xe4, 0x5d, 0xbf, 0xe7,
 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 0x48, 0x49, 0xe8, 0x93, 0x94, 0x95, 0xa2, 0xed,
 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
 0x51, 0x52, 0xee, 0x96, 0x81, 0x97, 0xa3, 0x98,
 0x5c, 0xf0, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 0x59, 0x5a, 0xfd, 0xf5, 0x99, 0xf7, 0xf6, 0xf9,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0xdb, 0xfb, 0x9a, 0xf4, 0xea, 0xff,

};

#define MAP	256
#define MAPS	(sizeof(tab)/MAP)

struct Map_s; typedef struct Map_s Map_t;

struct Map_s
{
	Map_t*		next;
	int		op;
	unsigned char	map[MAP];
};

static Map_t*	maps;

/*
 * generate and return ccode map from i to o
 */

unsigned char*
_ccmap(int i, int o)
{
	register unsigned char*	a;
	register unsigned char*	m;
	register unsigned char*	z;
	register Map_t*		map;
	register int		n;
	int			op;

	if (!i && !o)
		return (unsigned char*)tab;
	if (CCCONVERT(i))
	{
		o = CCOUT(i);
		i = CCIN(i);
	}
	else if (CCCONVERT(o))
	{
		i = CCIN(o);
		o = CCOUT(o);
	}
	else
	{
		if (i == 0)
			i = CC_NATIVE;
		if (o == 0)
			o = CC_NATIVE;
	}
	if (i == o || i < 1 || i > MAPS || o < 1 || o > MAPS)
		return 0;
	if (i == CC_ASCII)
		return (unsigned char*)tab + MAP * (2 * (o - 1));
	if (o == CC_ASCII)
		return (unsigned char*)tab + MAP * (2 * (i - 1) + 1);
	op = CCOP(i, o);
	for (map = maps; map; map = map->next)
		if (map->op == op)
			return map->map;
	if (!(map = newof(0, Map_t, 1, 0)))
		return 0;
	map->op = op;
	a = (unsigned char*)tab + MAP * (2 * (o - 1));
	z = (unsigned char*)tab + MAP * (2 * (i - 1) + 1);
	m = map->map;
	for (n = 0; n < MAP; n++)
		m[n] = n;
	for (n = MAP - 1; n >= 0; n--)
		m[n] = a[z[n]];
	map->next = maps;
	maps = map;
	return m;
}

void*
_ccmapcpy(register unsigned char* m, void* b, const void* a, size_t n)
{
	register unsigned char*	ub;
	register unsigned char*	ue;
	register unsigned char*	ua;

	if (m)
	{
		ub = (unsigned char*)b;
		ue = ub + n;
		ua = (unsigned char*)a;
		while (ub < ue)
			*ub++ = m[*ua++];
	}
	else
		memcpy(b, a, n);
	return b;
}

void*
_ccmapstr(register unsigned char* m, void* b, size_t n)
{
	register unsigned char*	s;
	register unsigned char*	e;

	if (m)
		for (e = (s = (unsigned char*)b) + n; s < e; s++)
			*s = m[*s];
	return b;
}

#else

static void
dump(int from, int to)
{
	register const unsigned char*	m;
	register int			c;
	register int			i;

	m = ccmap(from, to);
	sfprintf(sfstdout, "\n /* %s => %s */\n\n", ccmapname(from), ccmapname(to));
	for (c = i = 0; c <= UCHAR_MAX; c++)
	{
		sfprintf(sfstdout, " 0x%02x,", m ? m[c] : c);
		if (!(++i & 0x7))
		{
			i = 0;
			sfprintf(sfstdout, "\n");
		}
	}
}

main(int argc, char** argv)
{
	register int			i;

	for (i = 1; i <= CC_MAPS; i++)
	{
		dump(CC_ASCII, i);
		dump(i, CC_ASCII);
	}
	return 0;
}

#endif
