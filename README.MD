# CppArrai18n
is translating and formatting tool.
## but WIP.
so busy.

## how to
### write LangFile  
ja-jp.txt
```toml
[ja-JP]
main.open = "{0}を開く"
main.saved = "{0}を{1}として保存しました。"
# ...
```
----
en-us.txt
```toml
[en-US]
main.open = "Open {0}"
main.saved = "Saved {0} as {1}."
# ...
```
---
### include
```cpp
#include <Arrai18n>
```
### load
```c++
Arrai18n::load("ja-jp.txt");
Arrai18n::load("en-us.txt");
```
### use
```c++
std::cout << Arrai18n::trl("main.saved","ja-JP",{"virus.exe", "iexplore.exe"})
std::cout << Arrai18n::trl("main.saved","en-US",{"virus.exe", "iexplore.exe"})
```
### result
```
virus.exeをiexplore.exeとして保存しました。
Saved virus.exe as iexplore.exe.
```