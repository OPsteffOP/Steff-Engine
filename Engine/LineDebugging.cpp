#include "pch.h"
#include "LineDebugging.h"
#include "DeviceManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "Renderer.h"
#include "PosVertexShader.h"
#include "ColPixelShader.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Debugging;

LineDebugging* LineDebugging::m_pInstance{ nullptr };
uint32_t LineDebugging::m_MaxPermanentVertices{ 100 };

LineDebugging::LineDebugging()
	: m_CurrentMaxVertices(100)
{

}

LineDebugging* LineDebugging::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new LineDebugging{};
		m_pInstance->Initialize();
	}

	return m_pInstance;
}

void LineDebugging::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

LineDebugging::~LineDebugging()
{
	SAFE_DELETE_DIRECTX(m_pVertexBuffer);
	SAFE_DELETE_DIRECTX(m_pPermanentVertexBuffer);

	SAFE_DELETE(m_pVertexShader);
	SAFE_DELETE(m_pPixelShader);
}

void LineDebugging::Initialize()
{
	Logging::Log(LogType::INFORMATION, "Initializing line debugging...");

	m_pVertexShader = new PosVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\LineVertexShader.cso" };
	m_pPixelShader = new ColPixelShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\LinePixelShader.cso" };

	Logging::Log(LogType::INFORMATION, "Initialized line debugging");
}

void LineDebugging::InitializeVertexBuffer()
{
	Logging::Log(LogType::INFORMATION, "Initializing vertex buffer for line debugging...");

	// Create vertex buffer
	D3D11_BUFFER_DESC vertexBufferDescriptor{};
	vertexBufferDescriptor.ByteWidth = sizeof(XMFLOAT3) * m_CurrentMaxVertices;
	vertexBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDescriptor.MiscFlags = NULL;
	vertexBufferDescriptor.StructureByteStride = sizeof(XMFLOAT3);

	D3D11_SUBRESOURCE_DATA vertexBufferInitData{};
	vertexBufferInitData.pSysMem = m_Vertices.data();

	HRESULT result = DeviceManager::m_pDevice->CreateBuffer(&vertexBufferDescriptor, &vertexBufferInitData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create vertex buffer for line debugging");
		return;
	}

	Logging::Log(LogType::INFORMATION, "Initialized vertex buffer for line debugging");
}

void LineDebugging::InitializePermanentVertexBuffer()
{
	Logging::Log(LogType::INFORMATION, "Initializing permanent vertex buffer for line debugging...");

	// Create permanent vertex buffer
	D3D11_BUFFER_DESC permanentVertexBufferDescriptor{};
	permanentVertexBufferDescriptor.ByteWidth = sizeof(XMFLOAT3) * m_MaxPermanentVertices;
	permanentVertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	permanentVertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	permanentVertexBufferDescriptor.CPUAccessFlags = NULL;
	permanentVertexBufferDescriptor.MiscFlags = NULL;
	permanentVertexBufferDescriptor.StructureByteStride = sizeof(XMFLOAT3);

	D3D11_SUBRESOURCE_DATA permanentVertexBufferInitData{};
	permanentVertexBufferInitData.pSysMem = m_PermanentVertices.data();

	HRESULT result = DeviceManager::m_pDevice->CreateBuffer(&permanentVertexBufferDescriptor, &permanentVertexBufferInitData, &m_pPermanentVertexBuffer);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create permanent vertex buffer for line debugging");
		return;
	}

	Logging::Log(LogType::INFORMATION, "Initialized permanent vertex buffer for line debugging");
}

// Points in world space
void LineDebugging::AddLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint)
{
	m_Vertices.push_back(beginPoint);
	m_Vertices.push_back(endPoint);

	// Resize vertex buffer if the vertex count goes over the current limit
	if (m_Vertices.size() > m_CurrentMaxVertices)
	{
		m_CurrentMaxVertices *= 2; // double the vertex buffer size (to make sure you don't have to create a new vertex buffer soon - expensive)

		SAFE_DELETE_DIRECTX(m_pVertexBuffer);
		InitializeVertexBuffer();

		return;
	}

	// Initialize if this is the first line
	if (m_pVertexBuffer == nullptr)
	{
		InitializeVertexBuffer();
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mappedVertexBuffer;
	HRESULT result = DeviceManager::m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertexBuffer);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to update the line debugging vertex buffer!");
		return;
	}

	CopyMemory(mappedVertexBuffer.pData, m_Vertices.data(), m_Vertices.size() * sizeof(XMFLOAT3));
	DeviceManager::m_pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

// Points in world space
void LineDebugging::AddPermanentLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint)
{
	m_PermanentVertices.push_back(beginPoint);
	m_PermanentVertices.push_back(endPoint);

	// Initialize if this is the first line
	if (m_PermanentVertices.size() == 2)
	{
		InitializePermanentVertexBuffer();
		return;
	}

	// Remove the first added line if it reaches the limit
	if (m_PermanentVertices.size() > m_MaxPermanentVertices)
	{
		m_PermanentVertices.erase(m_PermanentVertices.begin());
		m_PermanentVertices.erase(m_PermanentVertices.begin());
	}

	DeviceManager::m_pDeviceContext->UpdateSubresource(m_pPermanentVertexBuffer, 0, nullptr, m_PermanentVertices.data(), 0, 0);
}

void LineDebugging::Update()
{
	if (m_pVertexShader != nullptr)
		m_pVertexShader->SetViewProjectionMatrix(CameraManager::GetInstance()->GetActiveCamera()->GetInverseViewProjectionMatrix());
}

void LineDebugging::Render(Renderer& renderer)
{
	if (m_Vertices.empty() && m_PermanentVertices.empty())
		return;

	// Binding
	renderer.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	renderer.SetVertexShader(m_pVertexShader);
	renderer.SetPixelShader(m_pPixelShader);

	uint32_t strides{ sizeof(XMFLOAT3) };
	uint32_t offset{ 0 };

	// Drawing lines
	if (!m_Vertices.empty())
	{
		renderer.SetVertexBuffer(&m_pVertexBuffer, 1, &strides, &offset);
		renderer.Draw(unsigned int(m_Vertices.size()));

		// Clearing the lines
		m_Vertices.clear();

		D3D11_MAPPED_SUBRESOURCE mappedVertexBuffer;
		HRESULT result = DeviceManager::m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertexBuffer);
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to update the line debugging vertex buffer!");
			return;
		}

		CopyMemory(mappedVertexBuffer.pData, m_Vertices.data(), m_Vertices.size() * sizeof(XMFLOAT3));
		DeviceManager::m_pDeviceContext->Unmap(m_pVertexBuffer, 0);
	}

	// Drawing permanent lines
	if (!m_PermanentVertices.empty())
	{
		renderer.SetVertexBuffer(&m_pPermanentVertexBuffer, 1, &strides, &offset);
		renderer.Draw(unsigned int(m_PermanentVertices.size()));
	}
}