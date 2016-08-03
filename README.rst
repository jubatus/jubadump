==========
 jubadump
==========

``jubadump`` converts saved jubatus internal model to json.
``jubadump`` supports only classifier (except for NN), regression, recommender (inverted_index), anomaly (lof + inverted_index) and nearest_neighbor model.

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

::

    usage: jubadump --input=string [options] ...
    options:
      -i, --input    Input file (string)
      -?, --help     print this message


Format
======

- common

  - ``weights`` (some values are available only when IDF/BM25 weighting is used)

    - ``version_number`` : Version of model. This value will be updated by MIX.
    - ``document_frequencies`` : Frequency of each feature in data inputted so far.
    - ``document_count`` : Number of all documents. This value will be used in calculation of global_weight(idf).
    - ``group_frequencies`` : Frequency of each Datum (string_values) key in data inputted so far.
    - ``group_total_length`` : Number of total features extracted from each Datum (string_values) key in data inputted so far.

- classifier

  - ``storage``

    - ``labels``

      - ``version_number`` : Version of model. This value will be updated by MIX.
      - ``label_count`` : Number of traind data of each label.
    - ``storage``

      - ``weight`` : Weights of each feature and each label in data inputted so far.

- regression

  - ``storage``

    - ``weight`` : Weights of each feature and each label in data inputted so far.

- recommender (inverted_index)

  - ``index``

    - ``storage``

      - ``inv``: No data
    - ``original``

      - ``inv`` : Features of each ID

- anomaly (lof based on inverted_index)

  - ``storage``

    - ``lof_records``

      - ``entry``: kdist and lrd value of each ID
    - ``nn_records`` : Refer to recommender for the list of values.

- nearest_neighbor

  - ``table``

    - ``clock``: generation of the table (number of updates)

    - ``schema``: schema of the table

    - ``data``: records of each ID

      - ``version``: generation of the record

      - ``values``: data of each column for the ID; multiple 64-bit integers are used to represent the bit vector for column longer than 64 bit.

      - ``owner``: identifier of the server who has a permission to modify the record

License
=======

LGPL 2.1
