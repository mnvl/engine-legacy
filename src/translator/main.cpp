//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

#include "mesh_translator.h"
#include "scene_translator.h"

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "(c) 2004-2010 Manvel Avetisian <manvel.avetisian@gmail.com>", "2", "Any");

	status =  plugin.registerFileTranslator("Mesh Translator", "", MeshTranslator::creator, "", "", true);

	if (!status) {
		status.perror("registerFileTranslator");
		return status;
	}

	status =  plugin.registerFileTranslator("Scene Translator", "", SceneTranslator::creator, "", "", true);
	return status;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status =  plugin.deregisterFileTranslator("Mesh Translator");
	if (!status) {
		status.perror("deregisterFileTranslator");
		return status;
	}

	status =  plugin.deregisterFileTranslator("Scene Translator");
	if (!status) {
		status.perror("deregisterFileTranslator");
		return status;
	}

	return status;
}
