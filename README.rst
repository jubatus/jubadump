==========
 jubadump
==========

``jubadump`` converts saved jubatus internal model to json.``jubadump`` supports only classifier model and inverted_index model.

Requirements
============

- g++
- pkg-config
- jubatus_core
- msgpack


Compile and Install
===================

::

   $ ./waf configure
   $ ./waf
   $ ./waf install


Usage
=====

1. Call save RPC and save your model.
2. Execute jubadump.

    usage: jubadump --input=string [options] ... 
    options:
      -i, --input    Input file (string)
      -t, --type     Format type (string [=classifier])
      -?, --help     print this message


Format
======

- common

  - ``weights``

    - ``version_number`` : Version of model. This value will be updated by MIX.
    - ``document_frequencies`` : Frequency of each features in data inputted so far.
    - ``document_count`` : Number of all documents. This value will be used in calculation of global_weight(idf).
- classifier, regression

  - ``storage``

    - ``weight`` : Weights of each features and each labels in data inputted so far.
- recommender (inverted_index)

  - ``index``

    - ``storage``

      - ``inv``: IDs of each features
    - ``original``

      - ``inv`` : Features of each IDs
- anomaly (lof based on inverted_index)

  - ``storage``

    - ``lof_records``

      - ``entry``: kdist and lrd value of each IDs
    - ``nn_records`` : Refer to recommender for the list of values.


License
=======

LGPL 2.1
