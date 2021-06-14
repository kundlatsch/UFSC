# INE5426

# Usage requirements

You need to have `pip3` installed, compatible with `python=^3.6.9`.

# Setup

Run `make setup` to createthe environment. A virtual environment will be created using `virtualenv` in `./venv` folder. To activate it manually, you can run `source venv/bin/activate`, but using the `make run` command, it is used by default.

# Executing

Run `make run filepath=<source/code/file/path>`. If you run `make run` without setting up a `filepath`, the `examples/exemplo1.ccc` will be used.


# With poetry

Also, you can install this project with **poetry**. To install it, you can follow [this tutorial](https://python-poetry.org/docs/#installation). (Using pip is possible: `pip install poetry`).

Once you have poetry installed, you need to install de dependencies with 

```
poetry install
```

Then, you can run the project with `make poetry-run filepath=<source/code/file/path>`.