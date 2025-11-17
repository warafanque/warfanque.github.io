# 球体Gouraud光照模型实验

这是一个使用MFC (Microsoft Foundation Classes) 实现的3D球体渲染程序，具有Gouraud光照模型和Z-Buffer消隐算法。专为Visual Studio 2022设计。

## 📚 文档导航

- **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - 快速参考卡（推荐新手阅读）
- **[USAGE.md](USAGE.md)** - 详细使用指南
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - 故障排除指南
- **[PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)** - 项目完成总结

## ⚡ 快速开始

1. 打开 **Visual Studio 2022**
2. 选择 **文件 > 打开 > 项目/解决方案**
3. 打开 `SphereGouraud.sln`
4. 按 **F5** 运行

**控制方式**:
- ← → ↑ ↓ 方向键旋转球体
- 点击"动画"按钮开始/停止自动旋转

## 功能特性

1. **坐标系 (Coordinate System)**: 
   - 在屏幕客户区中心建立右手三维坐标系
   - X轴水平向右为正
   - Y轴垂直向上为正
   - Z轴垂直屏幕指向观察者为正

2. **地理划分 (Geographic Subdivision)**:
   - 采用经纬网格法细分球面
   - 构建三角形网格
   - 30个纬度分段，40个经度分段

3. **消隐 (Hidden Surface Removal)**:
   - 使用Z-Buffer算法进行消隐
   - 球体中心与坐标系原点重合

4. **光照 (Lighting)**:
   - 实现Gouraud着色（顶点着色插值）
   - 单点光源位于球体右上方
   - 支持环境光、漫反射光和镜面反射光

5. **背景色 (Background Color)**:
   - 固定为RGB(128, 0, 0) - 深红色

6. **交互 (Interaction)**:
   - 方向键旋转球体：
     - ← 左箭头：向左旋转
     - → 右箭头：向右旋转
     - ↑ 上箭头：向上旋转
     - ↓ 下箭头：向下旋转

7. **动画 (Animation)**:
   - 工具栏"动画"按钮控制
   - 点击播放/停止球体自动旋转

## 系统要求

- **Visual Studio 2022** (或更高版本)
- **MFC (Microsoft Foundation Classes)** - 需要在Visual Studio安装程序中选择"使用C++的桌面开发"工作负载
- **Windows 10 SDK**

## 构建说明

### 在Visual Studio 2022中打开和构建

1. 启动 **Visual Studio 2022**
2. 点击"打开项目或解决方案"
3. 浏览到项目目录，选择 `SphereGouraud.sln`
4. 在工具栏中选择配置（Debug 或 Release）和平台（x86 或 x64）
5. 按 **F5** 运行，或按 **Ctrl+Shift+B** 构建

### 注意事项

- 如果编译时提示"未安装MFC"，请在Visual Studio安装程序中添加"使用C++的桌面开发"工作负载
- 如果遇到中文资源文件问题，请确保Visual Studio的代码页设置正确

## 运行

构建成功后，可执行文件位于：
- Debug 版本: `Debug/SphereGouraud.exe` 或 `x64/Debug/SphereGouraud.exe`
- Release 版本: `Release/SphereGouraud.exe` 或 `x64/Release/SphereGouraud.exe`

直接运行可执行文件即可。

## 技术实现

### 球体网格生成
使用球坐标系（θ, φ）生成球面顶点：
- θ (theta): 纬度角，范围[0, π]
- φ (phi): 经度角，范围[0, 2π]
- 顶点坐标：
  - x = R * sin(θ) * cos(φ)
  - y = R * cos(θ)
  - z = R * sin(θ) * sin(φ)

### Gouraud着色
1. 在每个顶点计算光照颜色
2. 光照模型包含：
   - 环境光 (Ambient)
   - 漫反射 (Diffuse)
   - 镜面反射 (Specular, Blinn-Phong)
3. 三角形内部通过重心坐标插值顶点颜色

### Z-Buffer算法
1. 为每个像素维护深度值
2. 渲染时比较当前片段深度与缓冲区深度
3. 只绘制更靠近观察者的片段

### 背面剔除
通过叉积计算三角形法向量，只渲染面向观察者的三角形。

## 项目结构

```
.
├── SphereGouraud.sln          - Visual Studio 解决方案文件
├── SphereGouraud.vcxproj      - Visual Studio 项目文件
├── SphereGouraud.h            - 应用程序主头文件
├── SphereGouraud.cpp          - 应用程序主实现
├── SphereGouraudDlg.h         - 对话框头文件
├── SphereGouraudDlg.cpp       - 对话框实现（包含所有渲染逻辑）
├── SphereGouraud.rc           - 资源文件
├── resource.h                 - 资源ID定义
├── pch.h/pch.cpp              - 预编译头文件
├── framework.h                - MFC框架头文件
├── targetver.h                - Windows SDK版本定义
├── res/                       - 资源文件夹
│   ├── SphereGouraud.ico      - 应用程序图标
│   └── SphereGouraud.rc2      - 手动编辑的资源
└── README.md                  - 本文档
```

## 核心文件说明

- **SphereGouraudDlg.cpp**: 包含所有核心渲染逻辑
  - `CreateSphereMesh()`: 生成球体网格
  - `TransformVertices()`: 顶点变换
  - `CalculateVertexColor()`: Gouraud着色计算
  - `DrawTriangleGouraud()`: 三角形光栅化
  - `RenderScene()`: 场景渲染主函数

## 许可

本项目为教育实验项目。
