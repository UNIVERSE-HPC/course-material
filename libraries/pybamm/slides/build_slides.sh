#!/bin/bash
# Run this in the Slides directory to rebuild for local use

pandoc -t revealjs -s -o parameters_and_outputs.html parameters_and_ouputs.md --include-in-header=slides.css -V theme=beige  --slide-level=3 -V revealjs-url=https://cdnjs.cloudflare.com/ajax/libs/reveal.js/3.9.2 
pandoc -t revealjs -s -o next_steps.html next_steps.md --include-in-header=slides.css -V theme=beige  --slide-level=3 -V revealjs-url=https://cdnjs.cloudflare.com/ajax/libs/reveal.js/3.9.2
