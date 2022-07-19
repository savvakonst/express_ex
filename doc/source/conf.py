# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

import textwrap

# import exhale.utils
# from exhale import utils

# -- Project information -----------------------------------------------------

project = 'express-ex'
copyright = '2022, SVK'
author = 'SVK'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
# extensions = ["breathe", 'exhale', "sphinx.ext.graphviz"]
extensions = ["breathe", "sphinx.ext.graphviz", 'sphinxcontrib.plantuml']
# plantuml = 'java -jar plantuml.jar'
plantuml = 'java -jar "C:\\Program Files\\Graphviz\\bin\\plantuml.jar"'
plantuml_output_format = "svg_obj"
# Breathe Configuration
breathe_projects = {"express-ex": "../xml/"}
breathe_default_project = "express-ex"

# breathe_projects_source = {
#    "auto": ("../examples/specific", ["auto_function.h", "auto_class.h"])
# }


graphviz_output_format = "svg"

# graphviz_dot = "C:/Program Files/Graphviz/bin/dot.bat"
# graphviz_dot = "C:/Users/SVK/Desktop/aaa/dot.bat"


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = 'en'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'CMakeLists.txt']

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
# html_theme = 'alabaster'
# html_theme = 'classic'

html_theme = 'sphinxdoc'
html_theme = 'llvm-theme'
html_theme_path = ["_themes"]
# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
