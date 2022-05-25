# 命名規則

* クラス/構造体 - 'PascalCase'
* 関数 - 'camelCase()'

Range-based-forではbegin/endという関数名の定義を必要とされます。そのため、関数名はキャメルケースで記述してください。（STLはスネーク記法に則っていますが、サードパーティとのソースコードの一貫性や、C++の慣例的なことからキャメルケースを採用します）

* メンバー変数 - '_camelCase'

メンバー変数と引数との衝突を回避するため、メンバー変数の前にアンダースコアを利用してください。

* マクロ - 'CONSTANT_CASE'

# ヘッダーファイル
ヘッダーにはインクルードガードを利用してください。
主要なコンパイラは「#pragma once」をサポートしています。  
そのため、「#ifdef」によるインクルードは行わないでください。  


```cpp
// Good
#pragma once 

// Bad
#ifndef HEADER_H
#define HEADER_H

// ...

#endif
```
C++のヘッダーであることを明示するため、.hppの拡張子を利用してください。

# autoの利用

C++コード内では auto を可能な限り使用しないでください。  
IDE上では型を推測できますが、コンパイル出来ない状態や、Github上や差分ツールでは、サポートさていません。  
そのため、いつでも読み手が読みやすいようにするため、型推論は可能な限り避けてください。  

例外:
lamdaによって、表現できないものやiteratorによって過度に可読性を下げるものに関してはautoを利用することが認められます。

# 列挙型

スコープがないため、enumではなくenum classの利用を推奨します。

```cpp
// Good
enum class Color
{
	Red,
	Green,
	Blue,
};

// Bad
enum Color
{
	Color_Red,
	Color_Green,
	Color_Blue,
};

```

enumを使うことで可読性が上がる場合、namespaceで囲い、スコープを制限してください。

```cpp
// Good
namespace Color
{
	enum Type
	{
		Red,
		Green,
		Blue,
	};
}
```

# マクロ

マクロの利用は可能な限り控えてください。意図しない展開や、IDE上以外での可読性を下げる可能性があります。　　constexprによる定数化や、templateによって解決できないもののみに利用してください。

```cpp
// Good
inline constexpr float pi{ 3.14f };

// Bad
#define PI (3.14)


// Good
template<typename T>
void safeDelete(T*& ptr){ /**/ }

// Bad
#define SAFE_DELETE(ptr)
```

# コメント

クラスやヘッダー上の関数、メンバー変数に対してのコメントはJavadoc形式で記述してください。これによってDoxygenによるドキュメント生成が行えます。

ファイルに作者の署名は行わないでください。途中でメンテされず、実際の著者と異なることが起きます。

# if内のブロック

ifの後の中括弧は省略せずに記述してください。

# 無名名前空間

実装側に無名名前空間で関数や変数を定義すると、UnityBuildによって、偶然同名の変数があった際、コンパイルエラーとなってしまいます。

同名変数のコリジョンを緩和させるために、無名名前内の外はinternal名前空間で閉じてください。


# ポインターの利用

生ポインターの利用は、避けてください。代わりに、std::unique_ptrやstd::shared_ptrといったスマートポインタの利用を行ってください。
