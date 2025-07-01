#!/bin/bash
# Used to run mpi4py processes using a virtual machine in a distributed system
# based on code from https://docs.alliancecan.ca/wiki/MPI4py

# Needed when running across multiple devices (as specified in the hostfile) AND
# mpi4py is installed using a *VIRTUAL ENVIRONMENT*

PROGRAM=pinFinder.py
HASH=f638f4354ff089323d1a5f78fd8f63ca # your hash

PYTHON=python3
ENV_DIR=env
HOSTFILE=../hosts

# create virtual environment
$PYTHON -m venv $ENV_DIR
source $ENV_DIR/bin/activate

# install dependencies
$ENV_DIR/bin/pip install mpi4py
$ENV_DIR/bin/pip install numpy

# MPI Processes
NUM_PROCS=24

mpirun -np $NUM_PROCS -hostfile $HOSTFILE $ENV_DIR/bin/python ../$PROGRAM $HASH
