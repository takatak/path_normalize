/*
 * This file is sample WITHOUT WARRANTY OF ANY KIND.
 * I(Takataka) abandon the copyright of this file.
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "path_normalize.h"

typedef struct{
	char *path;
	char *base;
	bool  fResolveLink;
	bool  fCheckExist;
	bool  fCheckMax;
	char *comment;
} XMY_PARM;

int main(int argc,char **argv){

	char 	*emsg=NULL;
	int  	 ierrno=0;
	char 	*ret;
	XMY_PARM myparm[20];
	int  	 k;

	myparm[0].path="../";
	myparm[0].base="/tmp/hoge/hage/";
	myparm[0].fResolveLink=true;
	myparm[0].fCheckExist =false;
	myparm[0].fCheckMax   =false;
	myparm[0].comment="resolve link";

	myparm[1].path="../";
	myparm[1].base="/tmp/hage/hage/";
	myparm[1].fResolveLink=false;
	myparm[1].fCheckExist =false;
	myparm[1].fCheckMax   =false;
	myparm[1].comment="not resolve link ,no check";

	myparm[2].path="../";
	myparm[2].base="/tmp/hage/hage/";
	myparm[2].fResolveLink=false;
	myparm[2].fCheckExist =true;
	myparm[2].fCheckMax   =false;
	myparm[2].comment="not resolve link,check exist";

	myparm[3].path="../xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx/";
	myparm[3].base="/tmp/hoge/hage/";
	myparm[3].fResolveLink=false;
	myparm[3].fCheckExist =false;
	myparm[3].fCheckMax   =true;
	myparm[3].comment="not resolve link,long path , check max";

	myparm[4].path="/tmp/";
	myparm[4].base=NULL;
	myparm[4].fResolveLink=false;
	myparm[4].fCheckExist =true;
	myparm[4].fCheckMax   =false;
	myparm[4].comment="not resolve link,check exist";

	myparm[5].path="bin/";
	myparm[5].base="~/";
	myparm[5].fResolveLink=false;
	myparm[5].fCheckExist =false;
	myparm[5].fCheckMax   =false;
	myparm[5].comment="not resolve link,no check,'~/'";

	myparm[6].path="/home/hoge/..//foo/bin/../dev/./bin/../";
	myparm[6].base=NULL;
	myparm[6].fResolveLink=false;
	myparm[6].fCheckExist =false;
	myparm[6].fCheckMax   =false;
	myparm[6].comment="not resolve link,no check";

	myparm[7].path="/home/hoge/test/slink-dir/";
	myparm[7].base=NULL;
	myparm[7].fResolveLink=false;
	myparm[7].fCheckExist =false;
	myparm[7].fCheckMax   =false;
	myparm[7].comment="not resolve link,no check";

	myparm[8].path="/home/hoge/test/slink-dir/";
	myparm[8].base=NULL;
	myparm[8].fResolveLink=true;
	myparm[8].fCheckExist =false;
	myparm[8].fCheckMax   =false;
	myparm[8].comment="resolve link,no check";

	myparm[9].path="";
	myparm[9].base=NULL;
	myparm[9].fResolveLink=false;
	myparm[9].fCheckExist =false;
	myparm[9].fCheckMax   =false;
	myparm[9].comment="path is '' and base is null,not resolve link,no check";

	myparm[10].path="";
	myparm[10].base=NULL;
	myparm[10].fResolveLink=true;
	myparm[10].fCheckExist =false;
	myparm[10].fCheckMax   =false;
	myparm[10].comment="path is '' and base is null,resolve link,no check";

	myparm[11].path="";
	myparm[11].base=NULL;
	myparm[11].fResolveLink=false;
	myparm[11].fCheckExist =true;
	myparm[11].fCheckMax   =false;
	myparm[11].comment="path is '' and base is null,not resolve link,check exist";

	myparm[12].path=NULL;
	myparm[12].base=NULL;
	myparm[12].fResolveLink=false;
	myparm[12].fCheckExist =false;
	myparm[12].fCheckMax   =true;
	myparm[12].comment="path is NULL and base is null,not resolve link,check max";

	myparm[13].path="!!!!EXIT!!!";

	for( k = 0 ; ( k < sizeof(myparm)/sizeof(XMY_PARM) )&&( strcmp(myparm[k].path,"!!!!EXIT!!!") != 0 ) ; k++ ){
			ierrno=0;
			emsg=NULL;
			printf("[%d]----------------------------------------------------------------------\n",k);
			printf("path   :[%s]\n",myparm[k].path);
			printf("base   :[%s]\n",myparm[k].base);
			printf("comment:[%s]\n",myparm[k].comment);

			ret = path_normalize(myparm[k].path,myparm[k].base,myparm[k].fResolveLink,myparm[k].fCheckExist,myparm[k].fCheckMax,(const char **)&emsg);

			printf("  ret  :[%s]\n",ret);
			printf("  errno:[%d] , emsg[%s]\n",ierrno,emsg);
	}
	return 0;
}
