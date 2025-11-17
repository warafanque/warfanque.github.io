// SphereGouraudDlg.h : header file
//

#pragma once

#include <vector>
#include <cmath>

// Vector3 structure
struct Vector3 {
    double x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(double s) const { return Vector3(x * s, y * s, z * s); }
    
    double dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vector3 cross(const Vector3& v) const {
        return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    double length() const { return sqrt(x * x + y * y + z * z); }
    
    Vector3 normalize() const {
        double len = length();
        if (len > 0.0001) return Vector3(x / len, y / len, z / len);
        return *this;
    }
};

// Vertex structure
struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector3 transformedPos;
    Vector3 transformedNormal;
    COLORREF color;
    int screenX, screenY;
    double screenZ;
};

// Triangle structure
struct Triangle {
    int v0, v1, v2;
};

// CSphereGouraudDlg dialog
class CSphereGouraudDlg : public CDialogEx
{
// Construction
public:
	CSphereGouraudDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPHEREGOURAUD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    // Sphere data
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<double> m_zBuffer;
    int m_zBufferWidth;
    int m_zBufferHeight;
    
    // Rotation angles
    double m_rotationX;
    double m_rotationY;
    double m_rotationZ;
    
    // Animation
    bool m_isAnimating;
    UINT_PTR m_timerID;
    
    // Light source
    Vector3 m_lightPos;
    
    // Material properties
    Vector3 m_ambientColor;
    Vector3 m_diffuseColor;
    Vector3 m_specularColor;
    double m_shininess;
    
    // Light properties
    Vector3 m_lightAmbient;
    Vector3 m_lightDiffuse;
    Vector3 m_lightSpecular;
    
    // Methods
    void CreateSphereMesh();
    void TransformVertices(int width, int height);
    COLORREF CalculateVertexColor(const Vertex& vertex);
    void RenderScene(CDC* pDC, int width, int height);
    void DrawTriangleGouraud(CDC* pDC, const Vertex& v0, const Vertex& v1, const Vertex& v2);
    void UpdateRotation();
    
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedAnimate();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnDestroy();
};
