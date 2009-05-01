
ADD_LIBRARY (engine STATIC
	exception/directx_error.h
	exception/winapi_error.h

	graphic/collada_loader.cpp
	graphic/model.h
	graphic/collada_loader.h
	graphic/font.h
	graphic/font.cpp
	graphic/camera.cpp
	graphic/memory_mesh.cpp
	graphic/camera.h
	graphic/memory_model.h
	graphic/memory_model.cpp
	graphic/model.cpp
	graphic/memory_mesh.h
	graphic/renderer.h
	graphic/renderer.cpp

	system/window.cpp
	system/window.h
	system/timer.h

	util/boost_com_intrusive_ptr.h
	util/xerces_lcp_transcoder.h
)
