#pragma once
#include <tiff/tiffio.h>
#include <vector>

namespace TEngine {
	class Mesh;
	class MyTif {
	private:
		char* path = nullptr;
		TIFF* tif = nullptr;
		unsigned short* raster = nullptr; //����֮�£��������ң�ȡÿһ����
	public:
		int width = -1;
		int height = -1;
	public:
		MyTif(const char* path_);
		//��ȡ���ݡ����У�����֮�£���������
		const unsigned short* GetRasterData();
		void ClearRasterData();
		int GetBitsPerSample();
		~MyTif();
	};
}