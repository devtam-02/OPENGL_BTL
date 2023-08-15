#pragma region 
#include "Angel.h"
#include <cstdlib>
typedef vec4 point4;
typedef vec4 color4;
using namespace std;
#pragma endregion Khai báo thư viện và định nghĩa kiểu dữ liệu

#pragma region 
GLuint program;
GLuint model_loc;
GLuint projection_loc;
GLuint view_loc;
#pragma endregion Các biến kết hợp vshader dùng để chạy chương trình, NO LOGIC

#pragma region
mat4 wolrd_matrix;
mat4 projection;
mat4 view;
#pragma endregion Các biến ma trận model, projection, view, symbol

#pragma region
// Số các đỉnh của các tam giác trong hình lập phương
const int soDinhLapPhuong = 36;
// Số các đỉnh của các tam giác trong hình trụ (độ chính xác 8 mặt)
// Mỗi mặt ngang là 1 hcn = 2 tam giác => 6 đỉnh, tổng 8 mặt = 6 * 8
// 2 đáy, mỗi đáy 8 tam giác = 3 * 8 * 2 đáy
// 6 * 8 + 3 * 8 * 2 = 96
const int soDinhHinhTru = 96;
// 4 * 6 * 8 = 192
const int soDinhHinhTron = 192;

// Danh sách các đỉnh của các tam giác cần vẽ
point4 arrDinhTamGiac[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];
// Danh sách các màu tương ứng cho các đỉnh trên
color4 arrMauCacDinh[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];
//Danh sách các vector pháp tuyến ứng với mỗi đỉnh
vec3 arrVecPhapTuyen[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];


// Danh sách 8 đỉnh của hình lập phương
point4 cacDinhLapPhuong[8];
// Danh sách các màu cho 8 đỉnh hình lập phương
color4 cacMauHinhLapPhuong[8];

// Danh sách các đỉnh của hình trụ
// 8 cạnh bên, mỗi cạnh 2 đỉnh, 2 tâm tại 2 đáy
// 2 * 8 + 2 = 18
point4 cacDinhHinhTru[18];
// Danh sách các màu cho 10 mặt hình trụ
color4 cacMauHinhTru[10];

// Hình tròn
point4 cacDinhHinhTron[5][8];
color4 cacMauHinhTron[10];
#pragma endregion Các biến tính toán trong bộ nhớ

#pragma region
// Vị trí ánh sáng
point4 viTriAnhSang(0.0, 10.0, 10.0, 1.0);
// Màu gốc ánh sáng
color4 mauAnhSang(1, 1, 1, 1.0);
// Màu khuếch tán
color4 mauAnhSangKhuechTan(1.0, 1.0, 1.0, 1.0);
// Màu gương phản xạ lại
color4 mauAnhSangPhanXaGuong(1.0, 1.0, 1.0, 1.0);

// Màu gốc vật liệu
color4 mauVatLieu;
// Màu khuếch tán vật liệu
color4 mauVatLieuKhuechTan;
// Màu phản xạ gương vật liệu
color4 mauVatLieuPhanXaGuong;
// Độ bóng vật liệu
float doBongVatLieu = 1000.0;

// Màu được trộn từ 2 màu gốc
color4 mauGocTron;
// Màu khuếch tán được trộn từ 2 màu gốc
color4 mauKhuechTanTron;
// Màu phản xạ gương được trộn từ 2 màu gốc
color4 mauPhanXaGuongTron;
#pragma  endregion Các biến ánh sáng, vật liệu

#pragma region 
GLfloat cam_Eye[3] = { 0, 5, 6 };
float cam_Rotation[3] = { 0,0,90 };
GLfloat dr = 5;
vec4 eye(0, 5, -5, 1);
vec4 at(0, 0, 0, 1);
vec4 up(0, 1, 0, 1);

bool che_do_chi_tiet = false;
#pragma endregion Các biến hành động

#pragma region 
// Gán toạ độ điểm và giá trị màu cho HLP
void TinhDiemVaMauLapPhuong()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	cacDinhLapPhuong[0] = point4(-0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[1] = point4(-0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[2] = point4(0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[3] = point4(0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[4] = point4(-0.5, -0.5, -0.5, 1.0);
	cacDinhLapPhuong[5] = point4(-0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[6] = point4(0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	cacMauHinhLapPhuong[0] = color4(0.0, 0.0, 0.0, 1.0); // đen
	cacMauHinhLapPhuong[1] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhLapPhuong[2] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhLapPhuong[3] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhLapPhuong[4] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhLapPhuong[5] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhLapPhuong[6] = color4(1.0, 0.5, 0.0, 1.0); // cam
	cacMauHinhLapPhuong[7] = color4(0.0, 1.0, 1.0, 1.0); // xanh lơ
}
// Gán toạ độ điểm và giá trị màu cho hình trụ
void TinhDiemVaMauHinhTru()
{
	float tren = 0.5;
	float duoi = -tren;
	// Tâm trên
	cacDinhHinhTru[0] = point4(0, tren, 0, 1);

	// For để gán cho các đỉnh cạnh bên
	for (int i = 1; i <= 8; i++)
	{
		float banKinh = 0.5;
		float rad = (i - 1) * 45.0 / 360 * 2 * M_PI;
		// Tính x va z
		float x = banKinh * cosf(rad);
		float z = banKinh * sinf(rad);
		cacDinhHinhTru[i * 2 - 1] = point4(x, tren, z, 1);
		cacDinhHinhTru[i * 2] = point4(x, duoi, z, 1);
	}

	// Tâm dưới
	cacDinhHinhTru[17] = point4(0, duoi, 0, 1);

	cacMauHinhTru[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTru[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTru[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTru[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTru[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhTru[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTru[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTru[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTru[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTru[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}
// Gán toạ độ điểm và giá trị màu cho hình tròn
void TinhDiemVaMauHinhTron()
{
	float x, y, z;
	const float r = 0.5;
	const float cv45torad = 45.0 / 360 * 2 * M_PI;
	for (int i = 0; i <= 4; i++)
	{
		y = -r * cosf(i * cv45torad);
		float bk = r * sinf(i * cv45torad);
		//Tính y
		for (int j = 0; j <= 7; j++)
		{
			// Tính x,z
			x = bk * cosf(j * cv45torad);
			z = -bk * sinf(j * cv45torad);
			cacDinhHinhTron[i][j] = point4(x, y, z, 1);
		}
	}

	cacMauHinhTron[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTron[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTron[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTron[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTron[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhTron[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTron[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTron[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTron[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTron[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}

// Start HLP = 0
// Start Hình trụ = 0 + 36
// Start Hình tròn = 36 + 96 = 132
int Index = 0;
// Tạo một mặt hình lập phương (đưa đỉnh, pháp tuyến, màu vào mảng thứ tự cho việc chuyển đến bộ nhớ, )
void TaoMotMatHLP(int a, int b, int c, int d)
{
	vec4 u = cacDinhLapPhuong[b] - cacDinhLapPhuong[a];
	vec4 v = cacDinhLapPhuong[c] - cacDinhLapPhuong[b];
	vec3 normal = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[b]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[d]; Index++;
}
// Dùng hàm bên trên để đưa vào mảng
void TaoHinhVuong()  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	TaoMotMatHLP(1, 0, 3, 2);
	TaoMotMatHLP(2, 3, 7, 6);
	TaoMotMatHLP(3, 0, 4, 7);
	TaoMotMatHLP(6, 5, 1, 2);
	TaoMotMatHLP(4, 5, 6, 7);
	TaoMotMatHLP(5, 4, 0, 1);
}


// Hình trụ
void TaoMatBenHTru(int a, int b, int c, int d, int mau)
{
	vec4 u = cacDinhHinhTru[b] - cacDinhHinhTru[a];
	vec4 v = cacDinhHinhTru[c] - cacDinhHinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	// a -> b -> c -> a -> c -> d
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[b]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[d]; Index++;
}

void TaoTamGiacDay(int a, int b, int c, int mau)
{
	vec4 u = cacDinhHinhTru[b] - cacDinhHinhTru[a];
	vec4 v = cacDinhHinhTru[c] - cacDinhHinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[b]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
}

void TaoHTru()
{
	// Mỗi lệnh 6 điểm, 8 lệnh 48 điểm
	TaoMatBenHTru(2, 1, 3, 4, 0);
	TaoMatBenHTru(4, 3, 5, 6, 1);
	TaoMatBenHTru(6, 5, 7, 8, 2);
	TaoMatBenHTru(8, 7, 9, 10, 3);
	TaoMatBenHTru(10, 9, 11, 12, 4);
	TaoMatBenHTru(12, 11, 13, 14, 5);
	TaoMatBenHTru(14, 13, 15, 16, 6);
	TaoMatBenHTru(16, 15, 1, 2, 7);
	// Kết thúc tại điểm 2

	// Mỗi lệnh 3 điểm, 8 lệnh 24 điểm
	// Bắt đầu tại điểm 2
	TaoTamGiacDay(2, 17, 16, 9);
	TaoTamGiacDay(16, 17, 14, 9);
	TaoTamGiacDay(14, 17, 12, 9);
	TaoTamGiacDay(12, 17, 10, 9);
	TaoTamGiacDay(10, 17, 8, 9);
	TaoTamGiacDay(8, 17, 6, 9);
	TaoTamGiacDay(6, 17, 4, 9);
	TaoTamGiacDay(4, 17, 2, 9);

	// Mỗi lệnh 3 điểm, 8 lệnh 24 điểm
	TaoTamGiacDay(1, 0, 3, 8);
	TaoTamGiacDay(3, 0, 5, 8);
	TaoTamGiacDay(5, 0, 7, 8);
	TaoTamGiacDay(7, 0, 9, 8);
	TaoTamGiacDay(9, 0, 11, 8);
	TaoTamGiacDay(11, 0, 13, 8);
	TaoTamGiacDay(13, 0, 15, 8);
	TaoTamGiacDay(15, 0, 1, 8);

	//Tổng 48 + 24 + 24 = 96 = biến soDinhHinhTru
}


// Hình tròn
void TaoMatBenHTron(int i, int j, int i1, int j1, int mau)
{
	vec3 phapTuyen;
	vec4 u;
	vec4 v;
	if (i == 0)
	{
		u = cacDinhHinhTron[i1][j1] - cacDinhHinhTron[i][j];
		v = cacDinhHinhTron[i1][j] - cacDinhHinhTron[i][j];
		phapTuyen = normalize(cross(u, v));
	}
	else
	{
		u = cacDinhHinhTron[i][j1] - cacDinhHinhTron[i][j];
		v = cacDinhHinhTron[i1][j1] - cacDinhHinhTron[i][j1];
		phapTuyen = normalize(cross(u, v));
	}
	// a -> b -> c -> a -> c -> d
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j]; Index++;
}

void TaoHTron()
{
	for (int j = 0; j <= 6; j++)
	{
		for (int i = 0; i <= 3; i++)
		{
			TaoMatBenHTron(i, j, i + 1, j + 1, 4);
		}
	}

	for (int i = 0; i <= 3; i++)
	{
		TaoMatBenHTron(i, 7, i + 1, 0, 3);
	}
}


void GenerateCacHinh(void)
{
	TinhDiemVaMauLapPhuong();
	TaoHinhVuong();

	TinhDiemVaMauHinhTru();
	TaoHTru();

	TinhDiemVaMauHinhTron();
	TaoHTron();
}

#pragma endregion Xử lí Generate các hình

#pragma region
// Hàm đổi _param, ví dụ 255 => 255/255 = 1
float RGBConvert(float _param)
{
	return _param / 255;
}
color4 RGBtoColor(int _R, int _G, int _B)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), 1.0);
}
color4 RGBtoColor(int _R, int _G, int _B, int _A)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), RGBConvert(_A));
}
float DEGtoRAD(float DEG)
{
	return DEG / 360 * 2 * M_PI;
}
void TronMau()
{
	mauGocTron = mauAnhSang * mauVatLieu;
	mauKhuechTanTron = mauAnhSangKhuechTan * mauVatLieuKhuechTan;
	mauPhanXaGuongTron = mauAnhSangPhanXaGuong * mauVatLieuPhanXaGuong;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, mauGocTron);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, mauKhuechTanTron);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, mauPhanXaGuongTron);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, viTriAnhSang);
	glUniform1f(glGetUniformLocation(program, "Shininess"), doBongVatLieu);
}
// Xem hình để biết về hàm này
void TaoVatLieu(color4 mauGoc, color4 mauPhanXa, color4 mauPXGuong, float doBong)
{
	mauVatLieu = mauGoc;
	mauVatLieuPhanXaGuong = mauPXGuong;
	mauVatLieuKhuechTan = mauPhanXa;
	doBongVatLieu = doBong;
	TronMau();
}
// Hàm vẽ hình lập phương tâm O,  với ma trận _matrixPhanCap * _symbol
void VeHinhLapPhuong(mat4 _symbol, mat4 _matrixPhanCap)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrixPhanCap * _symbol);
	glDrawArrays(GL_TRIANGLES, 0, soDinhLapPhuong);    /*Vẽ các tam giác*/
}
// Hàm vẽ hình trụ tâm O, bán kính đáy 0.5, chiều cao 1 với ma trận _matrix * _symbol
void VeHinhTru(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 36, soDinhHinhTru);    /*Vẽ các tam giác*/
}
// Hàm vẽ hình tròn tâm O, bán kính 0.5 với ma trận _matrix * _symbol
void VeHinhTron(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 132, soDinhHinhTron);    /*Vẽ các tam giác*/
}
#pragma endregion Các hàm tạo sẵn

#pragma region 
void KhoiTaoBoDemGPU(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh) + sizeof(arrVecPhapTuyen), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arrDinhTamGiac), arrDinhTamGiac);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac), sizeof(arrMauCacDinh), arrMauCacDinh);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh), sizeof(arrVecPhapTuyen), arrVecPhapTuyen);
}

void SetupShader(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh)));

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(33.0 / 255, 114.0 / 255, 160.0 / 255, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
#pragma endregion Tạo GPU buffer và setup shader

#pragma region  
mat4 matrixPhanCapMoiTruong; //Biến ma trận phân cấp của môi trường
mat4 matrixPhanCap;	//Biến ma trận phân cấp
mat4 MoiTruong_symbol;	//Biến tạo instance vật thể môi trường

float RungLaCay = 15; //Biến góc rung của lá cây
bool RungLaCayBenPhai = true;

color4 MT_vl; //Biến color môi trường
color4 MT_kt;
color4 MT_pxg;

// Tạo hồ nước trên mặt đất
void HoNuoc(mat4 _trans)
{
	MT_vl = RGBtoColor(32, 90, 199);
	MT_kt = RGBtoColor(106, 147, 222);
	MT_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Translate(0, -0.9, 0) * Scale(10, 0, 10);
	VeHinhTru(MoiTruong_symbol, _trans);
}

// Vẽ cái cây
void CaiCayTo(mat4 _trans)
{
	// Thân cây
	MT_vl = RGBtoColor(114, 66, 18);
	MT_kt = RGBtoColor(124, 76, 29);
	MT_pxg = RGBtoColor(124, 76, 29);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Translate(0, -1, 0) * Scale(2, 8, 2) * Translate(0, 0.5, 0);
	VeHinhTru(MoiTruong_symbol, _trans);
	// Cành
	MoiTruong_symbol = Translate(0, 2, 0) * RotateZ(-45 + RungLaCay) * Scale(1, 5, 1) * Translate(0, 0.5, 0);
	VeHinhTru(MoiTruong_symbol, _trans);
	MoiTruong_symbol = Translate(0, 1, 0) * RotateZ(45 + RungLaCay) * Scale(1, 5, 1) * Translate(0, 0.5, 0);
	VeHinhTru(MoiTruong_symbol, _trans);

	// Rễ
	MoiTruong_symbol = Translate(0, -0.8, 0) * RotateY(-11) * RotateZ(100) * Scale(0.5, 3, 0.5) * Translate(0, 0.5, 0);
	VeHinhTru(MoiTruong_symbol, _trans);
	MoiTruong_symbol = Translate(0, -0.7, 0) * RotateY(69) * RotateZ(100) * Scale(0.7, 3, 0.8) * Translate(0, 0.5, 0);
	VeHinhTru(MoiTruong_symbol, _trans);
	MoiTruong_symbol = Translate(0, -0.7, 0) * RotateY(195) * RotateZ(100) * Scale(0.7, 3, 0.4) * Translate(0, 0.5, 0);
	VeHinhTru(MoiTruong_symbol, _trans);

	// Lá cây
	MT_vl = RGBtoColor(61, 187, 52);
	MT_kt = RGBtoColor(77, 196, 68);
	MT_pxg = RGBtoColor(132, 208, 127);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Translate(0, 10, 0) * Scale(10, 10, 10);
	VeHinhTron(MoiTruong_symbol, _trans);
	MoiTruong_symbol = RotateZ(RungLaCay) * Translate(5, 7, 0) * RotateZ(34) * RotateX(56) * RotateY(78) * Scale(6, 6, 7);
	VeHinhTron(MoiTruong_symbol, _trans);
	MoiTruong_symbol = RotateZ(RungLaCay) * Translate(-4, 5, 0) * RotateZ(34) * RotateX(56) * RotateY(78) * Scale(4, 3, 5);
	VeHinhTron(MoiTruong_symbol, _trans);

}

float viTriXe[15];
float _viTriXe[15];
color4 colorNha[4] = { RGBtoColor(63,72,204),RGBtoColor(63 ,204,70), RGBtoColor(204,176,63), RGBtoColor(148,63,204) };

void XeCo(color4 _color, float _vt)
{
	MT_vl = _color;
	MT_kt = _color;
	MT_pxg = _color;
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 1000);
	MoiTruong_symbol = Translate(-55, -1, _vt) * Scale(3, 2, 6) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(MoiTruong_symbol, matrixPhanCap);
}
void _XeCo(color4 _color, float _vt)
{
	MT_vl = _color;
	MT_kt = _color;
	MT_pxg = _color;
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 1000);
	MoiTruong_symbol = Translate(-65, -1, _vt) * Scale(3, 2, 6) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(MoiTruong_symbol, matrixPhanCap);
}

void MT_Duong()
{
	MT_vl = RGBtoColor(10, 10, 10);
	MT_kt = RGBtoColor(30, 30, 30);
	MT_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Translate(-60, -1, 0) * Scale(20, 0.01, 1000);
	VeHinhLapPhuong(MoiTruong_symbol, matrixPhanCap);

	MT_vl = RGBtoColor(150, 150, 150);
	MT_kt = RGBtoColor(150, 150, 150);
	MT_pxg = RGBtoColor(150, 150, 150);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Translate(-60, -0.99, 0) * Scale(1, 0.01, 1000);
	VeHinhLapPhuong(MoiTruong_symbol, matrixPhanCap);

	//For Xe Cộ
	for (int i = 0; i < 15; i++)
	{
		XeCo(colorNha[i % 4], viTriXe[i]);
	}

	for (int i = 0; i < 15; i++)
	{
		_XeCo(colorNha[i % 4], _viTriXe[i]);
	}
}

void Nha(color4 _color, mat4 _trans, float _cao)
{
	MT_vl = _color;
	MT_kt = _color;
	MT_pxg = _color;
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = _trans * Translate(0, -1, 0) * Scale(10, _cao, 10) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(MoiTruong_symbol, matrixPhanCap);
}

// Vẽ cái sân
void Dat_Nen()
{
	for (int i = 0; i < 10; i++)
	{
		Nha(colorNha[i % 4], Translate(55, 0, -45 + i * 10), (i % 4 + 1) * 10);
	}
	for (int i = 0; i <= 10; i++)
	{
		Nha(colorNha[i % 4], Translate(-45 + i * 10, 0, -55), (i % 4 + 1) * 10);
	}
	for (int i = 0; i <= 10; i++)
	{
		Nha(colorNha[i % 4], Translate(-45 + i * 10, 0, 55), (i % 4 + 1) * 10);
	}

	MT_Duong();
	// Đất
	MT_vl = RGBtoColor(129, 133, 65);
	MT_kt = RGBtoColor(30, 30, 30);
	MT_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 1000);
	MoiTruong_symbol = Translate(0, -1.1, 0) * Scale(100, 0.01, 100);
	VeHinhLapPhuong(MoiTruong_symbol, matrixPhanCap);

	// Vẽ hồ nước
	HoNuoc(Translate(-30, 0, 20));

	// Vẽ cái cây
	CaiCayTo(Translate(-19, 0.1, -23) * RotateY(145));
	CaiCayTo(Translate(-18, 0.2, 16) * RotateY(75) * Scale(0.7, 0.7, 0.7));


	// Vẽ vài quả bóng màu đỏ
	MT_vl = RGBtoColor(170, 24, 24);
	MT_kt = RGBtoColor(205, 76, 76);
	MT_pxg = RGBtoColor(224, 145, 145);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Translate(4, -1 + 0.25, -5) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(MoiTruong_symbol, matrixPhanCap);
	MoiTruong_symbol = Translate(-5, -1 + 0.25, -8) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(MoiTruong_symbol, matrixPhanCap);
	MoiTruong_symbol = Translate(-5, -1 + 0.25, 8) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(MoiTruong_symbol, matrixPhanCap);
	MoiTruong_symbol = Translate(5, -1 + 0.25, 8) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(MoiTruong_symbol, matrixPhanCap);

}

void Mat_Co(mat4 _trans)
{
	MT_vl = RGBtoColor(33, 222, 66);
	MT_kt = RGBtoColor(33, 222, 66);
	MT_pxg = RGBtoColor(33, 222, 66);
	TaoVatLieu(MT_vl, MT_kt, MT_pxg, 100);
	MoiTruong_symbol = Scale(80, 0.01, 80);
	VeHinhLapPhuong(MoiTruong_symbol, _trans);
}

void MoiTruong()
{
	matrixPhanCapMoiTruong = mat4();
	Dat_Nen();
	Mat_Co(Translate(0, -0.998, 0));

}
#pragma endregion Môi Trường

#pragma region Lê Hoàng Thái - Vẽ Bàn, Ghế, Đồ Trang Trí
mat4 matrixPhanCapThai;
mat4 Thai_symbol;
color4 Thai_vl;
color4 Thai_kt;
color4 Thai_pxg;

//Tạo bộ bàn vuông
void BoBanTron()
{
	//Thân bàn tròn
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Scale(0.2, 1, 0.2);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);

	//Mặt bàn tròn
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(0, 0.5, 0) * Scale(2, 0.2, 2);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);

	//Chân bàn tròn
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(0, -0.45, 0) * Scale(0.7, 0.1, 0.2);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(0, -0.45, 0) * Scale(0.2, 0.1, 0.7);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Ghế tròn
	Thai_symbol = Translate(1.6, -0.2, 0) * Scale(0.6, 0.5, 0.6);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(-1.6, -0.2, 0) * Scale(0.6, 0.5, 0.6);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(0, -0.2, 1.6) * Scale(0.6, 0.5, 0.6);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(0, -0.2, -1.6) * Scale(0.6, 0.5, 0.6);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
}

//Tạo bộ bàn vuông
void BoBanVuong()
{
	//Thân bàn vuông
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Scale(0.2, 1, 0.2);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Mặt bàn vuông
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(0, 0.5, 0) * Scale(2, 0.2, 2);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Chân bàn vuông
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(0, -0.45, 0) * Scale(0.7, 0.1, 0.2);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(0, -0.45, 0) * Scale(0.2, 0.1, 0.7);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Ghế vuông
	Thai_symbol = Translate(1.8, -0.2, 0) * Scale(0.6, 0.5, 0.6);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(-1.8, -0.2, 0) * Scale(0.6, 0.5, 0.6);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(0, -0.2, 1.8) * Scale(0.6, 0.5, 0.6);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(0, -0.2, -1.8) * Scale(0.6, 0.5, 0.6);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

}

//Tạo bàn vuông dài
void BanVuongDai()
{
	//Mặt bàn vuông
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Scale(6, 0.1, 2);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Chân bàn
	Thai_symbol = Translate(-2.7, -0.4, 0) * Scale(0.1, 0.8, 1.6);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(2.7, -0.4, 0) * Scale(0.1, 0.8, 1.6);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
}

void GheDai()
{
	//Mặt ghế dài
	Thai_vl = RGBtoColor(132, 90, 199);
	Thai_kt = RGBtoColor(106, 147, 222);
	Thai_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Scale(5, 0.1, 1);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Chân ghế dài
	Thai_symbol = Translate(-2.2, -0.25, 0) * Scale(0.1, 0.5, 0.8);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(2.2, -0.25, 0) * Scale(0.1, 0.5, 0.8);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
}

//Sân khấu
void SanKhau()
{
	//Banner
	Thai_vl = RGBtoColor(170, 170, 90);
	Thai_kt = RGBtoColor(180, 180, 90);
	Thai_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Scale(0.2, 10, 20);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Bục đứng
	Thai_vl = RGBtoColor(170, 70, 90);
	Thai_kt = RGBtoColor(180, 80, 90);
	Thai_pxg = RGBtoColor(190, 90, 95);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(-5, -4.5, 0) * Scale(12, 1, 30);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);

	//Dây buộc
	Thai_vl = RGBtoColor(25, 0, 0);
	Thai_kt = RGBtoColor(25, 0, 0);
	Thai_pxg = RGBtoColor(25, 0, 0);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(1.8, -1.5, -9) * RotateZ(25) * Scale(0.1, 8, 0.1);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(1.8, -1.5, 9) * RotateZ(25) * Scale(0.1, 8, 0.1);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);

	//Loa trái
	Thai_vl = RGBtoColor(155, 145, 14);
	Thai_kt = RGBtoColor(145, 140, 14);
	Thai_pxg = RGBtoColor(135, 135, 13);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(-0.5, -2, -12.5) * RotateY(25) * Scale(2, 4, 3);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_vl = RGBtoColor(15, 14, 14);
	Thai_kt = RGBtoColor(14, 14, 14);
	Thai_pxg = RGBtoColor(13, 13, 13);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(-1.3, -1, -12) * RotateY(25) * RotateZ(90) * Scale(1.5, 0.3, 1.5);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(-1.3, -3, -12) * RotateY(25) * RotateZ(90) * Scale(1.5, 0.3, 1.5);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);

	//Loa phải
	Thai_vl = RGBtoColor(155, 145, 14);
	Thai_kt = RGBtoColor(145, 140, 14);
	Thai_pxg = RGBtoColor(135, 135, 13);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(-0.5, -2, 12.5) * RotateY(-25) * Scale(2, 4, 3);
	VeHinhLapPhuong(Thai_symbol, matrixPhanCapThai);
	Thai_vl = RGBtoColor(15, 14, 14);
	Thai_kt = RGBtoColor(14, 14, 14);
	Thai_pxg = RGBtoColor(13, 13, 13);
	TaoVatLieu(Thai_vl, Thai_kt, Thai_pxg, 100);
	Thai_symbol = Translate(-1.3, -1, 12) * RotateY(-25) * RotateZ(90) * Scale(1.5, 0.3, 1.5);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
	Thai_symbol = Translate(-1.3, -3, 12) * RotateY(-25) * RotateZ(90) * Scale(1.5, 0.3, 1.5);
	VeHinhTru(Thai_symbol, matrixPhanCapThai);
}

//Lê Hoàng Thái - Ma trận phân cấp
void LeHoangThai()
{
	matrixPhanCapThai = mat4();
	mat4 matrixTam = matrixPhanCapThai;
	//Hàng gần sân khấu
	matrixPhanCapThai = matrixTam * Translate(10, -0.5, -10);
	BoBanTron();
	matrixPhanCapThai = matrixTam * Translate(0, -0.5, -20);
	BoBanTron();
	matrixPhanCapThai = matrixTam * Translate(10, -0.5, -20);
	BoBanVuong();
	//Hàng 2
	matrixPhanCapThai = matrixTam * Translate(0, -0.5, -10);
	BoBanVuong();
	matrixPhanCapThai = matrixTam * Translate(10, -0.15, 0);
	BanVuongDai();
	matrixPhanCapThai = matrixTam * Translate(10, -0.4, -2.5);
	GheDai();
	matrixPhanCapThai = matrixTam * Translate(10, -0.4, 2.5);
	GheDai();
	matrixPhanCapThai = matrixTam * RotateY(90) * Translate(0, -0.15, 0);
	BanVuongDai();
	matrixPhanCapThai = matrixTam * RotateY(90) * Translate(0, -0.4, -2.5);
	GheDai();
	//Hàng cuối
	matrixPhanCapThai = matrixTam * Translate(-10, -0.5, -20);
	BoBanVuong();
	matrixPhanCapThai = matrixTam * Translate(-10, -0.5, -10);
	BoBanVuong();
	matrixPhanCapThai = matrixTam * Translate(-10, -0.5, 0);
	BoBanVuong();
	//Sân khấu
	matrixPhanCapThai = matrixTam * Translate(35, 4, 0);
	SanKhau();

}
#pragma endregion

#pragma region Nguyễn Lam Trường - Vẽ Cổng, Hàng Rào
mat4 matrixPhanCapTruong;	 // Biến ma trận phân cấp
mat4 Truong_symbol;		// Biến tạo instance vật thể
color4 Truong_vl;	// Biến color vật liệu
color4 Truong_kt;	// Biến color khuếch tán
color4 Truong_pxg;	// Biến color phản xạ gương

//Hàng rào phía trước
void Hang_Rao()
{
	Truong_kt = RGBtoColor(170, 170, 85);
	Truong_kt = RGBtoColor(180, 180, 90);
	Truong_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);

	//Hai thanh ngang
	Truong_symbol = Scale(0.1, 0.1, 35);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
	Truong_symbol = Translate(0, 1.5, 0) * Scale(0.1, 0.1, 35);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);

	//Các thanh dọc
	for (int i = -17; i < 18; i++)
	{
		Truong_symbol = Translate(0, 0.5, i) * Scale(0.1, 3, 0.1);
		VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
	}
}

//Hàng rào phía sau và 2 bên
void Hang_Rao_Full()
{
	Truong_kt = RGBtoColor(170, 170, 85);
	Truong_kt = RGBtoColor(180, 180, 90);
	Truong_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);

	//Hai thanh ngang
	Truong_symbol = Scale(0.1, 0.1, 80);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
	Truong_symbol = Translate(0, 1.5, 0) * Scale(0.1, 0.1, 80);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);

	//Các thanh dọc
	for (int i = -40; i < 41; i++)
	{
		Truong_symbol = Translate(0, 0.5, i) * Scale(0.1, 3, 0.1);
		VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
	}
}

GLfloat moCanhCong = 75;
//Tạo Cánh cổng
void CanhCong()
{
	//Cánh cổng
	Truong_kt = RGBtoColor(170, 170, 185);
	Truong_kt = RGBtoColor(180, 180, 190);
	Truong_pxg = RGBtoColor(190, 190, 195);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);
	Truong_symbol = Translate(0, 0, 2.5) * Scale(0.1, 4, 5);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);

	//Tay nắm cổng
	Truong_kt = RGBtoColor(170, 170, 85);
	Truong_kt = RGBtoColor(180, 180, 90);
	Truong_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);
	Truong_symbol = Translate(0, 0, 4) * Scale(0.2, 0.5, 0.1);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
}

//Tạo Khung cổng
void KhungCong()
{
	Truong_kt = RGBtoColor(170, 170, 85);
	Truong_kt = RGBtoColor(180, 180, 90);
	Truong_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);
	Truong_symbol = Translate(0, 0, 2.5) * Scale(0.6, 6, 0.6);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
}

//Tạo Mái che cổng
void MaiCheCong()
{
	//Miếng mái che to
	Truong_kt = RGBtoColor(170, 170, 85);
	Truong_kt = RGBtoColor(180, 180, 90);
	Truong_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);
	Truong_symbol = Scale(6, 0.5, 14);
	VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);

	//Thanh trang trí bên trên mái che
	Truong_kt = RGBtoColor(117, 17, 85);
	Truong_kt = RGBtoColor(118, 18, 90);
	Truong_pxg = RGBtoColor(119, 19, 95);
	TaoVatLieu(Truong_vl, Truong_kt, Truong_pxg, 100);
	for (int i = -6; i < 7; i++)
	{
		Truong_symbol = Translate(0, 0, i) * Scale(5, 0.8, 0.2);
		VeHinhLapPhuong(Truong_symbol, matrixPhanCapTruong);
	}
}

//Nguyễn Lam Trường - Ma trận phân cấp
void NguyenLamTruong()
{
	matrixPhanCapTruong = mat4();
	mat4 matrixTam = matrixPhanCapTruong; //Biến tạm lưu ma trận phân cấp
	matrixPhanCapTruong = matrixTam * Translate(-40, 0, -22.5);
	Hang_Rao();
	matrixPhanCapTruong = matrixTam * Translate(-40, 0, 22.5);
	Hang_Rao();
	matrixPhanCapTruong = matrixTam * Translate(40, 0, 0);
	Hang_Rao_Full();
	matrixPhanCapTruong = matrixTam * RotateY(90) * Translate(-40, 0, 0);
	Hang_Rao_Full();
	matrixPhanCapTruong = matrixTam * RotateY(90) * Translate(40, 0, 0);
	Hang_Rao_Full();
	matrixPhanCapTruong = matrixTam * Translate(-40, 1, -5.01) * RotateY(moCanhCong);
	CanhCong();
	matrixPhanCapTruong = matrixTam * Translate(-40, 1, 5.01) * RotateY(-moCanhCong + 180);
	CanhCong();
	matrixPhanCapTruong = matrixTam * Translate(-40, 2, 2.8);
	KhungCong();
	matrixPhanCapTruong = matrixTam * Translate(-40, 2, -7.8);
	KhungCong();
	matrixPhanCapTruong = matrixTam * Translate(-41, 5, 0);
	MaiCheCong();
}

#pragma endregion

#pragma region Tạ Văn Ngọc - Quầy bán, Cửa Ra Vào
mat4 matrixPhanCapNgoc;	 // Biến ma trận phân cấp
mat4 Ngoc_symbol;		// Biến tạo instance vật thể
color4 Ngoc_vl;	// Biến color vật liệu
color4 Ngoc_kt;	// Biến color khuếch tán
color4 Ngoc_pxg;	// Biến color phản xạ gương

//Quầy bán
void QuayBan()
{
	//Mặt sau
	Ngoc_vl = RGBtoColor(170, 70, 90);
	Ngoc_kt = RGBtoColor(180, 80, 90);
	Ngoc_pxg = RGBtoColor(190, 90, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Scale(20, 8, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Trang trí trên cửa sổ
	Ngoc_vl = RGBtoColor(170, 70, 90);
	Ngoc_kt = RGBtoColor(180, 80, 90);
	Ngoc_pxg = RGBtoColor(190, 90, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(10.4, 3.5, -5) * Scale(0.4, 0.2, 6);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mặt trái dưới cửa sổ
	Ngoc_vl = RGBtoColor(70, 170, 90);
	Ngoc_kt = RGBtoColor(80, 180, 90);
	Ngoc_pxg = RGBtoColor(90, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(10, -2.5, -5) * Scale(0.1, 3, 10);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mặt trái trên cửa sổ
	Ngoc_vl = RGBtoColor(70, 170, 90);
	Ngoc_kt = RGBtoColor(80, 180, 90);
	Ngoc_pxg = RGBtoColor(90, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(10, 3.5, -5) * Scale(0.1, 1, 10);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mảnh bên cửa sổ trái
	Ngoc_vl = RGBtoColor(70, 170, 90);
	Ngoc_kt = RGBtoColor(80, 180, 90);
	Ngoc_pxg = RGBtoColor(90, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(10, 1, -8.5) * Scale(0.1, 5, 3);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mảnh bên cửa sổ phải
	Ngoc_vl = RGBtoColor(70, 170, 90);
	Ngoc_kt = RGBtoColor(80, 180, 90);
	Ngoc_pxg = RGBtoColor(90, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(10, 1, -1.5) * Scale(0.1, 5, 3);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mặt phải
	Ngoc_vl = RGBtoColor(70, 170, 90);
	Ngoc_kt = RGBtoColor(80, 180, 90);
	Ngoc_pxg = RGBtoColor(90, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(-10, 0, -5) * Scale(0.1, 8, 10);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mặt trên
	Ngoc_vl = RGBtoColor(70, 170, 90);
	Ngoc_kt = RGBtoColor(80, 180, 90);
	Ngoc_pxg = RGBtoColor(90, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(0, 4, -6.5) * Scale(24, 0.1, 15);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mặt nền
	Ngoc_vl = RGBtoColor(70, 70, 90);
	Ngoc_kt = RGBtoColor(80, 80, 90);
	Ngoc_pxg = RGBtoColor(90, 90, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(0, -3.8, -11) * Scale(30, 0.4, 24);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Mặt trước
	Ngoc_vl = RGBtoColor(177, 170, 90);
	Ngoc_kt = RGBtoColor(188, 180, 90);
	Ngoc_pxg = RGBtoColor(199, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(9.5, 0, -10) * Scale(1, 8.1, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Thanh phía trước gắn với cửa đi
	Ngoc_vl = RGBtoColor(177, 170, 90);
	Ngoc_kt = RGBtoColor(188, 180, 90);
	Ngoc_pxg = RGBtoColor(199, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(-9.5, -0.5, -10) * Scale(1, 6.2, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Thanh phía trước gần cửa đi
	Ngoc_vl = RGBtoColor(177, 170, 90);
	Ngoc_kt = RGBtoColor(188, 180, 90);
	Ngoc_pxg = RGBtoColor(199, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(-4.5, -0.5, -10) * Scale(3, 6.5, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Thanh phía trước gần tường
	Ngoc_vl = RGBtoColor(177, 170, 90);
	Ngoc_kt = RGBtoColor(188, 180, 90);
	Ngoc_pxg = RGBtoColor(199, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(3, -2.5, -10) * Scale(12, 3, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Thanh để bán hàng
	Ngoc_vl = RGBtoColor(170, 70, 255);
	Ngoc_kt = RGBtoColor(180, 80, 255);
	Ngoc_pxg = RGBtoColor(190, 90, 255);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(3, -1, -8.75) * Scale(14, 0.1, 2.5);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Thanh phía trên mặt trước
	Ngoc_vl = RGBtoColor(177, 170, 90);
	Ngoc_kt = RGBtoColor(188, 180, 90);
	Ngoc_pxg = RGBtoColor(199, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(0, 3.25, -10) * Scale(20, 1.5, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Cột chống chỗ bán bên trong tường
	Ngoc_vl = RGBtoColor(17, 170, 90);
	Ngoc_kt = RGBtoColor(18, 180, 90);
	Ngoc_pxg = RGBtoColor(19, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(8, -2.5, -8) * Scale(0.3, 3, 0.3);
	VeHinhTru(Ngoc_symbol, matrixPhanCapNgoc);

	//Cột chống chỗ bán gần cửa đi
	Ngoc_vl = RGBtoColor(17, 170, 90);
	Ngoc_kt = RGBtoColor(18, 180, 90);
	Ngoc_pxg = RGBtoColor(19, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(-3, -2.5, -8) * Scale(0.3, 3, 0.3);
	VeHinhTru(Ngoc_symbol, matrixPhanCapNgoc);

}

//Máy bán hàng
void MayBanHang()
{
	//Mặt đế máy bán hàng
	Ngoc_vl = RGBtoColor(170, 170, 90);
	Ngoc_kt = RGBtoColor(180, 180, 90);
	Ngoc_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Scale(1, 0.2, 1);
	VeHinhTru(Ngoc_symbol, matrixPhanCapNgoc);

	//Thân máy bán hàng
	Ngoc_vl = RGBtoColor(170, 170, 90);
	Ngoc_kt = RGBtoColor(180, 180, 90);
	Ngoc_pxg = RGBtoColor(190, 190, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(0, 0.25, 0) * Scale(0.4, 0.5, 0.2);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Màn hình máy bán hàng
	Ngoc_vl = RGBtoColor(17, 170, 190);
	Ngoc_kt = RGBtoColor(18, 180, 190);
	Ngoc_pxg = RGBtoColor(19, 190, 195);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 100);
	Ngoc_symbol = Translate(0, 0.75, -0.1) * RotateX(-30) * Scale(1.2, 0.8, 0.2);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);
}

//Cửa ra vào quầy bán
GLfloat moCuaRaVaoQuan = 60;
void CuaRaVaoQuayBan()
{
	//Cửa ra vào quầy bán
	Ngoc_vl = RGBtoColor(70, 70, 190);
	Ngoc_kt = RGBtoColor(80, 80, 190);
	Ngoc_pxg = RGBtoColor(90, 90, 195);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 1000);
	Ngoc_symbol = Translate(1.5, -0.05, 0) * Scale(3, 6.1, 0.1);
	VeHinhLapPhuong(Ngoc_symbol, matrixPhanCapNgoc);

	//Tay nắm cửa
	Ngoc_vl = RGBtoColor(170, 70, 90);
	Ngoc_kt = RGBtoColor(180, 80, 90);
	Ngoc_pxg = RGBtoColor(190, 90, 95);
	TaoVatLieu(Ngoc_vl, Ngoc_kt, Ngoc_pxg, 1000);
	Ngoc_symbol = Translate(2.5, 0, 0) * Scale(0.2, 0.6, 0.4);
	VeHinhTru(Ngoc_symbol, matrixPhanCapNgoc);
}

//Tạ Văn Ngọc - Ma trận phân cấp Ngọc
void TaVanNgoc()
{
	matrixPhanCapNgoc = mat4();
	mat4 matrixTam = matrixPhanCapNgoc;
	matrixPhanCapNgoc = matrixTam * Translate(0, 3, 38);
	QuayBan();
	matrixPhanCapNgoc = matrixTam * Translate(-2, 2, 29.5);
	MayBanHang();
	matrixPhanCapNgoc = matrixTam * Translate(0, 2, 28.6) * RotateY(180);
	MayBanHang();
	matrixPhanCapNgoc = matrixTam * Translate(-9, 2.5, 28) * RotateY(-moCuaRaVaoQuan);
	CuaRaVaoQuayBan();
}
#pragma endregion

#pragma region Phùng Bá Quý - Tủ bánh ngọt, Cửa Sổ, Đèn
mat4 matrixPhanCapQuy;	 // Biến ma trận phân cấp
mat4 Quy_symbol;		// Biến tạo instance vật thể
color4 Quy_vl;	// Biến color vật liệu
color4 Quy_kt;	// Biến color khuếch tán
color4 Quy_pxg;	// Biến color phản xạ gương

GLfloat moCuaSo = 45;
//Cửa số quầy bán
void CuaSoQuayBan()
{
	Quy_vl = RGBtoColor(70, 17, 90);
	Quy_kt = RGBtoColor(10, 80, 90);
	Quy_pxg = RGBtoColor(10, 10, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 0, 1) * Scale(0.1, 4, 2);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Tay nắm cửa
	Quy_vl = RGBtoColor(70, 170, 90);
	Quy_kt = RGBtoColor(10, 180, 90);
	Quy_pxg = RGBtoColor(10, 190, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 0, 1.4) * Scale(0.3, 0.4, 0.1);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);
}

GLfloat moTuBanhNgot = 0.6;
//Tủ bánh ngọt
void TuBanhNgot()
{
	//Mặt đế
	Quy_vl = RGBtoColor(70, 17, 90);
	Quy_kt = RGBtoColor(10, 80, 90);
	Quy_pxg = RGBtoColor(10, 10, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Scale(3, 0.2, 6);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Mặt trên
	Quy_vl = RGBtoColor(70, 17, 90);
	Quy_kt = RGBtoColor(10, 80, 90);
	Quy_pxg = RGBtoColor(10, 10, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 2.95, 0) * Scale(3, 0.1, 6);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Mặt để bánh
	Quy_vl = RGBtoColor(70, 17, 90);
	Quy_kt = RGBtoColor(10, 80, 90);
	Quy_pxg = RGBtoColor(10, 10, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.2, 0.75, 0) * Scale(2.6, 0.1, 5.85);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	Quy_vl = RGBtoColor(70, 17, 90);
	Quy_kt = RGBtoColor(10, 80, 90);
	Quy_pxg = RGBtoColor(10, 10, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.2, 1.6, 0) * Scale(2.6, 0.1, 5.85);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//4 cái chân
	Quy_vl = RGBtoColor(70, 17, 90);
	Quy_kt = RGBtoColor(10, 80, 90);
	Quy_pxg = RGBtoColor(10, 10, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(1, -0.15, 2) * Scale(0.3, 0.6, 0.3);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(-1, -0.15, 2) * Scale(0.3, 0.6, 0.3);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(1, -0.15, -2) * Scale(0.3, 0.6, 0.3);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(-1, -0.15, -2) * Scale(0.3, 0.6, 0.3);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

	//2 Mặt bên
	Quy_vl = RGBtoColor(170, 170, 90);
	Quy_kt = RGBtoColor(170, 180, 90);
	Quy_pxg = RGBtoColor(170, 190, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 1.5, 3) * Scale(3, 3, 0.1);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(0, 1.5, -3) * Scale(3, 3, 0.1);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Mặt trước
	Quy_vl = RGBtoColor(170, 170, 90);
	Quy_kt = RGBtoColor(170, 180, 90);
	Quy_pxg = RGBtoColor(170, 190, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-1.5, 0.4, 0) * Scale(0.1, 0.8, 6);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	Quy_symbol = Translate(-1.5, 1.6, 0) * Scale(0.1, 0.1, 6);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Mặt sau chỗ mở tủ
	Quy_vl = RGBtoColor(170, 70, 190);
	Quy_kt = RGBtoColor(170, 80, 190);
	Quy_pxg = RGBtoColor(170, 90, 195);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(1.5, 1.5, 0) * Scale(0.1, 3, 2);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Cánh cửa tủ bánh ngọt
	Quy_vl = RGBtoColor(70, 170, 90);
	Quy_kt = RGBtoColor(70, 180, 90);
	Quy_pxg = RGBtoColor(70, 190, 95);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(1.475, 1.5, 2 - moTuBanhNgot) * Scale(0.1, 2.9, 2);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(1.475, 1.5, -2 + moTuBanhNgot) * Scale(0.1, 2.9, 2);
	VeHinhLapPhuong(Quy_symbol, matrixPhanCapQuy);

	//Bánh ngọt 1 tầng
	Quy_vl = RGBtoColor(255, 17, 9);
	Quy_kt = RGBtoColor(255, 18, 9);
	Quy_pxg = RGBtoColor(255, 19, 9);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.1, 0.95, -2) * Scale(1, 0.4, 1);	//Bên trái
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(-0.1, 0.95, 0) * Scale(1, 0.4, 1);	//Bên giữa
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(-0.1, 0.95, 2) * Scale(1, 0.4, 1);	//Bên phải
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

	//Bánh ngọt 2 tầng bên trái
	//Tầng 1/2
	Quy_vl = RGBtoColor(255, 17, 90);
	Quy_kt = RGBtoColor(255, 18, 100);
	Quy_pxg = RGBtoColor(255, 19, 110);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.1, 1.95, -1) * Scale(1, 0.4, 1);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	//Tầng 2/2
	Quy_vl = RGBtoColor(25, 117, 90);
	Quy_kt = RGBtoColor(25, 118, 100);
	Quy_pxg = RGBtoColor(25, 119, 110);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.1, 2.2, -1) * Scale(0.6, 0.4, 0.6);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

	//Bánh ngọt 2 tầng bên phải
	//Tầng 1/2
	Quy_vl = RGBtoColor(255, 17, 90);
	Quy_kt = RGBtoColor(255, 18, 100);
	Quy_pxg = RGBtoColor(255, 19, 110);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.1, 1.95, 1) * Scale(1, 0.4, 1);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	//Tầng 2/2
	Quy_vl = RGBtoColor(25, 117, 90);
	Quy_kt = RGBtoColor(25, 118, 100);
	Quy_pxg = RGBtoColor(25, 119, 110);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(-0.1, 2.2, 1) * Scale(0.6, 0.4, 0.6);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

}

//Đèn cổng
bool doiMauDenCong = true;
GLint colorDenCong = 10;
void DenCong()
{
	//Dây đèn
	Quy_vl = RGBtoColor(10, 17, 50);
	Quy_kt = RGBtoColor(10, 20, 50);
	Quy_pxg = RGBtoColor(10, 25, 55);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 0.85, 0) * Scale(0.08, 1, 0.08);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

	//Bóng đèn
	Quy_vl = RGBtoColor(colorDenCong, colorDenCong + 10, colorDenCong + 20);
	Quy_kt = RGBtoColor(colorDenCong - 10, colorDenCong + 10, colorDenCong + 20);
	Quy_pxg = RGBtoColor(colorDenCong - 20, colorDenCong + 10, colorDenCong + 20);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Scale(0.8, 0.8, 0.8);
	VeHinhTron(Quy_symbol, matrixPhanCapQuy);
}

//Đèn sân khấu
void DenSanKhau()
{
	//Cột đèn sân khấu
	Quy_vl = RGBtoColor(10, 17, 50);
	Quy_kt = RGBtoColor(10, 20, 50);
	Quy_pxg = RGBtoColor(10, 25, 55);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 0, -18) * Scale(0.4, 6, 0.4);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	Quy_symbol = Translate(0, 0, 18) * Scale(0.4, 6, 0.4);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

	//Dây đèn sân khấu
	Quy_vl = RGBtoColor(10, 17, 50);
	Quy_kt = RGBtoColor(10, 20, 50);
	Quy_pxg = RGBtoColor(10, 25, 55);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	Quy_symbol = Translate(0, 2.5, 0) * RotateX(90) * Scale(0.15, 35.9, 0.15);
	VeHinhTru(Quy_symbol, matrixPhanCapQuy);

	//Dây điện đèn sân khấu
	Quy_vl = RGBtoColor(100, 17, 50);
	Quy_kt = RGBtoColor(100, 20, 50);
	Quy_pxg = RGBtoColor(100, 25, 55);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	for (int i = -14; i < 15; i += 2)
	{
		Quy_symbol = Translate(0, 2.25, i) * Scale(0.1, 0.5, 0.1);
		VeHinhTru(Quy_symbol, matrixPhanCapQuy);
	}

	//Bóng đèn sân khấu
	Quy_vl = RGBtoColor(100, 17, 50);
	Quy_kt = RGBtoColor(100, 20, 50);
	Quy_pxg = RGBtoColor(100, 25, 55);
	TaoVatLieu(Quy_vl, Quy_kt, Quy_pxg, 100);
	for (int i = -14; i < 15; i += 2)
	{
		Quy_symbol = Translate(0, 1.9, i) * Scale(0.4, 0.4, 0.4);
		VeHinhTron(Quy_symbol, matrixPhanCapQuy);
	}
}

//Phùng Bá Quý - Ma trận phân cấp Quý
void PhungBaQuy()
{
	matrixPhanCapQuy = mat4();
	mat4 matrixTam = matrixPhanCapQuy;
	matrixPhanCapQuy = matrixTam * Translate(10, 4, 31) * RotateY(moCuaSo);
	CuaSoQuayBan();
	matrixPhanCapQuy = matrixTam * Translate(10, 4, 35) * RotateY(-moCuaSo + 180);
	CuaSoQuayBan();
	matrixPhanCapQuy = matrixTam * Translate(6, -0.2, 20) * RotateY(-90);
	TuBanhNgot();
	matrixPhanCapQuy = matrixTam * Translate(-42.5, 3.8, -5.5);
	DenCong();
	matrixPhanCapQuy = matrixTam * Translate(-42.5, 3.8, 5.5);
	DenCong();
	matrixPhanCapQuy = matrixTam * Translate(23, 1.85, 0);
	DenSanKhau();
}
#pragma endregion

#pragma region 
void DisplayModels(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrixPhanCap = mat4();
	MoiTruong();
	LeHoangThai();
	NguyenLamTruong();
	TaVanNgoc();
	PhungBaQuy();

	at = vec4(cosf(DEGtoRAD(cam_Rotation[0])), 0, cosf(DEGtoRAD(cam_Rotation[2])), 0) + eye;
	view = RotateX(cam_Rotation[1]) * LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	projection = Frustum(-0.01, 0.01, -0.01, 0.01, 0.02, 50);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glViewport(0, 0, 1000, 1000);
	glutPostRedisplay();
	glutSwapBuffers();
}

void KeyboardControl(unsigned char key, int x, int y)
{
	vec3 cam_forward = normalize(vec3(cosf(DEGtoRAD(cam_Rotation[0])) * abs(cosf(DEGtoRAD(cam_Rotation[1]))),
		-sinf(DEGtoRAD(cam_Rotation[1])),
		cosf(DEGtoRAD(cam_Rotation[2])) * abs(cosf(DEGtoRAD(cam_Rotation[1])))));
	vec3 cam_right = normalize(cross(cam_forward, up));
	vec4 cam_up = normalize(cross(cam_right, cam_forward));

	switch (key) {
		/*case 033:
			exit(1);
			break;*/
	case 'f':
		cam_Rotation[0] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		cam_Rotation[2] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'h':
		cam_Rotation[0] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		cam_Rotation[2] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 't':
		cam_Rotation[1] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		if (cam_Rotation[1] <= -90) cam_Rotation[1] = -90;
		break;
	case 'g':
		cam_Rotation[1] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		if (cam_Rotation[1] >= 90) cam_Rotation[1] = 90;
		break;
	case 'w':
		eye = eye + cam_forward * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 's':
		eye = eye - cam_forward * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'a':
		eye = eye - cam_right * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'd':
		eye = eye + cam_right * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'q':
		eye = eye - cam_up * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'e':
		eye = eye + cam_up * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'r':
		che_do_chi_tiet = !che_do_chi_tiet;
		break;
		//Mở/ Đóng cổng
	case 'm':
		moCanhCong += 5;
		if (moCanhCong >= 160)
		{
			moCanhCong = 160;
		}
		break;
	case 'M':
		moCanhCong -= 5;
		if (moCanhCong <= 0)
		{
			moCanhCong = 0;
		}
		break;
		//Mở/ Đóng cửa sổ
	case 'o':
		moCuaSo += 5;
		if (moCuaSo >= 160)
		{
			moCuaSo = 160;
		}
		break;
	case 'O':
		moCuaSo -= 5;
		if (moCuaSo <= 0)
		{
			moCuaSo = 0;
		}
		break;
		//Mở/ Đóng tủ bánh ngọt
	case 'p':
		moTuBanhNgot += 0.1;
		if (moTuBanhNgot >= 1.5)
		{
			moTuBanhNgot = 1.5;
		}
		break;
	case 'P':
		moTuBanhNgot -= 0.1;
		if (moTuBanhNgot <= 0)
		{
			moTuBanhNgot = 0;
		}
		break;
		//Mở/ Đóng cửa ra/vào quán
	case 'n':
		moCuaRaVaoQuan += 5;
		if (moCuaRaVaoQuan >= 80)
		{
			moCuaRaVaoQuan = 80;
		}
		break;
	case 'N':
		moCuaRaVaoQuan -= 5;
		if (moCuaRaVaoQuan <= 0)
		{
			moCuaRaVaoQuan = 0;
		}
		break;
	}
}

void IdleControl()
{
	if (RungLaCayBenPhai)
	{
		RungLaCay += 0.05;
		if (RungLaCay >= 1) RungLaCayBenPhai = !RungLaCayBenPhai;
	}
	else
	{
		RungLaCay -= 0.05;
		if (RungLaCay <= -1) RungLaCayBenPhai = !RungLaCayBenPhai;
	}

	if (doiMauDenCong)
	{
		colorDenCong += 2;
		if (colorDenCong >= 255) doiMauDenCong = !doiMauDenCong;
	}
	else
	{
		colorDenCong -= 2;
		if (colorDenCong <= 1) doiMauDenCong = !doiMauDenCong;
	}


	for (int i = 0; i < 15; i++)
	{
		viTriXe[i] += 1;
		if (500 < viTriXe[i]) viTriXe[i] = -500;
		_viTriXe[i] -= 1;
		if (-500 > _viTriXe[i]) _viTriXe[i] = 500;
	}
}
#pragma endregion Các hàm hiển thị model, camera, bàn phím.

int main(int argc, char** argv)
{
	for (int i = 0; i < 15; i++)
	{
		viTriXe[i] = -i * 70;
		_viTriXe[i] = i * 70;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Build Garden Coffee");


	glewInit();

	GenerateCacHinh();
	KhoiTaoBoDemGPU();
	SetupShader();

	glutDisplayFunc(DisplayModels);
	glutKeyboardFunc(KeyboardControl);
	glutIdleFunc(IdleControl);
	//glutReshapeFunc(CameraControl);

	glutMainLoop();
	return 0;
}