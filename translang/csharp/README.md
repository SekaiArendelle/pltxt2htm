# translang/csharp

这个目录是 `include/pltxt2htm` -> C# 的自动翻译实验管线（基于 Python + uv）。

## 设计目标

- 使用 **uv** 管理 Python 依赖与执行环境。
- 使用 **LLVM/Clang**（`clang++` + `clang.cindex`）先做 C++ 预处理，再抽取函数声明并生成 C# 文件。
- 翻译策略对齐你的要求：
  - `fast_io::vector<T>` / `std::vector<T>` -> `List<T>`
  - `terminate` / `unreachable` 风格行为 -> C# 抛异常

> 当前版本先生成 API 框架和函数桩（stub），用于建立可持续迭代的翻译基础；后续可在 AST 层逐步补齐函数体翻译。

## 目录结构

- `pyproject.toml`: Python 依赖定义（由 uv 管理）。
- `src/translate.py`: 翻译脚本（预处理 + libclang 抽取 + 代码生成）。
- `generated/`: 输出目录，默认生成 `Pltxt2htm.Generated.cs`。

## 快速开始

```bash
cd translang/csharp
uv sync
uv run python src/translate.py \
  --header ../../include/pltxt2htm/pltxt2htm.hh \
  --output generated/Pltxt2htm.Generated.cs
```

## 手动执行脚本

```bash
uv run python src/translate.py \
  --header ../../include/pltxt2htm/pltxt2htm.hh \
  --output generated/Pltxt2htm.Generated.cs
```

## 后续建议

1. 把 `include/pltxt2htm/*.hh` 全量入口纳入一个 translation unit（例如生成专用聚合头）。
2. 在 AST 到 C# 的表达式/语句级别翻译器里，逐步替换：
   - `fast_io::string/basic_string` -> `string`
   - 错误路径 `terminate/unreachable` -> `throw`。
3. 增加回归测试：
   - 对比 C++ 与 C# 版本在相同输入上的 HTML 输出一致性。
