#!/bin/bash
# Run this in the Slides directory to rebuild for local use

pandoc -t revealjs -s -o index.html index.md -V theme=black --slide-level=3 --metadata tile="session introduction"
