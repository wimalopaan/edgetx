#!/bin/bash

# auch in der shell zum compilieren, sonst im Simulator Fehler bei Zahlliteralen wie 0.1
export LANG=C

make submodule init
make submodule update
make configure

make -C arm-none-eabi -j2 firmware

