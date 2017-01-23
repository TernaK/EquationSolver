#!/usr/bin/env sh
set -e

~/caffe/tools/caffe train --solver=equation_solver_solver.prototxt $@
