////////////////////////////////////////////////////////////////////////////////
// Filename: quadtreeclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _QUADTREECLASS_H_
#define _QUADTREECLASS_H_


/////////////
// GLOBALS //
/////////////
const int MAX_TRIANGLES = 10000;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "terrainclass.h"
#include "frustumclass.h"
#include "terrainshaderclass.h"
#include "ColorShaderClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: QuadTreeClass
////////////////////////////////////////////////////////////////////////////////
class QuadTreeClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
		D3DXVECTOR4 color;
	};

	struct VectorType 
	{
		float x, y, z;
	};

	struct NodeType
	{
        float positionX, positionY, positionZ, width, height;
		int triangleCount, m_lineIndexCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		ID3D11Buffer* m_lineIndexBuffer, *m_lineVertexBuffer;
		D3DXVECTOR3* vertexArray;

        NodeType* nodes[4];
	};

	struct ColorVertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
public:
	QuadTreeClass();
	QuadTreeClass(const QuadTreeClass&);
	~QuadTreeClass();

	bool Initialize(TerrainClass*, ID3D11Device*);
	void Shutdown();
	void Render(FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*, ColorShaderClass*,bool);

	int GetDrawCount();
	bool GetHeightAtPosition(float, float, float&);

private:
	void CalculateMeshDimensions(int, float&, float&, float&, float&, float&);
	void CreateTreeNode(NodeType*, float, float, float, float, float, ID3D11Device*);
	int CountTriangles(float, float, float, float, float);
	bool IsTriangleContained(int, float, float, float, float, float);
	void ReleaseNode(NodeType*);
	void RenderNode(NodeType*, FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);

	bool CreateLineBuffers(NodeType*, ID3D11Device*);
	void RenderLineBuffers(NodeType*, ID3D11DeviceContext*, ColorShaderClass*);

	void FindNode(NodeType*, float, float, float&);
	bool CheckHeightOfTriangle(float, float, float&, float[3], float[3], float[3]);

	//void ReleaseLineBuffers(NodeType*);
private:
	int m_triangleCount, m_drawCount;
	VertexType* m_vertexList;
	NodeType* m_parentNode;
	
};

#endif