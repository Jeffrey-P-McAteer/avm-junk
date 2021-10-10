
# Compile fresh

import sys
import subprocess
import shutil

subprocess.run([sys.executable, 'setup.py', 'build'], check=True)
shutil.copy('build/lib.linux-x86_64-3.9/vm.cpython-39-x86_64-linux-gnu.so', 'vm.so')
subprocess.run(['clear'])


# Actual test

import vm

def some_func(row):
  print('row={}'.format(row))
  if row[0] > 1:
    return row[0] * 10
  else:
    return 0


vm.runit(some_func)


