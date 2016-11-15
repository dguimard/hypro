# Dependencies:
- requires at least cmake 2.8, gcc-4.9, boost 1.55, eigen3, glpk
- carl (https://github.com/smtrat/carl)

# Installation:
- create some build folder in the root directory of the library (e.g. build) and enter it
- run 
<pre><code>cmake ..</code><pre>
- (optional) configure the library via gui (requires cmake-curses-gui):
<pre><code>ccmake ..</code><pre>
- build via: <pre><code>make resources
make</code></pre>

# Documentation
https://hypro.github.io/hypro/html/


Current TravisCi build state: 

[![Build Status](https://travis-ci.org/hypro/hypro.svg?branch=master)](https://travis-ci.org/hypro/hypro)