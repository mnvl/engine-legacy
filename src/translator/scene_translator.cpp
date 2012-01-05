//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>

#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>
#include <maya/MColor.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>
#include <maya/MPlugArray.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MIOStream.h>
#include <maya/MFStream.h>
#include <maya/MItDependencyGraph.h>

#include <math/vec.h>
#include <math/matrix.h>

#include <util/logger.h>

#include "convert.h"
#include "scene_translator.h"

SceneTranslator::SceneTranslator() {
}

SceneTranslator::~SceneTranslator() {
}

void *SceneTranslator::creator() {
	return new SceneTranslator();
}

MString SceneTranslator::defaultExtension() const {
	return "scene";
}
MString SceneTranslator::filter() const {
	return "*.scene";
}

MStatus SceneTranslator::extractAttributes(const MFnDependencyNode &node,attribute_vector_type &attributes) {
	MStatus status;

	unsigned i, attributes_count = node.attributeCount();

	for(i = 0; i < attributes_count; ++i) {
		MObject attribute_object = node.attribute(i);
		MFnAttribute attribute(attribute_object,&status);
		if(status != MStatus::kSuccess) continue;

		if(attribute.isDynamic()) {
			MPlug attribute_plug = node.findPlug(attribute_object,&status);
			if(status != MStatus::kSuccess) {
				LOG_DEBUG(attribute.name().asChar() << " " << attribute_object.apiTypeStr() <<" get_plug failed: "<<status<<"!\n");
				continue;
			}

			MString attribute_value;
			status = attribute_plug.getValue(attribute_value);
			if(status != MStatus::kSuccess) {
				LOG_DEBUG(attribute.name().asChar() << " " << attribute_object.apiTypeStr() <<" get_value failed: "<<status<<"!\n");
				continue;
			}

			attributes.insert(std::make_pair(attribute.name().asChar(),attribute_value.asChar()));
		}
	}

	attributes.insert(std::make_pair("name", node.name().asChar()));

	return MStatus::kSuccess;
}

MStatus SceneTranslator::extractAttributes(const MDagPath &dagPath,attribute_vector_type &attributes) {
	if(dagPath.hasFn(MFn::kTransform)) {
		MFnTransform transform(dagPath);
		return extractAttributes(transform,attributes);
	} else {
		MFnDependencyNode dependency_node(dagPath.node());
		return extractAttributes(dependency_node,attributes);
	}
}

MStatus SceneTranslator::exportSelection() {
	LOG_DEBUG("warning: exportSelection command is unsupported, using exportAll instead\n");
	return exportAll();
}

MStatus SceneTranslator::exportAll() {
	//create an iterator for only the mesh components of the DAG
	MStatus status;
	MItDag itDag(MItDag::kDepthFirst, MFn::kInvalid, &status);

	if (MStatus::kFailure == status) {
		MGlobal::displayError("MItDag::MItDag");
		return MStatus::kFailure;
	}

	startExport();

	while(!itDag.isDone()) {
		//get the current DAG path
		MDagPath dagPath;
		if (MStatus::kFailure == itDag.getPath(dagPath)) {
			MGlobal::displayError("MDagPath::getPath");
			return MStatus::kFailure;
		}

		// экспоритровать
		if(dagPath.hasFn(MFn::kMesh) && dagPath.apiType()==MFn::kTransform) exportMesh(dagPath);
		else if(dagPath.hasFn(MFn::kAmbientLight)) exportAmbientLight(dagPath);
		else if(dagPath.hasFn(MFn::kDirectionalLight)) exportDirectionalLight(dagPath);
		else if(dagPath.hasFn(MFn::kPointLight)) exportPointLight(dagPath);
		else if(dagPath.hasFn(MFn::kSpotLight)) exportSpotLight(dagPath);

		itDag.next();
	}

	finishExport();

	return MStatus::kSuccess;
}

MStatus SceneTranslator::startExport() {
	boost::uint32_t data_tag = tag_scene;
	file_stream.write((char const *) &data_tag, sizeof(data_tag));
	file_stream.write((char const *) &scene_aabb, sizeof(scene_aabb));
	first_agent = true;
	return MStatus::kSuccess;
}

void SceneTranslator::exportAgent(const attribute_vector_type &attributes) {
	boost::uint32_t data_tag = tag_agent, attributes_count = attributes.size();
	file_stream.write((char const *) &data_tag, sizeof(data_tag));
	file_stream.write((char const *) &attributes_count, sizeof(attributes_count));
	attribute_vector_type::const_iterator attribute_it = attributes.begin();
	while(attribute_it != attributes.end()) {
		LOG_DEBUG(attribute_it->first<<" = "<<attribute_it->second);
		file_stream.write(attribute_it->first.c_str(), attribute_it->first.size() + 1);
		file_stream.write(attribute_it->second.c_str(), attribute_it->second.size() + 1);
		attribute_it++;
	}
}

MStatus SceneTranslator::exportMesh(const MDagPath &dagPath) {
	LOG_DEBUG("\n\n\nAgent path="<<dagPath.partialPathName().asChar());

	attribute_vector_type attributes;
	extractAttributes(dagPath,attributes);

	exportAgent(attributes);

	if(attributes.find("reference") == attributes.end()) {
		MeshTranslator::exportMesh(dagPath);
	} else {
		extractTransform(dagPath);
		extractGeometry(dagPath);
		file_stream.write((char const *) &mesh_transform, sizeof(mesh_transform));

		position_array.clear();
		normal_array.clear();
		tex2_array.clear();
	}

	if(first_agent) {
		scene_aabb = mesh_aabb;
		first_agent = false;
	} else {
		scene_aabb.extend(mesh_aabb);
	}

	return MStatus::kSuccess;
}

MStatus SceneTranslator::exportAmbientLight(const MDagPath &dagPath) {
	MStatus status;

	MFnAmbientLight light(dagPath,&status);
	if(status != MStatus::kSuccess) return status;

	attribute_vector_type attributes;

	attributes["classname"]="light_ambient";
	attributes["name"]=light.partialPathName().asChar();
	attributes["color"]=convert(light.color()).to_string();
	attributes["intensity"]=boost::lexical_cast<std::string>(light.intensity());
	attributes["reference"]="<empty>";
	attributes["shape_file"]="<empty>";

	exportAgent(attributes);

	return MStatus::kSuccess;
}

MStatus SceneTranslator::exportDirectionalLight(const MDagPath &dagPath) {
	MStatus status;
	MFnDirectionalLight light(dagPath,&status);

	if(status != MStatus::kSuccess) return status;

	attribute_vector_type attributes;
	attributes["classname"]="light_directional";
	attributes["name"]=light.partialPathName().asChar();
	attributes["color"]=convert(light.color()).to_string();
	attributes["intensity"]=boost::lexical_cast<std::string>(light.intensity());
	attributes["direction"]=convert(light.lightDirection()).to_string();
	attributes["reference"]="<empty>";
	attributes["shape_file"]="<empty>";

	exportAgent(attributes);

	return MStatus::kSuccess;
}

MStatus SceneTranslator::exportPointLight(const MDagPath &dagPath) {
	MStatus status;

	MFnPointLight light(dagPath,&status);
	if(status != MStatus::kSuccess) return status;

	MFnTransform transform(dagPath,&status);
	if(status != MStatus::kSuccess) return status;

	attribute_vector_type attributes;
	attributes["classname"]="light_directional";
	attributes["name"]=light.partialPathName().asChar();
	attributes["color"]=convert(light.color()).to_string();
	attributes["intensity"]=boost::lexical_cast<std::string>(light.intensity());
	attributes["position"]=convert(transform.translation(MSpace::kWorld)).to_string();
	attributes["direction"]=convert(light.lightDirection()).to_string();
	attributes["reference"]="<empty>";
	attributes["shape_file"]="<empty>";

	exportAgent(attributes);

	return MStatus::kSuccess;
}

MStatus SceneTranslator::exportSpotLight(const MDagPath &dagPath) {
	MStatus status;

	MFnSpotLight light(dagPath,&status);
	if(status != MStatus::kSuccess) return status;

	MFnTransform transform(dagPath,&status);
	if(status != MStatus::kSuccess) return status;

	attribute_vector_type attributes;
	attributes["classname"]="light_directional";
	attributes["name"]=light.partialPathName().asChar();
	attributes["color"]=convert(light.color()).to_string();
	attributes["intensity"]=boost::lexical_cast<std::string>(light.intensity());
	attributes["position"]=convert(transform.translation(MSpace::kWorld)).to_string();
	attributes["direction"]=convert(light.lightDirection()).to_string();
	attributes["cone_angle"]=boost::lexical_cast<std::string>(light.coneAngle());
	attributes["penumbra_angle"]=boost::lexical_cast<std::string>(light.penumbraAngle());
	attributes["dropoff"]=boost::lexical_cast<std::string>(light.dropOff());
	attributes["reference"]="<empty>";
	attributes["shape_file"]="<empty>";

	exportAgent(attributes);

	return MStatus::kSuccess;
}

MStatus SceneTranslator::finishExport()
{
	exported_material_names.clear();

	file_stream.seekp(sizeof(boost::uint32_t), std::ios_base::beg);
	file_stream.write((char const *) &scene_aabb, sizeof(scene_aabb));

	LOG_DEBUG("\n\nscene aabb = " << scene_aabb);

	return MStatus::kSuccess;
}
