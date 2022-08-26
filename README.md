[![Build Status](https://travis-ci.org/neurosuite/neuroscope.svg?branch=blackrock)](https://travis-ci.org/neurosuite/neuroscope)
[![Build status](https://ci.appveyor.com/api/projects/status/00g6lmitve63gxpc/branch/blackrock?svg=true)](https://ci.appveyor.com/project/FlorianFranzen/neuroscope/branch/blackrock)

Neuroscope
==========

NeuroScope is an advanced viewer for electrophysiological and behavioral data (with limited editing capabilities): it can display local field potentials (EEG), neuronal spikes, behavioral events, and the position of the animal in the environment.

Developed by Lynn Hazan (main developer), Montel Laurent (qt3 to qt4/5 porting), David Faure (qt3 to qt4/5 porting), Michaël Zugaro (plugins, maintenance) and Florian Franzen (blackrock support, maintenance), distributed under the GNU Public License v2.

Compile software on Ubuntu 20.04

## Documentation
For more information about this software please read:

- [Neuroscope handbook](http://neuroscope.sourceforge.net/UserManual/index.html)

## Compile guide

  1. Install [Libneurosuite](https://github.com/natuneuro/neurosuite) and [LibKlustersShared](https://github.com/natuneuro/LibKlustersShared) before compiling Neuroscope 
  
  2. Go to `/Neuroscope` directory and type:
  ```
  mkdir build
  cd build
  cmake ..
  make
  sudo make install
  ```
