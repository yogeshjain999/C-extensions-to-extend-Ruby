require 'mkmf'
have_library('opencv_core')
have_library('opencv_highgui')
have_library('opencv_imgproc')
create_makefile('webcam')
