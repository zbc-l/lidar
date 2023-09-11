#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
	std::vector<uint16_t> ref;
	const int vector_size = 78750;

	// 设置随机数生成器
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dis(0, 100);

	// 生成随机数并填充到vector中
	for (int i = 0; i < vector_size; ++i) {
		uint16_t random_number = dis(gen);
		ref.push_back(random_number);
	}
	//std::vector<uint16_t> ref(78750); // 假设 ref 是一个 uint16_t 的 vector，已经有数据填充了
	std::vector<uint16_t> azimuthref; // 假设 azimuthref 也是一个 uint16_t 的 vector

	// 首次插入 ref
	azimuthref.insert(azimuthref.end(), ref.begin(), ref.end());

	// 修改 ref 的内容，将所有为 0 的值修改为正整数
	for (size_t i = 0; i < ref.size(); ++i) {
		if (ref[i] == 10) {
			ref[i] = 42; // 假设将所有为 0 的值修改为 42
		}
	}

	// 再次插入修改后的 ref，确保深拷贝
	azimuthref.insert(azimuthref.end(), ref.begin(), ref.end());

	// 将 azimuthref 存储为二进制文件
	std::ofstream outfile("azimuthref.bin", std::ios::binary);
	outfile.write(reinterpret_cast<const char*>(azimuthref.data()), azimuthref.size() * sizeof(uint16_t));
	outfile.close();

	std::cout << "azimuthref 已保存为二进制文件。" << std::endl;

	return 0;
}
