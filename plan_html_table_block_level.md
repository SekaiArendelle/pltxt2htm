# Plan: Move HTML table tag parsing to block level

Goal: mirror the `<ol>`/`<ul>`/`<li>` block-level migration and move the HTML
table series (`table/thead/tbody/tfoot/tr/th/td/caption/colgroup/col`) parsing
into the block-level scanners (main `details/parser/parser.hh` + experimental
`experimental/html_parser.hh` + `inline_parser.hh` literal text). Invalid /
unclosed tables fall back to literal (escaped) text.

## 1. New header `include/pltxt2htm/details/parser/html_table.hh`

Intermediate table AST + strict block scanner, mirroring `html_list.hh` /
`md_table.hh`:

- `struct HtmlTableCellRaw { u8string text; TableAlign align; bool is_header; }`
- `enum class HtmlTableRowSection : unsigned { none, thead, tbody, tfoot };`
- `struct HtmlTableRowRaw { vector<HtmlTableCellRaw> cells; HtmlTableRowSection section; }`
- `template<ndebug> struct HtmlTableAstRaw` — `rows`, `caption`, `has_caption`,
  `col_count`, `has_colgroup` + accessors (`rows_count`, `row_cells_count`, `cell_at`).
- `template<ndebug> struct ToHtmlTableAstResult { HtmlTableAstRaw<ndebug> raw_ast; size_t advance_count; }`
- `optionally_to_html_table_ast<ndebug>(u8string_view pltext)` — strict
  content-model state machine (table / section / row / caption / colgroup),
  reusing `try_parse_*_tag` (parent validation, case-insensitive names, align
  extraction) and `try_parse_bare_tag` for `<table` and `</…>` closers.
  Cell/caption content is captured raw, whitespace-trimmed (like `<li>`).
  Any violation / EOF mid-table → `::exception::nullopt` (whole table literal).

## 2. `include/pltxt2htm/details/parser/frame_context.hh`

- Include `html_table.hh`; add `enum class HtmlTableParsePhase { caption, body, finish }`.
- Add `ParserFrameContextWithHtmlTableInfo<ndebug>` (raw_ast + phase + row/cell
  index), mirroring `ParserFrameContextWithMdTableInfo` (frame_context.hh:219).
- `FrontendContextVariant`: union member + `ContextBranch::html_table`; update
  copy/move/destroy `ContextBranch` switches so `-Werror=switch` passes; add
  `get_html_table_*` / `set_html_table_*`.
- Add `push_table_frame(call_stack, HtmlTableAstRaw&&)`, mirroring
  `push_list_frame` (frame_context.hh:1740).

## 3. Main parser `include/pltxt2htm/details/parser/parser.hh`

- `find_next_block_after_line_break`: after the `<ul>/<ol>` list check, detect
  `<table` via `optionally_to_html_table_ast`; on success push table frame and
  return `advance_count` = consumed bytes.
- Frame iteration: add an `html_table` block (after the `md_table` block at
  parser.hh:479–583) with phases:
  - `caption`: push `html_caption` frame with a bounded caption view (if present).
  - `body`: push `html_th`/`html_td` cell frames (`ParserFrameContextWithCellInfo`)
    walking rows/cells, mirroring the md_table body loop.
  - `finish`: pop; assemble `table_ast` = `[HtmlCaption?, HtmlColgroup{col_count×HtmlCol}?,
    sections/rows...]` — direct rows (`section == none`) appended bare, runs of
    `thead`/`tbody`/`tfoot` rows wrapped in their section nodes; then
    `if (call_stack.empty()) return {subast = move(table_ast)};` else push
    `HtmlTable{move(table_ast)}` into the parent.
- Remove inline table detection: `<c` branch caption/colgroup (~1049–1073),
  `<t` branch table/thead/tbody/tfoot/tr/th/td (~1475–1559), `<col`
  self-closing (~2768), and the `</…>` end-tag cases (2477–2640+). Keep the
  `LessThan{}` literal fallbacks and the existing pop-assembly cases for
  `html_th`/`html_td`/`html_caption` (cell/caption frames still pop via them).

## 4. Public entry `include/pltxt2htm/parser.hh`

- Add `case NodeKind::html_table:` (next to `md_table`, parser.hh:138) that
  wraps the returned subast into `HtmlTable` for block-position tables.

## 5. Experimental `include/pltxt2htm/experimental/html_parser.hh`

- `find_next_block_after_line_break` (40–252): add table detection after the
  list block (174–179).
- Frame iteration: add the `html_table` phase machine mirroring the list frame
  iteration (194–252); caption/cell frames pop via the existing pop-assembly
  (1442–1473).
- Remove inline table branches (caption/colgroup ~414–436, table/thead/tbody/
  tfoot/tr/th/td ~620–700, end-tags 1130–1284). Update header doc comment (line 36).
- Public `parse_pltxt_html` (1491) needs no structural change.

## 6. Inline parser `include/pltxt2htm/inline_parser.hh`

- Table tags become literal text: delete caption/colgroup/col (523–557),
  table/thead/tbody/tfoot/tr/th/td (950–~1010), staged-pop (1912) and final
  push (2575) table handling, leaving the `LessThan{}` literal fallback. Output
  then emits `&lt;table&gt;…` like `<ul>` already does.

## 7. Test re-baseline

- `tests/test_html_table.cc`: keep well-formed cases; change unclosed `<table>`
  (line 64) and inline `t<table>…</table>t` (line 70) to escaped literals, plus
  the truncated tail.
- `tests/test_html_parser.cc`: keep 101/322–328; change unclosed 357/381/387/393/399
  and mismatched-end-tag 444–453 to escaped literals.
- `tests/test_common_parser.cc:249–255`: `"chdf"` → escaped literal table string.
- Grep `tests/` for any remaining `<table` expectations and reconcile.

## 8. Verification

- `python scripts/gen_format_ninja.py` + `python scripts/gen_tidy_ninja.py`
  (new header), then `ninja -f format_cpp.ninja`, `ninja -f tidy_cpp.ninja`.
- `cmake -S tests -B tests/build -G Ninja -DCMAKE_BUILD_TYPE=Debug`,
  `cmake --build tests/build`, `ctest --test-dir tests/build`
  (covers `test_deep_nesting_stress.cc`, `PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT`).
- Full suite `python ./tests/run_all_tests.py`.
- Fuzzer regression (`fuzzing/src/fixedadv_roundtrip.cc`, uses
  `experimental::parse_pltxt_html`).