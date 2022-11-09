// UV sphere model generator
// 09 Nov 2022

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <string>
#include "dtoa.h"

const unsigned MAX_VERT_LEN = 3 * MAX_DTOA_STRLEN + 11;

#undef PI
#undef R_PI
#undef TWO_PI
#undef R_TWO_PI

const double PI(3.1415926535897932384626433832795);
const double R_PI(0.31830988618379067153776752674503);
const double TWO_PI(6.283185307179586476925286766559);
const double R_TWO_PI(0.159154943091895335768883763372515);

std::ofstream logfile;

int dwpoint(char* cstr, int len) // don't write a point before 'f'
{
	for (int i = 0; i < len; ++i) {
		if (cstr[i] == '.' || cstr[i] == 'e')
			return 1;
	}
	return 0;
}

int swrite(double dv, char* dest) {
	int pos = dtoa(dest, dv);
	if (!dwpoint(dest, pos)) {
		dest[pos++] = '.';
	}
	strcpy(dest + pos, "f, ");
	return pos + 3;
}

unsigned min_u(unsigned n, unsigned m) {
	return n < m ? n : m;
}

int main()
{
	using std::cout;
	using std::cin;
	
	cout << "Sphere petals [3...65535]: ";
	unsigned short q;
	cin >> q;
	if (q < 3U) return -1;

	cout << "Radius [0...1e+14]: ";
	double r;
	cin >> r;
	if (r < 0.0 || r > 1.0e14) return -1;

	cout << "Filename (up to 31 symbol): ";
	char* filename = new char[32];
	cin >> filename;

	logfile.open(filename, std::fstream::trunc);
	for (int i = 0; i < strlen(filename); ++i)
	{
		if (filename[i] == '.') {
			filename[i] = '\0';
			break;
		}
		else if (i
		&&		!(filename[i] >= 'a' && filename[i] <= 'z')
		&&		!(filename[i] >= 'A' && filename[i] <= 'Z')
		&&		!(filename[i] >= '0' && filename[i] <= '9')
		) {
			filename[i] = '0';
		}
	}
	
	if (filename[0]
	&&	!(filename[0] >= 'a' && filename[0] <= 'z')
	&&	!(filename[0] >= 'A' && filename[0] <= 'Z')
	) {
		filename[0] = 's';
	}

	unsigned short p = q >> 1; // sphere circles
	if (q & 1) ++p;
	double n = TWO_PI / q; // radians per one sphere petal
	double h = PI / p;
	unsigned int verts = (q + 1) * (p + 1);
	
	logfile << "\nfloat " << filename << "Verts[" << verts * 5 << "U] = {";

	std::string fstream_buffer; // write buffer
	unsigned int maxBufferSize = 0;//min_u(fstream_buffer.max_size() - MAX_VERT_LEN, MAX_VERT_LEN * verts >> 4);

	for (unsigned pitch = 0; pitch <= p; ++pitch)
	{
		static bool end = false;
		for (unsigned roll = 0; roll <= q; ++roll)
		{
			double np, nrl, sinnp_r;
			np = h * pitch;
			nrl = n * roll;
			sinnp_r = sin(np) * r;

			char* output = new char[MAX_VERT_LEN]; // one output contains one vertice
			output[0] = '\n';
			output[1] = '\t';
			int pos = 2;

			pos += swrite(cos(nrl) * sinnp_r, output + pos); // x
			pos += swrite(cos(np) * r, output + pos); // y
			pos += swrite(sin(nrl) * sinnp_r, output + pos); // z

			pos += swrite(nrl * R_TWO_PI, output + pos); // image x
			pos += swrite(np * R_PI, output + pos); // image y

			if (pitch == p && roll == q) {
				output[pos - 2] = '\0'; // cut off ", " symbols
				end = true;
			}
			fstream_buffer.append(output);
			if (end) {
				logfile << fstream_buffer << "\n};\n\n";
				fstream_buffer.erase();
			} else if (fstream_buffer.size() >= maxBufferSize) {
				logfile << fstream_buffer;
				fstream_buffer.erase();
			}
		}
		if (!end)
			fstream_buffer.push_back('\n');
		
		float newp = (200.f * pitch) / q;
		static float opt_percent = 120000.f / verts;
		static float percent = -opt_percent;
		
		if (newp - percent >= opt_percent) {
			system("cls");
			cout << newp << "%\n";
			percent = newp;
		}
	}

	logfile << "unsigned int " << filename << "Indices[" << 6 * q * (p - 1) << "U] = {";

	for (unsigned int pitch = 0; pitch < p; ++pitch)
	{
		unsigned int pts[4];
		pts[0] = pitch * (q + 1);
		pts[1] = pts[0] + q + 1;
		for (unsigned int roll = 1; roll <= q; ++roll)
		{
			pts[2] = pts[0] + 1;
			pts[3] = pts[1] + 1;

			fstream_buffer.append("\n\t");
			char* num = new char[12];

			if (pitch < p - 1) {
				fstream_buffer.append(_ultoa(pts[2], num, 10));
				fstream_buffer.append("U, ");
				fstream_buffer.append(_ultoa(pts[1], num, 10));
				fstream_buffer.append("U, ");
				fstream_buffer.append(_ultoa(pts[3], num, 10));
				fstream_buffer.append("U, ");
			}
			
			if (pitch) {
				fstream_buffer.append(_ultoa(pts[0], num, 10));
				fstream_buffer.append("U, ");
				fstream_buffer.append(_ultoa(pts[1], num, 10));
				fstream_buffer.append("U, ");
				fstream_buffer.append(_ultoa(pts[2], num, 10));
				fstream_buffer.append("U, ");
			}

			if (roll == q && pitch == p-1) {
				fstream_buffer.replace(fstream_buffer.length()-2, fstream_buffer.length(), "");
				break;
			}
			
			if (fstream_buffer.size() >= maxBufferSize) {
				logfile << fstream_buffer;
				fstream_buffer.erase();
			}

			pts[0] = pts[2];
			pts[1] = pts[3];
		}
		fstream_buffer.push_back('\n');
		// logfile << fstream_buffer;
		// fstream_buffer.erase();
	}
	fstream_buffer.append("};");
	logfile << fstream_buffer;
	fstream_buffer.erase();

	system("cls");
	cout << (q << 1) * (p - 1) << " triangles\n";
	time_t lt;
	lt = time(nullptr);
	logfile << "\n\n// " << verts << " vertices. " << (q << 1) * (p - 1) << " triangles.\n// Generated at " << ctime(&lt);
	logfile.close();
	system("pause");
}
