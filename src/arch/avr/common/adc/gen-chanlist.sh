#!/bin/bash
#cause I'm a bit too lazy
for i in {0..16}; do
echo config AVR_ADC_CH$i
echo bool \"Enable Channel $i\"
echo 
done
