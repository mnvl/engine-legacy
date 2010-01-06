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

#include <boost/algorithm/string.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>

#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItDag.h>
#include <maya/MPlug.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MPlugArray.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnTransform.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

#include <util/logger.h>

#include "convert.h"
#include "mesh_translator.h"

MeshTranslator::MeshTranslator()
{
}

MeshTranslator::~MeshTranslator() {
}

void *MeshTranslator::creator() {
	return new MeshTranslator();
}

MStatus MeshTranslator::writer(const MFileObject &file,const MString &optionsString,FileAccessMode mode) {
	// вывести шапку
	LOG_DEBUG("--------------------------------------------------------------------------------");
	LOG_DEBUG("starting export, file_name = " << file.fullName().asChar());

	// открыть файл
	file_stream.open(file.fullName().asChar(),std::ios_base::out | std::ios_base::binary);

	// записать название сцены
	scene_name = file.name().asChar();
	boost::to_lower(scene_name);
	boost::replace_last(scene_name, ".scene", "");

	// получить режим экспорта и вызвать соответствующую функцию
	if (MPxFileTranslator::kExportAccessMode == mode) {
		if (MStatus::kFailure == exportAll()) {
			return MStatus::kFailure;
		}
	} else if (MPxFileTranslator::kExportActiveAccessMode == mode) {
		if (MStatus::kFailure == exportSelection()) {
			return MStatus::kFailure;
		}
	} else {
		return MStatus::kFailure;
	}

	return MStatus::kSuccess;
}

bool MeshTranslator::haveWriteMethod() const {
	return true;
}

MString MeshTranslator::defaultExtension() const {
	return "mesh";
}

MString MeshTranslator::filter() const {
	return "*.mesh";
}

MStatus MeshTranslator::exportSelection() {
	MStatus status;

	//create an iterator for the selected mesh components of the DAG
	MSelectionList selectionList;
	if (MStatus::kFailure == MGlobal::getActiveSelectionList(selectionList)) {
		MGlobal::displayError("MGlobal::getActiveSelectionList");
		return MStatus::kFailure;
	}

	MItSelectionList itSelectionList(selectionList, MFn::kMesh, &status);	
	if (MStatus::kFailure == status) {
		return MStatus::kFailure;
	}

	for (itSelectionList.reset(); !itSelectionList.isDone(); itSelectionList.next()) {
		MDagPath dagPath;

		//get the current dag path and process the poly mesh on it
		//
		if (MStatus::kFailure == itSelectionList.getDagPath(dagPath)) {
			MGlobal::displayError("MItSelectionList::getDagPath");
			return MStatus::kFailure;
		}

		if (MStatus::kFailure == exportMesh(dagPath)) {
			return MStatus::kFailure;
		}
	}
	return MStatus::kSuccess;
}

MStatus MeshTranslator::exportAll() {
	MStatus status;

	//create an iterator for only the mesh components of the DAG
	MItDag itDag(MItDag::kDepthFirst, MFn::kMesh, &status);

	if (MStatus::kFailure == status) {
		MGlobal::displayError("MItDag::MItDag");
		return MStatus::kFailure;
	}

	for(;!itDag.isDone();itDag.next()) {
		//get the current DAG path
		//
		MDagPath dagPath;
		if (MStatus::kFailure == itDag.getPath(dagPath)) {
			MGlobal::displayError("MDagPath::getPath");
			return MStatus::kFailure;
		}

		MFnDagNode visTester(dagPath);

		//if this node is visible, then process the poly mesh it represents
		//
		MStatus status;
		if(isVisible(visTester, status) && MStatus::kSuccess == status) {
			if (MStatus::kFailure == exportMesh(dagPath)) {
				return MStatus::kFailure;
			}
		}
	}
	return MStatus::kSuccess;
}

bool MeshTranslator::isVisible(MFnDagNode &fnDag, MStatus &status) {
	if(fnDag.isIntermediateObject())
		return false;

	MPlug visPlug = fnDag.findPlug("visibility", &status);
	if (MStatus::kFailure == status) {
		MGlobal::displayError("MPlug::findPlug");
		return false;
	} else {
		bool visible;
		status = visPlug.getValue(visible);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MPlug::getValue");
		}
		return visible;
	}
}

MStatus MeshTranslator::exportMaterial(const MPlug &shader_plug) {
	if(shader_plug.isNull()) return MStatus::kFailure;

	MPlugArray connectedPlugs;

	//get all the plugs that are connected as the destination of this 
	//surfaceShader plug so we can find the surface shaderNode
	//
	MStatus status;
	shader_plug.connectedTo(connectedPlugs, true, false, &status);
	if (MStatus::kFailure == status) {
		MGlobal::displayError("MPlug::connectedTo");
	}

	if (1 != connectedPlugs.length()) return MStatus::kFailure;

	std::string name;
	math::vec<4> ambient, diffuse, specular, emission;
	math::scalar shininess;

	MObject shader=connectedPlugs[0].node();
	if (shader.hasFn(MFn::kPhong)) {
		MFnPhongShader phong( shader );
		mesh_material_names.push_back(phong.name().asChar());

		if(std::find(exported_material_names.begin(),exported_material_names.end(),phong.name().asChar())
			!= exported_material_names.end()) return MStatus::kSuccess;

		LOG_DEBUG("Found Phong shader: " << phong.name().asChar());
		exported_material_names.push_back(phong.name().asChar());

		name = phong.name().asChar();
		ambient = convert(phong.ambientColor());
		diffuse = convert(phong.color()) * phong.diffuseCoeff();
		specular = convert(phong.specularColor());
		emission = convert(phong.color())*phong.glowIntensity();
		shininess = phong.glowIntensity();
	} else if(shader.hasFn(MFn::kLambert)) {
		MFnLambertShader lambert( shader );
		mesh_material_names.push_back(lambert.name().asChar());

		if(std::find(exported_material_names.begin(),exported_material_names.end(),lambert.name().asChar())
			!= exported_material_names.end()) return MStatus::kSuccess;

		LOG_DEBUG( "Found Lambert shader: " << lambert.name().asChar());
		exported_material_names.push_back(lambert.name().asChar());

		name = lambert.name().asChar();
		ambient = convert(lambert.ambientColor());
		diffuse = math::vec<4>(lambert.diffuseCoeff(),lambert.diffuseCoeff(),lambert.diffuseCoeff(),1.0f);
		specular = math::vec<4>(0.0f,0.0f,0.0f,1.0f);
		emission = convert(lambert.color())*lambert.glowIntensity();
		shininess = lambert.glowIntensity();
	} else {
		LOG_WARNING("can't extract shader");
		return MStatus::kFailure;
	}

	boost::uint32_t data_tag = tag_material;
	file_stream.write((char const *) &data_tag, sizeof(data_tag));
	file_stream.write((char const *) name.c_str(), name.size() + 1);
	file_stream.write((char const *) &ambient, sizeof(ambient));
	file_stream.write((char const *) &diffuse, sizeof(diffuse));
	file_stream.write((char const *) &specular, sizeof(specular));
	file_stream.write((char const *) &emission, sizeof(emission));
	file_stream.write((char const *) &shininess, sizeof(shininess));

	std::string texture_name;
	if(extractTextureName(shader_plug,texture_name) == MStatus::kSuccess) {
		LOG_DEBUG("texture_name = "<<texture_name);
	} else {
		LOG_WARNING("no texture found\n");
	}
	file_stream.write(texture_name.c_str(), texture_name.size() + 1);

	return MStatus::kSuccess;
}

MStatus MeshTranslator::extractTextureName(const MPlug &shaderPlug,std::string &texture_name) {
	MStatus status;

	// get all of the attributes connected to the shader
	MPlugArray connectedPlugs;
	shaderPlug.connectedTo(connectedPlugs, true, false, &status);

	// get the color attribute (should be the first one)
	MPlug colorPlug = MFnDependencyNode(connectedPlugs[0].node()).findPlug("color",&status);

	// gather all of the texture nodes from the color attribute into an iterator
	MItDependencyGraph itDG(colorPlug, MFn::kFileTexture, MItDependencyGraph::kUpstream, MItDependencyGraph::kBreadthFirst, MItDependencyGraph::kNodeLevel);

	//disable automatic pruning so that we can locate a specific plug 
	itDG.disablePruningOnFilter();

	// if there are any texture nodes (there's probably only 1)
	if (!itDG.isDone())
	{
		// get the filename attribute from the first node we have
		MPlug filenamePlug = MFnDependencyNode(itDG.thisNode()).findPlug("fileTextureName", &status);

		// finally, get the texture name from the attribute
		MString textureName;
		status = filenamePlug.getValue(textureName);
		texture_name = textureName.asChar();

		std::string::size_type p1,p2;
		p1=texture_name.rfind('/');
		if(p1==std::string::npos) p1=texture_name.rfind('\\');
		if(p1==std::string::npos) p1=0; else p1++;
		p2=texture_name.rfind('.');
		if(p2==std::string::npos) p2=texture_name.size(); else p2--;
		texture_name = texture_name.substr(p1,p2-p1+1);

		return MStatus::kSuccess;
	}

	return MStatus::kFailure;
}

MStatus MeshTranslator::extractTransform(const MDagPath &path) {
	MStatus status;

	MFnTransform transform(path,&status);
	if(status != MStatus::kSuccess) return status;

	MTransformationMatrix transform_matrix = transform.transformation(&status);
	if(status != MStatus::kSuccess) return status;

	mesh_origin = convert(transform_matrix.translation(MSpace::kWorld));
	mesh_rotation = convert(transform_matrix.rotation());
	LOG_DEBUG("mesh_origin = " << mesh_origin);

	double scale[3];
	transform_matrix.getScale(scale,MSpace::kWorld);
	mesh_scale = math::vec<3>(math::scalar(scale[0]),math::scalar(scale[1]),math::scalar(scale[2]));
	LOG_DEBUG("mesh_scale = " << mesh_scale);

	double shear[3];
	transform_matrix.getShear(shear,MSpace::kWorld);
	mesh_shear = math::vec<3>(math::scalar(shear[0]),math::scalar(shear[1]),math::scalar(shear[2]));
	LOG_DEBUG("mesh_shear = " << mesh_shear);

	mesh_pivot = convert(transform_matrix.rotatePivot(MSpace::kWorld));
	LOG_DEBUG("mesh_pivot = " << mesh_pivot);

	#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
	LOG_DEBUG("transform matrix: ";
	#endif

	MMatrix M = path.inclusiveMatrix();
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {

			mesh_transform.ij[i][j] = math::scalar(M.matrix[i][j]);

			#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
			LOG_DEBUG(mesh_transform.ij[i][j] << " ";
			#endif
		}
	}

	mesh_transform_determinant = mesh_transform.determinant();

	#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
	LOG_DEBUG("\n mesh_transform_determinant = " << mesh_transform_determinant << std::endl << std::flush;
	#endif

	return status;
} 

MStatus MeshTranslator::extractGeometry(MFnMesh const &mesh) {
	unsigned i,j,m,n;
	math::vec<3> v0;

	MPointArray points;
	mesh.getPoints(points,MSpace::kObject);

	const math::scalar units_to_meters = 1.0f;

	#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
	meta_debug_log() << "position_array:\n";
	#endif

	n=points.length();
	position_array.resize(n);
	mesh_aabb.null();
	for(i=0; i<n; i++) {
		position_array[i].x = math::scalar(points[i].x) * units_to_meters;
		position_array[i].y = math::scalar(points[i].y) * units_to_meters;
		position_array[i].z = math::scalar(points[i].z) * units_to_meters;

		#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
		meta_debug_log() << "i = " << i <<", "<< boost::lexical_cast<std::string>(position_array[i]) << std::endl << std::flush;
		#endif

		if(i == 0) mesh_aabb.min = mesh_aabb.max = position_array[i];
		else mesh_aabb.extend(position_array[i]);
	}

	MFloatVectorArray normals;
	mesh.getNormals(normals,MSpace::kObject);

	n=normals.length();
	normal_array.resize(n);
	for(i=0; i<n; i++) {
		normal_array[i].x = normals[i].x;
		normal_array[i].y = normals[i].y;
		normal_array[i].z = normals[i].z;
	}

	#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
	meta_debug_log() << "uv_array:\n";
	#endif

	MStringArray uvset_names;
	mesh.getUVSetNames(uvset_names);

	if(uvset_names.length() != 1)
	{
        LOG_WARNING("uvsets_count = "<<uvset_names.length());
	}

  	MFloatArray u_array,v_array;
	for(j=0, m=0; j<uvset_names.length(); j++) {
		mesh.getUVs(u_array,v_array,0);

		if(u_array.length() != v_array.length()) LOG_WARNING("UV array mismatch");

		n = u_array.length();
		tex2_array.resize(m+n);

		for(i=0; i<n; i++) {
			tex2_array[m].u = u_array[i];
			tex2_array[m].v = v_array[i];

			#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
			meta_debug_log() << "i = " << i << ", uv = (" << u_array[i] << ", " << v_array[i] << ")\n";
			#endif

			m++;
		}
	}

	LOG_DEBUG(position_array.size() << " points, "
		<< normal_array.size()   << " normals, "
		<< tex2_array.size()     << " uvs\n");

	return MStatus::kSuccess;
}

MStatus MeshTranslator::extractIndices(MFnMesh const &mesh) {
	MStatus status;
	MIntArray position_index_array,normal_index_array;
	int uv_index;

	#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
	meta_debug_log() << "face_array:\n";
	#endif

	unsigned i,n=mesh.numPolygons(),m;
	LOG_DEBUG(n << " faces\n");

	polygon_normal.resize(n);
	
	for(i=0; i<n; i++) {
		mesh.getPolygonVertices(i,position_index_array);
		mesh.getFaceNormalIds(i,normal_index_array);

		m=mesh.polygonVertexCount(i);

		if(m!=3) {
			LOG_DEBUG("Error: only triangular faces allowed!");
			return MStatus::kFailure;
		}

		#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
		LOG_DEBUG("face_index = " << i);
		#endif

		face_type face;
		for(unsigned j=0; j<m; j++) {
			mesh.getPolygonUVid(i,j,uv_index,0);
			if(uv_index < 0) {
				LOG_WARNING("negative UV index for vertex " << j
					           << " of face " << i << ", defaulting to zero");
				uv_index = 0;
			}

			index_type index;
			index.position_index = position_index_array[j];
			index.normal_index = normal_index_array[j];
			index.tex2_index = uv_index;

			#if defined(TRANSLATOR_PARANOIDAL_LOGGING)
			LOG_WARNING("\tvertex = " << j
				<< ", position_index = " << index.position_index
				<< ", normal_index = " << index.normal_index
				<< ", tex2_index = " << index.tex2_index);
			#endif

			face.i[j] = index;
		}

		MVector normal_vec;
		mesh.getPolygonNormal(i,normal_vec,MSpace::kObject);
		polygon_normal[i] = convert(normal_vec);

		face_array.push_back(face);
	}

	return MStatus::kSuccess;
}

MStatus MeshTranslator::extractMaterials(MFnMesh const &mesh) {
	unsigned i,n;
	MStatus status;

	MObjectArray shaders;
	MIntArray indices;
	mesh.getConnectedShaders(instance_number,shaders,indices);

	n=shaders.length();
	LOG_DEBUG(n <<" materials found\n");
	for(i=0; i<n; i++) {
		MFnDependencyNode node(shaders[i],&status);
		MPlug plug = node.findPlug("surfaceShader",&status);
		exportMaterial(plug);
	}

	n=indices.length();
	for(i=0; i<n; i++) {
		polygon_material.push_back(std::make_pair(indices[i],i));
	}

	std::sort(polygon_material.begin(),polygon_material.end());

        LOG_DEBUG("polygon_material_size = " << polygon_material.size()
			<< ", face_array_size = " << face_array.size());

        // обновить массив индексов в соответствии с polygon_material
        std::vector<face_type> face_array_copy = face_array;

        for(unsigned i = 0, n = polygon_material.size(); i < n; i++) {
                face_array[i] = face_array_copy[polygon_material[i].second];
        }

	return status;
}

void MeshTranslator::constructVertexIndex() {
	unsigned i,j,n;

	typedef std::map<index_type,boost::uint32_t> vertex_hash_type;
	boost::uint32_t vertex_index = 0;
	vertex_hash_type vertex_hash;
        int originals_count = 0, matches_count = 0;

	math::aabb<3> face_aabb;

	for(i=0, n=face_array.size(); i < n; i++) {
		boost::uint32_t face_vertex[3];

		for(j = 0; j < 3; j++) {
			index_type &index = face_array[i].i[j];

			if(index.tex2_index > tex2_array.size()) {
				LOG_WARNING("warning: invalid uv, index = "<<index.tex2_index<<", face = "<<i);
				index.tex2_index = 0;
			}

			#if !defined(TRANSLATOR_PRIMITIVE_EXPORT)
			vertex_hash_type::iterator vertex_it = vertex_hash.find(index);
			if(vertex_it == vertex_hash.end()) {
			#endif
				vertex_hash.insert(std::make_pair(index,vertex_index));

				vertex_type vertex;
				vertex.position = position_array[index.position_index];
				vertex.normal = normal_array[index.normal_index];
				vertex.texcoords = tex2_array[index.tex2_index];

				vertex_array.push_back(vertex);

				face_vertex[j] = vertex_index;

				originals_count++;
				vertex_index++;
			#if !defined(TRANSLATOR_PRIMITIVE_EXPORT)
			} else {
				face_vertex[j] = vertex_it->second;
				matches_count++;
			}
			#endif
		}

		// записать индексы
		index_array.push_back(face_vertex[0]);
		index_array.push_back(face_vertex[1]);
		index_array.push_back(face_vertex[2]);
	}

	LOG_DEBUG("vertex_buffer: "<<originals_count<<" original, " <<matches_count<<" matches");
}

MStatus MeshTranslator::exportMesh(const MDagPath &path)
{
	unsigned i,j,n;

	MStatus status;

	// получить меш
	MFnMesh mesh(path,&status);
	LOG_DEBUG("mesh = "<<mesh.partialPathName().asChar());

	instance_number = 0;
	if (path.isInstanced(&status)) {
		instance_number = path.instanceNumber();
		LOG_DEBUG("instance_number = "<<instance_number);
	}

	// получить преобразования
	LOG_DEBUG("*** extractTransform()");
	extractTransform(path);

	// получить геометрию
	LOG_DEBUG("*** extractGeometry()");
	extractGeometry(mesh);

	if(position_array.empty()) {
		LOG_DEBUG("warning: position array is empty, skipping.\n");
		return MStatus::kFailure;
	}

	// получить стороны
	LOG_DEBUG("*** extractIndices()");
	if(extractIndices(mesh) == MStatus::kFailure) return MStatus::kFailure;

	// получить маетраиалы
	LOG_DEBUG("*** extractMaterials()");
	extractMaterials(mesh);

	// сконструировать буфер вершин и буфер индексов
	LOG_DEBUG("*** constructVertexIndex()");
	constructVertexIndex();

	// записать наконец меш
	LOG_DEBUG("> mesh header");
	boost::uint32_t data_tag = tag_mesh, vertex_array_size = vertex_array.size(), index_array_size = index_array.size(),
		materials_count = mesh_material_names.size();
	file_stream.write((char const *) &data_tag, sizeof(data_tag));
	file_stream.write((char const *) mesh.partialPathName().asChar(), mesh.partialPathName().length() + 1);
	file_stream.write((char const *) &mesh_transform, sizeof(mesh_transform));
	file_stream.write((char const *) &mesh_aabb, sizeof(mesh_aabb));
	file_stream.write((char const *) &vertex_array_size, sizeof(vertex_array_size));
	file_stream.write((char const *) &index_array_size, sizeof(index_array_size));
	file_stream.write((char const *) &materials_count, sizeof(materials_count));
	
	// *** vertex array ***
	LOG_DEBUG("> vertex array");
	
	n=vertex_array.size();
	LOG_DEBUG("vertex_array_size = " << n);

	for(i=0; i<n; i++) {
		file_stream.write((char const *) &vertex_array[i],sizeof(vertex_type));
	}

	// *** index array ***
	LOG_DEBUG("> index array");

	n=index_array.size();
	LOG_DEBUG("index_array_size = " << n);

	for(i=0; i<n; i++) {
		boost::uint16_t index = index_array[i];
		file_stream.write((char const *) &index, sizeof(index));
	}

	LOG_DEBUG("> material table");
	i=0;
	n=polygon_material.size();
	unsigned index_number = 0;
	while(i<n) {
		// получить материал
		unsigned material = polygon_material[i].first;

		// просканировать полигоны и собрать данные
		unsigned l,min_vertex=unsigned(-1),max_vertex=0;
		for(j=i; i<n && polygon_material[j].first==material; j++) {
			for(l=0; l<3; l++) {
				unsigned index = index_array[index_number];
				if(index < min_vertex) min_vertex = index;
				if(index > max_vertex) max_vertex = index;

				index_number++;
			}
		}

		LOG_DEBUG("material_name = "<<mesh_material_names[material]
			<<", faces_count = "<<(j-i)
			<<", min_vertex = "<<min_vertex
			<<", max_vertex = "<<max_vertex);

		// вывести эти данные
		boost::uint32_t indices_count = j-i, vertices_count = max_vertex-min_vertex;
		file_stream.write(mesh_material_names[material].c_str(), mesh_material_names[material].size() + 1);
		file_stream.write((char const *) &indices_count, sizeof(indices_count));
		file_stream.write((char const *) &min_vertex, sizeof(min_vertex));
		file_stream.write((char const *) &vertices_count, sizeof(vertices_count));

		i=j;
	}

	// очистить данные
	clearBuffers();

	LOG_DEBUG("done.");

	return status;
}

void MeshTranslator::clearBuffers() {
	mesh_material_names.clear();
	exported_material_names.clear();
	polygon_material.clear();
	position_array.clear();
	normal_array.clear();
	tex2_array.clear();
	face_array.clear();
	vertex_array.clear();
	index_array.clear();
}
