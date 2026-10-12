# CLAUDE.md

Lecture material (Markdown + small example projects) on CI/CD for embedded systems.
There is no top-level build: each example directory is a self-contained project.

## Layout
- `introduction/`, `continuous-integration/`, `continuous-delivery/` – lecture chapters, one `README.md` per topic.
- `*-exercise/` directories are student exercises (with `TODO.txt`); the sibling directory without the suffix is the solution. Keep both in sync.

## C++ (CMake + GoogleTest)
Projects use CMake ≥ 3.25, C++17/20, `-Wall -Wextra -Wpedantic`, and `find_package(GTest REQUIRED)`.
Typical layout: `include/`, `src/` (library), `test/` (`test.cpp`, mocks). No CTest – run the test binary directly.

```bash
cd <project-dir>
cmake -S . -B build              # some projects select an impl: -DUSE_IMPL=<LCD|OLED|target-1|...>
cmake --build build
./build/test/test                # GoogleTest executable
```

## Python (Flask + pytest)
Dependencies are pinned in the root `requirements.txt`; use the root virtualenv.

```bash
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
cd <project-dir> && pytest       # tests are unittest-style *_test.py files
python <service>.py              # run a Flask example service
```

## Documentation
* Use only 80 chars per line for documentation text.
* Don't use — in the text.
