// SphereGouraudDlg.cpp : implementation file
//
#pragma execution_character_set("utf-8")

#include "pch.h"
#include "framework.h"
#include "SphereGouraud.h"
#include "SphereGouraudDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// =============================
// 常量配置
// =============================
const double PI = 3.14159265358979323846;
const int    SPHERE_SEGMENTS_LAT = 40;   // 适当增加细分让边缘更圆滑
const int    SPHERE_SEGMENTS_LON = 60;
const double SPHERE_RADIUS = 200.0;

// =============================
// CSphereGouraudDlg
// =============================

CSphereGouraudDlg::CSphereGouraudDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SPHEREGOURAUD_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // 初始变量
    m_zBufferWidth = 0;
    m_zBufferHeight = 0;
    m_rotationX = 0.0;
    m_rotationY = 0.0;
    m_rotationZ = 0.0;
    m_isAnimating = false;
    m_timerID = 0;

    // 光源位置（视线前方右上）
    m_lightPos = Vector3(300.0, 400.0, 200.0);

    // 材质参数
    m_ambientColor = Vector3(0.3, 0.3, 0.3);
    m_diffuseColor = Vector3(0.9, 0.9, 0.9);
    m_specularColor = Vector3(1.0, 1.0, 1.0);
    m_shininess = 32.0;

    // 光源颜色
    m_lightAmbient = Vector3(0.4, 0.4, 0.4);
    m_lightDiffuse = Vector3(1.0, 1.0, 1.0);
    m_lightSpecular = Vector3(1.0, 1.0, 1.0);
}

void CSphereGouraudDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSphereGouraudDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_ANIMATE, &CSphereGouraudDlg::OnBnClickedAnimate)
    ON_WM_KEYDOWN()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// =============================
// 初始化
// =============================

BOOL CSphereGouraudDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    CreateSphereMesh();
    return TRUE;
}

// =============================
// 基本旋转
// =============================

static void RotateX(Vector3& v, double angle) {
    double c = cos(angle), s = sin(angle);
    double y = v.y * c - v.z * s;
    double z = v.y * s + v.z * c;
    v.y = y; v.z = z;
}

static void RotateY(Vector3& v, double angle) {
    double c = cos(angle), s = sin(angle);
    double x = v.x * c + v.z * s;
    double z = -v.x * s + v.z * c;
    v.x = x; v.z = z;
}

static void RotateZ(Vector3& v, double angle) {
    double c = cos(angle), s = sin(angle);
    double x = v.x * c - v.y * s;
    double y = v.x * s + v.y * c;
    v.x = x; v.y = y;
}

// =============================
// 构建球体网格
// =============================

void CSphereGouraudDlg::CreateSphereMesh() {
    m_vertices.clear();
    m_triangles.clear();

    // 顶点
    for (int lat = 0; lat <= SPHERE_SEGMENTS_LAT; ++lat) {
        double theta = PI * lat / SPHERE_SEGMENTS_LAT;
        double sinTheta = sin(theta);
        double cosTheta = cos(theta);

        for (int lon = 0; lon <= SPHERE_SEGMENTS_LON; ++lon) {
            double phi = 2.0 * PI * lon / SPHERE_SEGMENTS_LON;
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);

            Vertex v;
            v.position.x = SPHERE_RADIUS * sinTheta * cosPhi;
            v.position.y = SPHERE_RADIUS * cosTheta;
            v.position.z = SPHERE_RADIUS * sinTheta * sinPhi;

            v.normal = v.position.normalize();
            m_vertices.push_back(v);
        }
    }

    // 三角形
    for (int lat = 0; lat < SPHERE_SEGMENTS_LAT; ++lat) {
        for (int lon = 0; lon < SPHERE_SEGMENTS_LON; ++lon) {
            int first = lat * (SPHERE_SEGMENTS_LON + 1) + lon;
            int second = first + SPHERE_SEGMENTS_LON + 1;

            Triangle t1{ first, second, first + 1 };
            Triangle t2{ second, second + 1, first + 1 };
            m_triangles.push_back(t1);
            m_triangles.push_back(t2);
        }
    }
}

// =============================
// 顶点光照（Gouraud）
// =============================

COLORREF CSphereGouraudDlg::CalculateVertexColor(const Vertex& vertex) {
    Vector3 lightDir = (m_lightPos - vertex.transformedPos).normalize();
    Vector3 viewDir = Vector3(0, 0, 1);

    // 环境光
    Vector3 ambient(
        m_ambientColor.x * m_lightAmbient.x,
        m_ambientColor.y * m_lightAmbient.y,
        m_ambientColor.z * m_lightAmbient.z
    );

    // 漫反射
    double diffFactor = std::max(0.0, vertex.transformedNormal.dot(lightDir));
    Vector3 diffuse(
        m_diffuseColor.x * m_lightDiffuse.x * diffFactor,
        m_diffuseColor.y * m_lightDiffuse.y * diffFactor,
        m_diffuseColor.z * m_lightDiffuse.z * diffFactor
    );

    // 高光（Blinn-Phong）
    Vector3 halfDir = (lightDir + viewDir).normalize();
    double specFactor = pow(std::max(0.0, vertex.transformedNormal.dot(halfDir)), m_shininess);
    Vector3 specular(
        m_specularColor.x * m_lightSpecular.x * specFactor,
        m_specularColor.y * m_lightSpecular.y * specFactor,
        m_specularColor.z * m_lightSpecular.z * specFactor
    );

    Vector3 color = ambient + diffuse + specular;

    // 防止太黑
    color.x = std::max(color.x, 0.12);
    color.y = std::max(color.y, 0.12);
    color.z = std::max(color.z, 0.12);

    int r = std::min(255, std::max(0, (int)(color.x * 255)));
    int g = std::min(255, std::max(0, (int)(color.y * 255)));
    int b = std::min(255, std::max(0, (int)(color.z * 255)));

    return RGB(r, g, b);
}

// =============================
// 顶点变换
// =============================

void CSphereGouraudDlg::TransformVertices(int width, int height) {
    int cx = width / 2;
    int cy = height / 2;

    for (auto& v : m_vertices) {
        v.transformedPos = v.position;
        v.transformedNormal = v.normal;

        // 旋转位置
        RotateX(v.transformedPos, m_rotationX);
        RotateY(v.transformedPos, m_rotationY);
        RotateZ(v.transformedPos, m_rotationZ);

        // 旋转法线
        RotateX(v.transformedNormal, m_rotationX);
        RotateY(v.transformedNormal, m_rotationY);
        RotateZ(v.transformedNormal, m_rotationZ);
        double len = v.transformedNormal.length();
        if (len > 1e-6)
            v.transformedNormal = v.transformedNormal.normalize();
        else
            v.transformedNormal = v.normal;

        v.color = CalculateVertexColor(v);

        v.screenX = cx + (int)std::round(v.transformedPos.x);
        v.screenY = cy - (int)std::round(v.transformedPos.y);
        v.screenZ = -v.transformedPos.z;   // 越小越近
    }
}

// =============================
// 重心坐标
// =============================

static void GetBarycentricCoords(
    double px, double py,
    double x1, double y1,
    double x2, double y2,
    double x3, double y3,
    double& w1, double& w2, double& w3)
{
    double denom = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
    if (std::fabs(denom) < 1e-8) {
        w1 = w2 = w3 = -1.0;
        return;
    }

    w1 = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3)) / denom;
    w2 = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3)) / denom;
    w3 = 1.0 - w1 - w2;
}

// =============================
// 颜色插值
// =============================

static COLORREF InterpolateColor(
    COLORREF c1, COLORREF c2, COLORREF c3,
    double w1, double w2, double w3)
{
    int r = (int)(GetRValue(c1) * w1 + GetRValue(c2) * w2 + GetRValue(c3) * w3);
    int g = (int)(GetGValue(c1) * w1 + GetGValue(c2) * w2 + GetGValue(c3) * w3);
    int b = (int)(GetBValue(c1) * w1 + GetBValue(c2) * w2 + GetBValue(c3) * w3);

    r = std::min(255, std::max(0, r));
    g = std::min(255, std::max(0, g));
    b = std::min(255, std::max(0, b));
    return RGB(r, g, b);
}

// =============================
// 画单个三角形（Gouraud）
// =============================

void CSphereGouraudDlg::DrawTriangleGouraud(
    CDC* pDC, const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
    // 背面剔除（在物体坐标系中做，视线沿 +Z）
    Vector3 e1 = v1.transformedPos - v0.transformedPos;
    Vector3 e2 = v2.transformedPos - v0.transformedPos;
    Vector3 faceN = e1.cross(e2);
    if (faceN.z >= 0.0)    // 面法线朝向 +Z 视为背面（根据你的坐标系可以调整）
        return;

    // 屏幕空间包围盒
    int minX = std::min({ v0.screenX, v1.screenX, v2.screenX });
    int maxX = std::max({ v0.screenX, v1.screenX, v2.screenX });
    int minY = std::min({ v0.screenY, v1.screenY, v2.screenY });
    int maxY = std::max({ v0.screenY, v1.screenY, v2.screenY });

    minX = std::max(0, minX);
    maxX = std::min(m_zBufferWidth - 1, maxX);
    minY = std::max(0, minY);
    maxY = std::min(m_zBufferHeight - 1, maxY);
    if (minX > maxX || minY > maxY) return;

    const double DEPTH_BIAS = 1e-4;

    // 光栅化
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            double w0, w1, w2;
            GetBarycentricCoords(
                x + 0.5, y + 0.5,
                v0.screenX, v0.screenY,
                v1.screenX, v1.screenY,
                v2.screenX, v2.screenY,
                w0, w1, w2);

            // 严格三角形内部测试，避免外部像素误判
            if (w0 < 0.0 || w1 < 0.0 || w2 < 0.0)
                continue;

            // 深度插值
            double z = v0.screenZ * w0 + v1.screenZ * w1 + v2.screenZ * w2;
            z -= DEPTH_BIAS;     // 防止边缘轻微 Z fighting

            int idx = y * m_zBufferWidth + x;
            if (idx < 0 || idx >= (int)m_zBuffer.size()) continue;

            if (z < m_zBuffer[idx]) {
                m_zBuffer[idx] = z;
                COLORREF col = InterpolateColor(v0.color, v1.color, v2.color, w0, w1, w2);
                pDC->SetPixel(x, y, col);
            }
        }
    }
}

// =============================
// 渲染整个场景
// =============================

void CSphereGouraudDlg::RenderScene(CDC* pDC, int width, int height) {
    if (width <= 0 || height <= 0) return;

    if (m_zBufferWidth != width || m_zBufferHeight != height) {
        m_zBufferWidth = width;
        m_zBufferHeight = height;
        m_zBuffer.assign(width * height, 1e9);   // 远平面
    }
    else {
        std::fill(m_zBuffer.begin(), m_zBuffer.end(), 1e9);
    }

    TransformVertices(width, height);

    for (const auto& tri : m_triangles) {
        const Vertex& v0 = m_vertices[tri.v0];
        const Vertex& v1 = m_vertices[tri.v1];
        const Vertex& v2 = m_vertices[tri.v2];
        DrawTriangleGouraud(pDC, v0, v1, v2);
    }
}

// =============================
// 绘制
// =============================

void CSphereGouraudDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this);
        CRect rect;
        GetClientRect(&rect);

        int buttonHeight = 40;
        int renderHeight = rect.Height() - buttonHeight;

        CDC memDC;
        memDC.CreateCompatibleDC(&dc);
        CBitmap bmp;
        bmp.CreateCompatibleBitmap(&dc, rect.Width(), renderHeight);
        CBitmap* oldBmp = memDC.SelectObject(&bmp);

        memDC.FillSolidRect(0, 0, rect.Width(), renderHeight, RGB(128, 0, 0));

        RenderScene(&memDC, rect.Width(), renderHeight);

        dc.BitBlt(0, buttonHeight, rect.Width(), renderHeight, &memDC, 0, 0, SRCCOPY);

        memDC.SelectObject(oldBmp);
        CDialogEx::OnPaint();
    }
}

HCURSOR CSphereGouraudDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// =============================
// 动画 / 交互
// =============================

void CSphereGouraudDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1 && m_isAnimating) {
        UpdateRotation();
        Invalidate(FALSE);
    }
    CDialogEx::OnTimer(nIDEvent);
}

void CSphereGouraudDlg::UpdateRotation()
{
    m_rotationY += 0.02;
    if (m_rotationY > 2.0 * PI) m_rotationY -= 2.0 * PI;
}

void CSphereGouraudDlg::OnBnClickedAnimate()
{
    m_isAnimating = !m_isAnimating;
    if (m_isAnimating) {
        m_timerID = SetTimer(1, 16, NULL);
        GetDlgItem(IDC_ANIMATE)->SetWindowText(L"停止");
    }
    else {
        if (m_timerID != 0) {
            KillTimer(m_timerID);
            m_timerID = 0;
        }
        GetDlgItem(IDC_ANIMATE)->SetWindowText(L"动画");
    }
}

void CSphereGouraudDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar) {
    case VK_LEFT:
        m_rotationY -= 0.1;
        break;
    case VK_RIGHT:
        m_rotationY += 0.1;
        break;
    case VK_UP:
        m_rotationX -= 0.1;
        break;
    case VK_DOWN:
        m_rotationX += 0.1;
        break;
    default:
        break;
    }
    Invalidate(FALSE);
    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CSphereGouraudDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        OnKeyDown((UINT)pMsg->wParam,
            (UINT)LOWORD(pMsg->lParam),
            (UINT)HIWORD(pMsg->lParam));
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CSphereGouraudDlg::OnDestroy()
{
    CDialogEx::OnDestroy();
    if (m_timerID != 0) {
        KillTimer(m_timerID);
        m_timerID = 0;
    }
}