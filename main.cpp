#include <algorithm>
using namespace std;

class Solver
{
	float* h;
	int DX, DY;

public:
	Solver(float* _h, int _DX, int _DY) : h(_h), DX(_DX), DY(_DY)
	{

	}

	void IntegrateHorizontal(float* gx, int iters)
	{
		float* tmp = new float[DX * DY];
		const float dt = 0.485;

		//1d horizontal alternating forward/backward scheme
		for (int k = 0; k < iters; k++)
		{
			for (int y = 0; y < DY; y++)
				for (int x = 1; x < DX; x++) {
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x - 1 + y * DX] + gx[x + y * DX]) * dt;
				}
			std::copy(tmp, tmp + DX * DY, h);

			for (int y = 0; y < DY; y++)
				for (int x = DX - 2; x >= 0; x--) {
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x + 1 + y * DX] - gx[x + y * DX]) * dt;
				}
			std::copy(tmp, tmp + DX * DY, h);
		}
		delete[]tmp;
	}

	void IntegrateVertical(float* gy, int iters)
	{
		float* tmp = new float[DX * DY];
		const float dt = 0.485;

		//1d vertical alternating forward/backward scheme
		for (int k = 0; k < iters; k++)
		{
			for (int y = 1; y < DY; y++)
				for (int x = 0; x < DX; x++)
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x + (y - 1) * DX] + gy[x + y * DX]) * dt;

			std::copy(tmp, tmp + DX * DY, h);

			for (int y = DY - 2; y >= 0; y--)
				for (int x = 0; x < DX; x++)
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x + (y + 1) * DX] - gy[x + y * DX]) * dt;

			std::copy(tmp, tmp + DX * DY, h);
		}
		delete[]tmp;
	}

	void Integrate2D(float* gx, float* gy, int iters)
	{
		float* tmp = new float[DX * DY];
		const float dt = 0.485;

		//2d alternating forward/backward scheme
		for (int k = 0; k < iters; k++)
		{
			//horizontal scheme
			for (int y = 0; y < DY; y++)
				for (int x = 1; x < DX; x++)
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x - 1 + y * DX] + gx[x + y * DX]) * dt; //fwd h

			std::copy(tmp, tmp + DX * DY, h);
			for (int y = 0; y < DY; y++)
				for (int x = DX - 2; x >= 0; x--)
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x + 1 + y * DX] - gx[x + y * DX]) * dt; //bwd h

			std::copy(tmp, tmp + DX * DY, h);

			//vertical scheme
			for (int y = 1; y < DY; y++)
				for (int x = 0; x < DX; x++)
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x + (y - 1) * DX] + gy[x + y * DX]) * dt; //fwd v

			std::copy(tmp, tmp + DX * DY, h);
			for (int y = DY - 2; y >= 0; y--)
				for (int x = 0; x < DX; x++)
					tmp[x + y * DX] = h[x + y * DX] - (h[x + y * DX] - h[x + (y + 1) * DX] - gy[x + y * DX]) * dt; //bwd v

			std::copy(tmp, tmp + DX * DY, h);
		}
		delete[]tmp;
	}

};


float dist(float x, float y, float cx, float cy, float pw = 1.6)
{
	float px = x - cx, py = y - cy;
	float d = (px * px + py * py);
	return 0.5f / (1 + powf(d * 18, pw));
}

float random()
{
	return (float)(rand() % 10000) / 5000.0f - 1;
}

void makeTestHeightmap(float* h, int DX, int DY)
{
	for (int y = 0; y < DY; y++)
		for (int x = 0; x < DX; x++)
		{
			float d = dist((float)x / DX, (float)y / DY, 0.5f, 0.5f);

			//compact and not efficient way to make random shapes, used only for demonstration!
			srand(0);
			for (int i = 0; i < 30; i++) d += dist((float)x / DX, (float)y / DY, 0.5 + random() * 0.5, 0.5 + random() * 0.5, 1) * random() * 0.35;
			h[x + y * DX] = d * 200;
		}
}

int main()
{
	int DX = 300, DY = 300;
	float* hmap = new float[DX * DY];

	makeTestHeightmap(hmap, DX, DY);

	float* gx = new float[DX * DY]();
	float* gy = new float[DX * DY]();

	float qf = 2;//quantization factor
	for (int y = 1; y < DY - 1; y++)
		for (int x = 1; x < DX - 1; x++) {
			float dx = hmap[x + 1 + y * DX] - hmap[x - 1 + y * DX];
			float dy = hmap[x + (y + 1) * DX] - hmap[x + (y - 1) * DX];
			gx[x + y * DX] = roundf(dx / qf) * qf;
			gy[x + y * DX] = roundf(dy / qf) * qf;
		}

	Solver solver(hmap, DX, DY);
	//solver.IntegrateHorizontal(gx, 24);
	//solver.IntegrateVertical(gy, 24);
	solver.Integrate2D(gx, gy, 240);

	delete[]gx;
	delete[]gy;
	delete[]hmap;
	return 0;
}
