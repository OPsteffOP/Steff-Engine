#include "pch.h"
#include "MeshData.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

MeshData::MeshData(std::vector<PosTexVertex>&& vertices, std::vector<uint32_t>&& indices)
	: vertices(std::move(vertices))
	, indices(std::move(indices))
{
	CreateBuffers();
}

MeshData::MeshData(std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices)
	: MeshData(std::move(vertices), std::move(indices))
{

}

MeshData::~MeshData()
{
	SAFE_DELETE_DIRECTX(m_pVertexBuffer);
	SAFE_DELETE_DIRECTX(m_pIndexBuffer);
}

void MeshData::CreateBuffers()
{
	// Vertex buffer
	D3D11_BUFFER_DESC vertexBufferDescriptor{};
	vertexBufferDescriptor.ByteWidth = sizeof(PosTexVertex) * unsigned int(vertices.size());
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = NULL;
	vertexBufferDescriptor.MiscFlags = NULL;
	vertexBufferDescriptor.StructureByteStride = sizeof(PosTexVertex);

	D3D11_SUBRESOURCE_DATA vertexBufferInitData{};
	vertexBufferInitData.pSysMem = vertices.data();

	DeviceManager::m_pDevice->CreateBuffer(&vertexBufferDescriptor, &vertexBufferInitData, &m_pVertexBuffer);

	// Index buffer
	D3D11_BUFFER_DESC indexBufferDescriptor{};
	indexBufferDescriptor.ByteWidth = sizeof(uint32_t) * unsigned int(indices.size());
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = NULL;
	indexBufferDescriptor.MiscFlags = NULL;
	indexBufferDescriptor.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA indexBufferInitData{};
	indexBufferInitData.pSysMem = indices.data();

	DeviceManager::m_pDevice->CreateBuffer(&indexBufferDescriptor, &indexBufferInitData, &m_pIndexBuffer);
}

void MeshData::BindVertexBuffer() const
{
	const unsigned int strides{ sizeof(PosTexVertex) };
	const unsigned int offset{ 0 };
	DeviceManager::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offset);
}

void MeshData::BindIndexBuffer() const
{
	DeviceManager::m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}