#!/usr/bin/env python
# -*- charset: utf-8 -*-

from jubatest import *
from jubatest.process import LocalSubprocess

import json

class JubadumpTestBase(object):
  @classmethod
  def setUpCluster(cls, env):
    cls.env = env

  def check_jubadump(self, target, cli):
    cli.save("jubadump_test")
    model_data = target.get_saved_model("jubadump_test")
    proc = LocalSubprocess(['jubadump', '--input', '/dev/stdin'])
    proc.start()

    # assert that command exits with status = 0
    self.assertEqual(proc.wait(model_data), 0)

    # assert that command prints valid JSON
    self.assertEqual(type(json.loads(proc.stdout)), dict)

  def test(self):
    (engine, config) = self.get_config()
    srv = self.env.server_standalone(self.env.get_node(0), engine, config)
    with srv as cli:
      self.do_training(srv, cli)
      self.check_jubadump(srv, cli)

class JubadumpClassifierTest(JubaTestCase, JubadumpTestBase):
  def get_config(self):
    return (CLASSIFIER, get_configs(CLASSIFIER)['arow'])

  def do_training(self, target, cli):
    d1 = target.types.LabeledDatum("positive", target.types.Datum({"string": "abc", "number": 123}))
    d2 = target.types.LabeledDatum("negative", target.types.Datum({"string": "abc", "number": 456}))
    cli.train([d1, d2])

class JubadumpRegressionTest(JubaTestCase, JubadumpTestBase):
  def get_config(self):
    return (REGRESSION, get_configs(REGRESSION)['pa'])

  def do_training(self, target, cli):
    d1 = target.types.ScoredDatum(10.0, target.types.Datum({"string": "abc", "number": 123}))
    d2 = target.types.ScoredDatum(-5.0, target.types.Datum({"string": "abc", "number": 456}))
    cli.train([d1, d2])

class JubadumpRecommenderTest(JubaTestCase, JubadumpTestBase):
  def get_config(self):
    return (RECOMMENDER, get_configs(RECOMMENDER)['inverted_index'])

  def do_training(self, target, cli):
    d1 = target.types.Datum({"string": "abc", "number": 123})
    d2 = target.types.Datum({"string": "abc", "number": 456})
    cli.update_row('user01', d1)
    cli.update_row('user02', d2)

class JubadumpAnomalyTest(JubaTestCase, JubadumpTestBase):
  def get_config(self):
    config = get_configs(ANOMALY)['lof']
    config['parameter']['method'] = 'inverted_index'
    return (ANOMALY, config)

  def do_training(self, target, cli):
    d1 = target.types.Datum({"string": "abc", "number": 123})
    d2 = target.types.Datum({"string": "abc", "number": 456})
    cli.add(d1)
    cli.add(d2)
