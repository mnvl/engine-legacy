
#include <d3dx9.h>
#include <stddef.h>
#include <graphic/vertex.h>

namespace graphic
{

D3DVERTEXELEMENT9 *vertex_traits<vertex_pnt>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{ 0, offsetof(vertex_pnt, position), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, offsetof(vertex_pnt, normal),   D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, offsetof(vertex_pnt, texcoords), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	return descr;
}

D3DVERTEXELEMENT9 *vertex_traits<vertex_pnjwt>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{ 0, offsetof(vertex_pnjwt, position),     D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, offsetof(vertex_pnjwt, normal),       D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, offsetof(vertex_pnjwt, joints),       D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
        { 0, offsetof(vertex_pnjwt, joints) + 4,   D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 1 },
		{ 0, offsetof(vertex_pnjwt, weights),      D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
        { 0, offsetof(vertex_pnjwt, weights) + 16, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1 },
		{ 0, offsetof(vertex_pnjwt, texcoords),    D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	return descr;
}

}
