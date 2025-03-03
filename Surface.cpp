#include "Surface.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment( lib,"gdiplus.lib" )

Surface::Surface(unsigned int width, unsigned int height) noexcept
	:
	pBuffer(std::make_unique<Color[]>(width* height)),
	width(width),
	height(height)
{}

Surface& Surface::operator=(Surface&& donor) noexcept
{
	width = donor.width;
	height = donor.height;
	pBuffer = std::move(donor.pBuffer);
	donor.pBuffer = nullptr;
	return *this;
}

Surface::Surface(Surface&& source) noexcept
	:
	pBuffer(std::move(source.pBuffer)),
	width(source.width),
	height(source.height)
{}

Surface::~Surface()
{}

void Surface::Clear(Color fillValue) noexcept
{
	memset(pBuffer.get(), fillValue.dword, width * height * sizeof(Color));
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color c)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);
	pBuffer[y * width + x] = c;
}

Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);
	return pBuffer[y * width + x];
}

unsigned int Surface::GetWidth() const noexcept
{
	return width;
}

unsigned int Surface::GetHeight() const noexcept
{
	return height;
}

Surface::Color* Surface::GetBufferPtr() noexcept
{
	return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPtr() const noexcept
{
	return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPtrConst() const noexcept
{
	return pBuffer.get();
}

Surface Surface::FromFile(const std::string& name)
{
	unsigned int width = 0;
	unsigned int height = 0;
	std::unique_ptr<Color[]> pBuffer;

	bool alphaLoaded = false;
	{
		// convert filenam to wide string (for Gdiplus)
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, name.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(wideName);

		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pBuffer[y * width + x] = c.GetValue();
				if (c.GetAlpha() != 255)
				{
					alphaLoaded = true;
				}
			}
		}
	}

	return Surface(width, height, std::move(pBuffer), alphaLoaded);
}

void Surface::Save(const std::string& filename) const
{
	auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void
		{
			UINT  num = 0;          // number of image encoders
			UINT  size = 0;         // size of the image encoder array in bytes

			Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

			Gdiplus::GetImageEncodersSize(&num, &size);

			pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

			GetImageEncoders(num, size, pImageCodecInfo);

			for (UINT j = 0; j < num; ++j)
			{
				if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
				{
					*pClsid = pImageCodecInfo[j].Clsid;
					free(pImageCodecInfo);
					return;
				}
			}

			free(pImageCodecInfo);
		};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);


	// convert filenam to wide string (for Gdiplus)
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer.get());
}

bool Surface::AlphaLoaded() const noexcept
{
	return alphaLoaded;
}

void Surface::Copy(const Surface& src)
{
	assert(width == src.width);
	assert(height == src.height);
	memcpy(pBuffer.get(), src.pBuffer.get(), width * height * sizeof(Color));
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam, bool alphaLoaded) noexcept
	:
	width(width),
	height(height),
	pBuffer(std::move(pBufferParam)),
	alphaLoaded(alphaLoaded)
{}