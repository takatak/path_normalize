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

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <linux/limits.h>
#include <sys/stat.h>

#include "path_normalize.h"


char *path_normalize(const char *path,const char *base,const bool fResolveLink,const bool fCheckExist,const bool fCheckMax,const char **emsg){
	int    len_path=0;
	int    len_base=0;
	int    len_ret=0;
	char  *ret=NULL;
	bool   fLastCharSlash=false;
	char  *pHomeDir=NULL;
	int    lenHomeDir=0;
	int    alloc_size=0;
	int    k;
	struct passwd *passwdEnt=NULL;
	int    iErrNo;

	if( path != NULL ){
		len_path=(int)strlen(path);
	}
	if( base != NULL ){   /* optional */
		if( base[0] == '~' ){
			if(( base[1] == '/' )||( base[1] == '\0' )){
				errno=0;
				passwdEnt = getpwuid(getuid());
				iErrNo=errno;
				if( passwdEnt == NULL ){
					if( emsg != NULL ) *emsg="error on getpwuid(getuid())";
					errno = iErrNo;
					return NULL;
				}
				lenHomeDir=strlen(passwdEnt->pw_dir);
				pHomeDir=passwdEnt->pw_dir;
			}else{
				if( emsg != NULL ) *emsg="base is not  '~' or '~/' or '~/xxx'";
				errno = EINVAL;
				return NULL;
			}
		}else if( base[0]!= '/' ){
			if( emsg != NULL ) *emsg="base is not  '~' or '~/' or '~/xxx' or '/' or '/xxx'";
			errno = EINVAL;
			return NULL;
		}
		len_base = (int)strlen(base);
	}

	if(( len_path > 0 )||( len_base > 0 )||( lenHomeDir > 0 )){
		/* allocate momory to store path */
		alloc_size = len_path + len_base + lenHomeDir + 1;
		if( ( ret=(char *)calloc((alloc_size),sizeof(char)) ) == NULL ){
			if( emsg != NULL ) *emsg="could not allocate memory";
			errno = ENOMEM;
			return NULL;
		}
	}
	/* determine converted path's last char is '/' or not */
	if( len_path > 0 ){
		if( path[len_path-1] =='/' ){
			fLastCharSlash = true;
		}
	}else{
		if( len_base > 0 ){
			if( base[len_base-1] == '/' ){
				fLastCharSlash = true;
			}
		}
	}
	/* at 1st , set part of base to path stored*/
	if( len_base > 0 ){
		int n=0,m;
		if( lenHomeDir > 0 ){
			for( m=0 ; pHomeDir[m] != '\0' ; n++ , m++ ){
				ret[n]=pHomeDir[m];
			}
			if( ret[n-1] != '/' ){
				ret[n]='/';
				n++;
			}
			if( base[1] == '/' ){
				m=2;
			}else{ /* base[1] == '\0' */
				m=1;
			}
		}else{
			m=0;
		}
		for( ; base[m] != '\0' ; n++,m++){
			ret[n]=base[m];
			if( base[m] == '/' ){
				for(;base[m+1]=='/';m++);
			}
		}
		len_ret = n;
		if( ret[len_ret-1] != '/' ){
			ret[len_ret] = '/' ;
			len_ret++;
		}
	}

	/* 2nd , set part of path to path stored*/
	if( len_path > 0 ){
		int n,m;
		for( n=len_ret , m=0 ; m < len_path ; n++ , m++ ){
			ret[n]=path[m];
			if( path[m] == '/' ){
				for( ; path[m+1] == '/' ; m++ );
			}
		}
		len_ret = n;
		if( ret[len_ret-1] != '/' ){
			ret[len_ret] = '/' ;
			len_ret++;
		}
	}
	if( len_ret == 0 ){ /* length of both path and base is zero! */
		/* get current directory*/
		char *pCurDir=NULL;
		errno = 0 ;
		if( ( pCurDir=getcwd(NULL,0)) != NULL ){
			iErrNo = errno;
			ret=pCurDir;
			len_ret=strlen(ret);
			/* determine converted path's last char is '/' or not */
			if( len_ret > 0 ){
				if( ret[len_ret-1] =='/' ){
					fLastCharSlash = true;
				}
			}else{ /* ??? */
				if( emsg != NULL ) *emsg="length of current director is zero";
				if( iErrNo == 0 ){
					errno = ENOTDIR; /* ??? */
				}else{
					errno = iErrNo;
				}
				return NULL;
			}
		}else{
			iErrNo = errno;
			if( emsg != NULL ) *emsg="could not get current director";
			errno=iErrNo;
			return NULL;
		}
	}
	/* ------------------- */
	/* end of preparation  */
	/* start normalization */
	/* ------------------- */
	k=0;
	while( k < len_ret ){
		if( ret[k] != '.' ){
			k++;
		}else{
			if( ( ret[k-1] == '/' )&&( ret[k+1] == '/' ) ){/* "/./"  */
				int m;
				for( m=k+2 ; m < len_ret ; m++ ){
					ret[m-2]=ret[m];
				}
				ret[m-2]='\0';
				len_ret-=2;
			}else if(( ret[k-1] == '/' )&&( ret[k+1] == '.')&&( ret[k+2] == '/') ){ /* "/../"  */
				int m,n,ll;
				m = k - 2;
				if( m < 0){
					m=1;
				}else{
					while( m > 0){
						if( ret[m] !='/' ){
							m--;
						}else{
							break;
						}
					}
				}
				n= k + 3;
				k=m;
				for( ; n < len_ret ; n++ , m++ ){
					ret[m+1]=ret[n];
				}
				ll= n-m-1;
				ret[n-ll]='\0';
				len_ret-=ll;
			}
		}
	}
	/* -------------------- */
	/* end of normalization */
	/* -------------------- */
	if( fResolveLink == false){
		if( fCheckExist == true ){
			struct stat sb;

			/* remove '/' of tail for stat() */
			if( ret[len_ret - 1 ] == '/' ) ret[len_ret - 1 ] = '\0' ;
			len_ret--;
			errno=0;
			if( stat(ret, &sb) == 0 ){
				if( !S_ISDIR(sb.st_mode) ){
					if( emsg != NULL ) *emsg="path is not exist as directory";
					free(ret);
					errno = ENOTDIR;
					return NULL;
				}
			}else{
				iErrNo = errno;
				if( iErrNo == ENOENT ){
					if( emsg != NULL ) *emsg="path is not exist";
				}else{
					if( emsg != NULL ) *emsg="error on stat()";
				}
				free(ret);
				errno = iErrNo;
				return NULL;
			}
		}else{
			if( fCheckMax == true ){
				if( len_ret >= PATH_MAX ){
					if( emsg != NULL ) *emsg="converted path is longer than PATH_MAX";
					free(ret);
					errno = ENAMETOOLONG;
					return NULL;
				}else{
					int m,pos_slash1=-1;
					for( m=0 ; m <= len_ret ; m++ ){
						if(( ret[m] == '/' )||( ret[m] == '\0' )){
							if( pos_slash1 < 0 ){
								pos_slash1 = m;
							}else{
								if( m - pos_slash1 - 1 > NAME_MAX ){
									if( emsg != NULL ) *emsg="converted path is include longer name component than NAME_MAX";
									free(ret);
									errno = ENAMETOOLONG;
									return NULL;
								}
								pos_slash1=-1;
							}
						}
					}
				}
			}
		}
		if(	fLastCharSlash == true ){
			/* set last '/' */
			if( ret[len_ret - 1 ] != '/' ){
				ret[len_ret] = '/';
				ret[len_ret+1] = '\0';
				len_ret++;
			}
		}else{
			/* remove last '/' */
			if( ret[len_ret - 1 ] == '/' ){
				ret[len_ret - 1 ] = '\0';
				len_ret--;
			}
		}
	}else{
		/* resolve link */
		char *ret_resolved=NULL;
		errno=0;
		ret_resolved=realpath((const char *)ret, NULL);
		iErrNo = errno;
		if( ret_resolved == NULL ){
			free(ret);
			if( emsg != NULL ) *emsg="error on realpath()";
			errno = iErrNo;
			return NULL;
		}else{
			int len_ret_resolved = strlen(ret_resolved);
			if(	fLastCharSlash == true ){
				if( ret_resolved[ len_ret_resolved - 1 ] != '/' ){
					/* could not get allocated size of ret_resolved . so could not append '/' to ret_resolved */
					/* and i dont know ret_resolved is reallocable or not */
					if( len_ret_resolved + 1 < alloc_size ){
						/* reuseable ret ! */
					}else{
						ret=realloc(ret,(len_ret_resolved+2)*sizeof(char));
						if( ret == NULL ){
							if( emsg != NULL ) *emsg="could not reallocate memory(1)";
							errno = ENOMEM;
							return NULL;
						}
					}
					strcpy(ret,ret_resolved);
					ret[len_ret_resolved]='/';
					len_ret_resolved++;
					ret[len_ret_resolved]='\0';
					free(ret_resolved);
					len_ret = len_ret_resolved;
				}else{
					free(ret);
					ret=ret_resolved;
					len_ret = len_ret_resolved;
				}
			}else{
				if( ret_resolved[ len_ret_resolved - 1 ] == '/' ){
					len_ret_resolved--;
					ret_resolved[ len_ret_resolved ] = '\0';
				}
				free(ret);
				ret=ret_resolved;
				len_ret = len_ret_resolved;
			}
		}
	}
	if( ret != NULL ){
		if( len_ret + 1 < alloc_size ){
			char *re=realloc( ret , len_ret + 1 );
			if( re != NULL ){
				ret=re;
			}else{
				/* do not care ! */
			}
		}
	}
	return ret;
}

