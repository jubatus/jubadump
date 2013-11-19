jubadump
================

``jubadump`` converts saved jubatus internal model to json.``jubadump`` supports only classifier model and inverted_index model.

Requirements
==============

- g++
- pkg-config
- pficommon
- jubatus


Compile and Install
=====================

$ ./waf configure
$ ./waf
$ ./waf install

Usage
===================

1. Call save RPC and save your model.
2. Execute jubadump.

    usage: jubadump --input=string [options] ... 
    options:
      -i, --input    Input file (string)
      -t, --type     Format type (string [=classifier])
      -?, --help     print this message


