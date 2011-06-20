#!/bin/bash
#cause I'm a bit too lazy
for i in {0..16}; do
echo config AVR_ADC_CH${i}_TAG
echo string \"Channel $i tag\"
echo 
done
