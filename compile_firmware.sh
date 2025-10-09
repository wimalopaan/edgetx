#!/bin/bash

# auch in der shell zum compilieren, sonst im Simulator Fehler bei Zahlliteralen wie 0.1
export LANG=C

git submodule init
git submodule update
make configure

make -C arm-none-eabi -j2 firmware

