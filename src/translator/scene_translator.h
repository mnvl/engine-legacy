#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <map>
#include "mesh_translator.h"

class MFnDagNode;
class MDagPath;

class SceneTranslator: public MeshTranslator {
public:
	static void *creator();

protected:
	enum {
		tag_agent = 0x03030303,
		tag_scene = 0x04040404
	};

	bool first_agent;
	math::aabb<3> scene_aabb;

	SceneTranslator();
	~SceneTranslator();

	MString defaultExtension() const;
	MString filter() const;

	MStatus exportSelection();
	MStatus exportAll();

	typedef std::map<std::string,std::string> attribute_vector_type;

	MStatus extractAttributes(const MFnDependencyNode &node,attribute_vector_type &attributes);
	MStatus extractAttributes(const MDagPath &dagPath,attribute_vector_type &attributes);

	void exportAgent(const attribute_vector_type &attributes);

	MStatus startExport();
	MStatus exportMesh(const MDagPath &dagPath);
	MStatus exportAmbientLight(const MDagPath &dagPath);
	MStatus exportDirectionalLight(const MDagPath &dagPath);
	MStatus exportPointLight(const MDagPath &dagPath);
	MStatus exportSpotLight(const MDagPath &dagPath);
	MStatus finishExport();
};
