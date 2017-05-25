/* path_normalize ver 1.0

MIT License

Copyright (c) 2017 Takataka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef INCL_PATH_NORMALIZE_H
	#define INCL_PATH_NORMALIZE_H
	/* ---------------------------------------------------------------------------------------------------------
	// normalize path
	// This function convert
	//   from :
	//     "/home/hoge/..//foo/bin/../dev/./bin/../"
	//   to:
	//     "/home/foo/dev/"
	//
	// args :
	//       path         : optional(allow NULL)
	//                      path to be converted
	//                      not allowed '~' as home-dir
	//       base         : optional (allow NULL)
	//                      if path is not descrived as fullpath , then set this as the base directory
	//                      this should be  '/xxxx' or '~/xxx'.
	//       fResolveLink : mandatory (true / false )
	//                      set true if convert directory-link to real-directory,
	//       fCheckExist  : mandatory (true / false )
	//                      set true if check path is exist or not
	//       fCheckMax    : mandatory (true / false )
	//                      set true if check PATH_MAX / NAME_MAX ( if fCheckExist == true then not checked)
	//       emsg         : optional(allow NULL)
	//
	// return :
	//       NULL         : some error. please check emsg / errno
	//       other        : the address of normalized path
	//
	// N.B. 1.If fResolvedLink == true , then
	//          1) normalized path should be exist.
	//          2) fCheckExist / fCheckMax is ignored( both checked always by realpath() )
	//      2.If fResolcedLink == false, then
	//          1) normalized path is not checked existing or not if fCheckExist == false
	//          2) the length of normalized path is not checked whether less than PATH_MAX / NAME_MAX if fCheckMax == false
	//      3.If returned value is not null , then  caller should be free the address as soon as possible.
	//      4.Only utf-8 environment support.
	//      5.If path is "" and base is NULL or "" ,then current working directory is assigned.
	// if NLLL returned , then check errno and/or emsg
	//  errno : EINVAL       : invalid parm
	//	        ENOMEM       : memory shortage?( could not allocate memory )
	//	        ENOTDIR      : the path converted is not directory
	//	        ENOENT       : the path converted is not exist
	//	        ENAMETOOLONG : the path converted is too long ( total or componet )
	//	        other        : set by reapath() or stat() or getpwuid(getuid()) or getcwd()
	// ---------------------------------------------------------------------------------------------------------
	*/
	char *path_normalize(const char *path,const char *base,const bool fResolveLink,const bool fCheckExist,const bool fCheckMax,const char **emsg);
#endif
