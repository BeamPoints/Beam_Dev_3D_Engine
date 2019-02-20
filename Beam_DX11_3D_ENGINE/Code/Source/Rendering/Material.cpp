#include "../../Include/Rendering/Material.h"

bool CMaterial::LoadShader(std::string const & aFilename, std::vector<char>& aByteCode)
{
	std::ifstream file;
	file.open(aFilename, std::ios::binary);

	bool const ok = not (file.eof() or file.fail());
	if (ok)
	{
		file.seekg(0, std::ios_base::end);
		std::streampos fileSize = file.tellg();

		aByteCode.resize(fileSize);

		file.seekg(0, std::ios_base::beg);
		file.read(aByteCode.data(), fileSize);
	}

	return ok;
}

std::vector<D3D11_INPUT_ELEMENT_DESC> const CMaterial::MaterialInfo::sVertexLayout =
{
	{
		"POSITION", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		1,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		2,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TEXCOORD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		3,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	}
};