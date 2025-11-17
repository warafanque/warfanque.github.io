# 快速参考卡 - Sphere Gouraud 光照模型

## 快速开始（3步）

```
1. 打开 Visual Studio 2022
2. 文件 > 打开 > 项目/解决方案 > SphereGouraud.sln
3. 按 F5 运行
```

## 控制键

| 按键 | 功能 |
|------|------|
| ← | 向左旋转 (Y轴) |
| → | 向右旋转 (Y轴) |
| ↑ | 向上旋转 (X轴) |
| ↓ | 向下旋转 (X轴) |
| 动画按钮 | 开始/停止自动旋转 |
| ESC | 关闭程序 |

## 技术参数

### 球体参数
- **半径**: 200像素
- **三角形**: ~2400个
- **顶点**: 1271个
- **细分**: 30纬度 × 40经度

### 光照参数
- **光源位置**: (300, 400, 500) 右上方
- **环境光**: 30%
- **漫反射**: 橙黄色 (0.8, 0.6, 0.4)
- **镜面反射**: 白色高光
- **光泽度**: 32

### 渲染参数
- **背景色**: RGB(128, 0, 0) 深红色
- **帧率**: ~60 FPS
- **消隐**: Z-Buffer算法
- **着色**: Gouraud顶点插值

## 常用修改

### 修改球体大小
`SphereGouraudDlg.cpp` 第15行:
```cpp
const double SPHERE_RADIUS = 200.0;  // 改为你想要的值
```

### 修改细分精度
`SphereGouraudDlg.cpp` 第16-17行:
```cpp
const int SPHERE_SEGMENTS_LAT = 30;  // 纬度
const int SPHERE_SEGMENTS_LON = 40;  // 经度
```

### 修改背景颜色
`SphereGouraudDlg.cpp` 第374行:
```cpp
memDC.FillSolidRect(0, 0, rect.Width(), renderHeight, RGB(128, 0, 0));
```

### 修改光源位置
`SphereGouraudDlg.cpp` 第35行:
```cpp
m_lightPos = Vector3(300.0, 400.0, 500.0);
```

## 文件位置

| 文件 | 说明 |
|------|------|
| `SphereGouraudDlg.cpp` | 主渲染代码 |
| `SphereGouraud.rc` | 界面资源 |
| `README.md` | 详细说明 |
| `USAGE.md` | 使用指南 |
| `TROUBLESHOOTING.md` | 问题排查 |

## 核心函数

| 函数 | 功能 |
|------|------|
| `CreateSphereMesh()` | 生成球体网格 |
| `TransformVertices()` | 变换和投影 |
| `CalculateVertexColor()` | 计算光照 |
| `DrawTriangleGouraud()` | 光栅化三角形 |
| `RenderScene()` | 渲染整个场景 |

## 常见问题

**Q: 找不到MFC?**  
A: 在VS Installer中安装"使用C++的桌面开发"

**Q: 球体太慢?**  
A: 使用Release配置或减少SPHERE_SEGMENTS值

**Q: 键盘不响应?**  
A: 点击窗口使其获得焦点

## VS2022快捷键

| 快捷键 | 功能 |
|--------|------|
| F5 | 调试运行 |
| Ctrl+F5 | 直接运行 |
| F7 | 生成项目 |
| Ctrl+Shift+B | 生成解决方案 |
| F9 | 设置断点 |
| F10 | 单步跳过 |
| F11 | 单步进入 |

## 性能提示

✓ 使用 Release 配置以提高速度  
✓ 减少分段数可提高帧率  
✓ 增加分段数可提高质量  
✓ x64平台比x86快

## 扩展建议

□ 添加鼠标拖拽旋转  
□ 添加滚轮缩放  
□ 添加纹理贴图  
□ 实现Phong着色  
□ 添加多个光源  
□ 导出渲染图片

---
**提示**: 按F1查看完整文档，或阅读USAGE.md获取详细说明
