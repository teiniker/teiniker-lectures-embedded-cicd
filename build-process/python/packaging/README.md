# Python Packaging 

Publishing a package requires a flow from the author’s source code to an end 
user’s Python environment. 

The steps to achieve this are:

* Have a **source tree containing the package**. 
    This is typically a checkout from a version control system (VCS).

* Prepare a configuration file describing the **package metadata**
    (name, version and so forth) and how to create the build artifacts. 
    For most packages, this will be a **pyproject.toml** file, maintained 
    manually in the source tree.

* **Create build artifacts** to be sent to the package distribution service. 
    These will normally be a **source distribution (sdist)** 
    and one or more **built distributions (wheels)**. 
    These are made by a build tool using the configuration file from the 
    previous step. Often there is just one generic wheel for a pure Python package.

* Upload the build artifacts to the **package distribution service** (usually PyPI).


To use the package, end users must:

* **Download** one of the package’s build artifacts from the package distribution 
    service.

* **Install it in their Python environment**, usually in its site-packages directory. 
    This step may involve a build/compile step which, if needed, must be described by 
    the package metadata.

These last 2 steps are typically performed by `pip` when an end user runs **pip install**.



## Create a Python Package 


### Source Tree 

The source tree contains the package source code.
The particular version of the code used to create the build artifacts will typically 
be a checkout from git based on a tag associated with the version.

_Example:_ Simple algorithm implementation.
```bash
├── README.md
├── pyproject.toml
└── src
    └── algorithm
        ├── fibonacci.py
        └── __init__.py
```    


### Configuration File

The configuration file depends on the tool used to create the build artifacts.

At a minimum, the `pyproject.toml` file needs a `[build-system]` table specifying your build tool. 

The particular build tool you choose dictates what additional information 
is required in the `pyproject.toml` file. For example, we might specify:

* A [project] table containing project Core Metadata (name, version, author and so forth),

* A [tool] table containing tool-specific configuration options.


_Example:_ Configuration file using wheel

```toml
[build-system]
requires = ["setuptools>=69", "wheel"]
build-backend = "setuptools.build_meta"

[project]
name = "simple_lib"
version = "0.1.0"
description = "Simple Algorthm Library"
readme = { file = "README.md", content-type = "text/markdown" }
authors = [{ name = "homer", email = "homer@springfield.com" }]
requires-python = ">=3.8"
dependencies = []

[tool.setuptools]
package-dir = {"" = "src"}

[tool.setuptools.packages.find]
where = ["src"]
```

### Build Artifacts

Before building your package, ensure the necessary tools are installed (use a virtual environment):

```bash
$ pip install --upgrade build twine
```

- `build` is used to create source and wheel distributions.
- `twine` is used to upload distributions to PyPI and verify them.

* **The source distribution (sdist)**:
A source distribution contains enough to install the package from source 
in an end user’s Python environment. As such, it needs the package source, 
and may also include tests and documentation. 
These are useful for end users wanting to develop your sources, and for 
end user systems where some local compilation step is required (such as 
a C extension).

The build package knows how to invoke your build tool to create one of these:
```bash
$ python3 -m build --sdist source-tree-directory
```

* **The built distributions (wheels)**:

A built distribution contains only the files needed for an end user’s Python 
environment. No compilation steps are required during the install, and the 
**wheel file** can simply be unpacked into the site-packages directory. 
This makes the install faster and more convenient for end users.

A pure Python package typically needs only one “generic” wheel. 

A package with compiled binary extensions needs a wheel for each supported 
combination of Python interpreter, operating system, and CPU architecture 
that it supports. 
If a suitable wheel file is not available, tools like `pip` will fall back 
to installing the source distribution.

The build package knows how to invoke your build tool to create one of these:

```bash
$ python -m build --wheel source-tree-directory
```


_Example:_ Build a source and wheel package and check the distributions 
```bash 
$ python -m build

├── dist
│   ├── simple_lib-0.1.0-py3-none-any.whl
│   └── simple_lib-0.1.0.tar.gz
```

### Upload to the Package Distribution Service

For our initial experiments, it is sufficient to store the package files locally.

## Install and Use a Python Package 

Now that the package is published, end users can download and install the package into 
their Python environment. Typically this is done with **pip**. 

_Example:_ Install from a local wheel file

1. Create a virtual environment:

```bash 
$ cd app
$ python3 -m venv .venv
$ source .venv/bin/activate
```

2. Install the wheel file:

```bash 
$ pip install ../dist/simple_lib-0.1.0-py3-none-any.whl
$ pip list
Package    Version
---------- -------
pip        25.1.1
simple_lib 0.1.0
```

3. Run the application:

```bash
$ cd app 
$ python demo.py
```

4. Deactivate virtual environment:
```bash
$ deactivate
```


## References
* [The Packaging Flow](https://packaging.python.org/en/latest/flow/)

* [RealPython: What Are Python Wheels and Why Should You Care?](https://realpython.com/python-wheels/)

* [wheel documentation](https://wheel.readthedocs.io/en/stable/)


*Egon Teiniker, 2025, GPL v3.0*
