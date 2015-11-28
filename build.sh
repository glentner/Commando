#!/bin/bash
# copied from @jsteemann

echo "Environment: `uname -a`"
echo "Compiler: `$CXX --version`"

make || exit 1
make test || exit 1
