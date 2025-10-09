#!/bin/bash

# auch in der shell zum compilieren, sonst im Simulator Fehler bei Zahlliteralen wie 0.1
export LANG=C

cmake -DDEBUG=YES -DCMAKE_BUILD_TYPE=Release -DVCONTROLS=YES -DUSE_UNSUPPORTED_TOOLCHAIN=YES -DTRANSLATIONS=DE -DPCB=X10 -DPCBREV=TX16S -DLUA=YES -DLUA_MIXER=YES -DGVARS=YES ..

