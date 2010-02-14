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

#include <math/vec.h>

namespace graphic
{

#pragma pack(push, 1)
struct DirectionalLightDescr
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	D3DXVECTOR3 dir;
};
#pragma pack(pop)

}
