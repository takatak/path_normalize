パスの正規化 ver 1.0

この関数は次のようにパスを正規化します。

from : "/home/hoge/..//foo/bin/../dev/./bin/../"
to   : "/home/foo/dev/"

 補足: "../” や "./" は取り除かれています。
       末尾以外の無駄な "/" は取り除かれています。
       この2つをもって、"正規化"と称しています。

インターフェース :
		char *path_normalize(const char  *path,
		                     const char  *base,
		                     const bool   fResolveLink,
		                     const bool   fCheckExist,
		                     const bool   fCheckMax,
		                     const char **emsg);

引数 :
		path         : 指定しない場合は、NULL を指定してください。
	                   正規化されるパスです。
	                   ホームディレクトリを示す '~' を用いる事はできません(指定しても、そう解釈されません)。
		base         : 指定しない場合は、NULL を指定してください。
		               上記 path がフルパス記述ではない時に、ベースとなるディレクトリを指定します。
		               絶対パス指定でルートから記述するか、'~/xxx'の方式で、ホームディレクトリから記述します。
		fResolveLink : true / false
		               true を指定すると、正規化された PATH を realpath() を用いて、
		               ディレクトリ・リンクがあれば実際のディレクトリに変換します。
		fCheckExist  : true / false
		               true を指定すると、正規化された path が存在するか否か、チェックします。
		fCheckMax    : true / false
		               true を指定すると、正規化された path が最大長の制約の範囲内か否か、チェックします。
		               （PATH_MAX / NAME_MAX ）
		emsg         : 指定しない場合は NULL を指定してください。
		               戻り値が NULL の場合にエラー内容をセットします。

戻り値 :
		NULL         : 何らかのエラーが発生した場合です。
		               emsg または errno を参照してください。
		その他       : 正規化された path を格納したアドレスです。
		               使い終わったら、忘れずに free() してください。
注意事項:
		1. fResolveLink に true を指定した場合
		  1) 正規化された後の path は存在している必要があります。
		  2) fCheckExist や fCheckMax に何を指定しても無視されます。呼び出している realpath()がチェックしている模様です。
		2. fResolveLink に false を指定した場合、正規化された後の path は、
		  1) fCheckExist が true でない限り、実在するかはチェックされません。
		  2) fCheckMax が true でない限り、最大長のチェックはされません。
		  3) fCheckMax が true でも、fCheckExist が true なら、最大長のチェックはされません。
		3. utf-8 の事しか考えていません。
		4. path が NULL または長さが ゼロ で、base が NULL または長さがゼロの場合、
		   path には現行作業ディレクトリがフルパスで指定されたのと同様に扱います。

想定される errno:
		EINVAL       : 引数に誤りがある
		ENOMEM       : 空きメモリ不足( メモリ割り当てができなかった )
		ENOTDIR      : 正規化された path はディレクトリではない
		ENOENT       : 正規化された path は存在しない
		ENAMETOOLONG : 正規化された path は最大長の制約の範囲外である( path 全体 または 各コンポーネントのいずれか )
		その他       :  呼び出している関数[ reapath() , stat() , getpwuid(getuid()) , getcwd() ]でセットされるもの

備考 :

1. ギリギリまでメモリ使用量を抑制するような配慮はしていません。
2. 難しい処理は行っていません。
3. 以前から面倒くさい処理になりそうだと尻込みしていましたが、リンクを展開しない realpath() が急遽欲しくなり作成しました。
   realpath の挙動とのバランスで、fCheckExist / fCheckMax の機能を付けました。
   個人的にはチェック無しでも良かったので、オプションにしています。
   但し、realpath のソースを確認していないので、考慮不足/勘違いはあるかも知れませんが、ご容赦ください。
4. 調べた限りでは、最大長の制約は絶対的なものでは無いようなので、固執する必要はない、と認識しています(誤っているかも知れません)。
   それもあって、システム構成やフィアル・システム毎に異なっていたりするかも知れませんが、それをいちいちハンドリングする気になれず、
   一律の最大長でチェックを行っています。おまけ、程度の機能と思ってください。
5. ライブラリにする程のものでもないので、行っていません。
6. まだ linux でしか試していません。
