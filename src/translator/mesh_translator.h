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

#include <string>
#include <vector>
#include <fstream>

#include <maya/MPxFileTranslator.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MColor.h>
#include <maya/MVector.h>
#include <maya/MFloatVector.h>
#include <maya/MQuaternion.h>

#include <math/vec.h>
#include <math/quaternion.h>
#include <math/matrix.h>
#include <math/aabb.h>

#include <graphic/vertex.h>
#include <graphic/index.h>

class MeshTranslator: public MPxFileTranslator {
public:
	static void *creator();

protected:
	enum {
		tag_material = 0x01010101,
		tag_mesh = 0x02020202,
	};

	// название текущей сцены
	std::string scene_name;

	// файловый поток для данных
	std::fstream file_stream;

	// копия меша
	unsigned instance_number;

	// материалы
	std::vector<std::string> exported_material_names;
	std::vector<std::string> mesh_material_names;

	typedef std::pair<unsigned,unsigned> polygon_material_type;
	typedef std::vector<polygon_material_type> polygon_material_vector_type;
        polygon_material_vector_type polygon_material;

	// геометрия
	math::vec<3> mesh_origin;
	math::quaternion<> mesh_rotation;
	math::vec<3> mesh_scale;
	math::vec<3> mesh_shear;
	math::vec<3> mesh_pivot;
	math::aabb<3> mesh_aabb;
	math::matrix<4,4> mesh_transform;
	math::scalar mesh_transform_determinant;
	std::vector<math::vec<3> > position_array;
	std::vector<math::vec<3> > normal_array;
	std::vector<math::vec<2> > tex2_array;

	// стороны
	struct index_type {
		unsigned position_index, normal_index, tex2_index;

		bool operator <(index_type const &rhs) const {
			if(position_index < rhs.position_index) return true;
			if(position_index > rhs.position_index) return false;

			if(normal_index < rhs.normal_index) return true;
			if(normal_index > rhs.normal_index) return false;

			return tex2_index < rhs.tex2_index;
		}
	};

	struct face_type {
		index_type i[3];
	};

	std::vector<face_type> face_array;
	std::vector<math::vec<3> > polygon_normal;

	// сконвертированная информация
	typedef graphic::vertex_pnt vertex_type;
	typedef std::vector<vertex_type> vertex_array_type;

	vertex_array_type vertex_array;
	std::vector<int> index_array;

	MStatus writer(const MFileObject &file,const MString &optionsString,FileAccessMode mode);
	bool haveWriteMethod() const;
	MString defaultExtension() const;
	MString filter() const;

	// конструктор и деструктор
	MeshTranslator();
	~MeshTranslator();

	virtual MStatus exportSelection();	// вызывает функцию exportMesh() для всех выбранных мешей
	virtual MStatus exportAll();		// вызывает функцию exportMesh() для всех мешей

	// проверяет fnDag на видимость
	bool isVisible(MFnDagNode &fnDag, MStatus& status);

	// записывает материал в файл
	MStatus exportMaterial(const MPlug &shader_plug);

	// получить название текстуры материала
	MStatus extractTextureName(const MPlug &shaderPlug,std::string &texture_name);

	// получает матрицу преобразования
	MStatus extractTransform(const MDagPath &path);

	// получает геометрию из меша
	MStatus extractGeometry(MFnMesh const &mesh);

	// получает стороны из меша
	MStatus extractIndices(MFnMesh const &mesh);

	// получает информацию по материалам меша
	MStatus extractMaterials(MFnMesh const &mesh);

	// сконуструировать буфер вертексов и индексов
	void constructVertexIndex();

	// экспортирует меш
	virtual MStatus exportMesh(const MDagPath &path);

	// освобождает временные буферы, использованные при экспорте
	void clearBuffers();
};
