#!/bin/sh
convert -delay 20 -loop 0 initial_colors.ppm alg_linear_result_*.ppm animated.gif
