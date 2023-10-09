# CppArrai18n
配列で要素を渡せるから関数同士で翻訳テキストをやり取りしやすいかもしれない、翻訳＆フォーマットするやつ

## how to
### example code
`example.cpp`をみて。
#### result
```
"index.html"を開く
Open "index.html"

cat.pngをvery_cute_cat.pngとして保存しました。
Saved cat.png as very_cute_cat.png.
```
### write language file
#### 言語コードを書く
まずは`[]`で囲って言語コードを書く
```
[ja-JP]
```
#### テキストを書く
 - `name = "text"` みたいな感じで行ごとに名前とテキストのペアを書いていく
 - 名前には半角スペース(" ")と半角のイコール("=")以外なら何でも使える
 - テキストには`{number}` を使って引数を入れられる
 - テキストの方はエスケープ文字も使える

| 変換元 | 変換後 |
| - | - |
| \\{ | { |
| \\} | } |
| \\" | " |
| \\\\ | \\ |
| \\n | \\n |
| \\r | \\r |

## license
 - WTFPL version 2.0
 - NYSL Version  0.9982
 - その他の一般にオープンソースと呼ばれている任意のライセンス

の内から好きなライセンスを適用してください。