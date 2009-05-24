
ADD_DEFINITIONS (-DDIRECTINPUT_VERSION=0x0800)

ADD_LIBRARY (platform STATIC
	timer.h

	file_handle.h
	file_handle.cpp

    keyboard.h
    keyboard.cpp
    mouse.h
    mouse.cpp
    input.h
    input.cpp

	window.h
	window.cpp
)
