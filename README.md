# Differentiater

Differentiater is a small symbolic differentiation playground. It parses an expression, builds an expression tree, produces the derivative, performs a few algebraic cleanups, and generates visual artefacts (LaTeX, PDF, Graphviz).

## Features
- recursive-descent parser for `+`, `-`, `*`, `/`, `^`, plus `log`, `sin`, `cos`, `tan`;
- derivative construction followed by basic simplifications (drop zero factors, fold constants, etc.);
- automatic LaTeX and PDF report with intermediate steps;
- Graphviz dump of the expression and derivative trees.

## Requirements
- `g++` and `make` (e.g. `build-essential` on Debian/Ubuntu);
- `graphviz` (used for the `dot` command);
- `pdflatex` with the LaTeX packages referenced in `src/differ.cpp`; the easiest option is `texlive-full`, or at least `texlive-latex-extra` plus `texlive-lang-cyrillic`;
- any PDF/PNG viewer you prefer.

Before running, make sure the output folders exist:

```bash
mkdir -p dump/{tex,pdf,dotfiles,tree_pictures}
```

## Build

```bash
make
```

Object files are stored in `obj/`; the binary is `Differ`. To remove build artefacts:

```bash
make clean
```

## Run

```bash
./Differ
```

The input expression lives in `src/main.cpp` inside `char s[]`. Example format:

```
(s(x))^(c(x))+(c(x))^(s(x))$
```

- parenthesis are mandatory for grouping;
- `s(...)`, `c(...)`, `t(...)` map to `sin`, `cos`, `tan`;
- `l(base, value)` stands for `log_base(value)`;
- the string must end with `$`.

Edit the string and rebuild to differentiate another function.

## Outputs
- LaTeX sources: `dump/tex/<index>.tex`;
- PDF reports: `dump/pdf/<index>.pdf`;
- Graphviz trees: `dump/dotfiles/<index>.dot` with PNG snapshots in `dump/tree_pictures/<index>.png`.

Indexes increase on each run.

## Repository layout
- `src/`
  - `main.cpp` — entry point and expression selection;
  - `differ.cpp` — tree utilities, differentiation logic, LaTeX/Graphviz generation;
  - `RecursDesc.cpp` — recursive-descent parser and tree builder.
- `headers/` — corresponding headers.
- `dump/` — generated artefacts.
- `makefile` — build rules.

## Notes
- The default build enables a large set of warnings and sanitizers (`-fsanitize=...`). Adjust `CFLAGS` if you need a release build.
- At the moment the program differentiates only the hard-coded expression; consider adding file or STDIN input if you need interactive usage.
